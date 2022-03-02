/*
* @file_name: HttpSplitter.cpp
* @date: 2021/12/06
* @author: oaho
* Copyright @ hz oaho, All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "HttpSplitter.hpp"
#include "Util/string_view.hpp"
#include <Util/logger.h>
#include <Util/util.h>

static auto getChunkedLength(const char* data, size_t length) -> size_t{
  size_t len = 0;
  for(size_t i = 0; i < length;i++){
    char c = data[i];
    if(i == 0){
      if('0' <= c && c <= '9'){
        len = c - '0';
      }
      else if('a' <= c && c <= 'f'){
        len = c - 'a' + 10;
      }
      else if('A' <= c && c <= 'F'){
        len = c - 'A' + 10;
      }
    }
    else{
      if('0' <= c && c <= '9'){
        len = len * 16 + (c - '0');
      }
      else if('a' <= c && c <= 'f'){
        len = len * 16 + (c - 'a' + 10);
      }
      else if('A' <= c && c <= 'F'){
        len = len * 16 + (c - 'A' + 10);
      }
    }
  }
  return len;
};

namespace Http{
  using namespace toolkit;

  HttpSplitter::HttpSplitter(){
    _next_func = [this](const char* data, size_t length){
      return onHeader(data, length);
    };
  }

  void HttpSplitter::input(const void* data, size_t length){
    if(length <= 0)return;
    return _next_func((const char*)data, length);
  }

  void HttpSplitter::onHeader(const char* data, size_t length){

    if(length <= 4)
      return;
    constexpr const char* content_length = "Content-Length: ";
    constexpr size_t _content_length = 16;
    toolkit::string_view view((const char*)data, length);
    auto pos = view.find("\r\n\r\n");
    //这里记录一下head尾部
    auto header_end = pos;
    //如果没有找到分界点，那么缓存数据后返回
    if( pos == toolkit::string_view::npos ){
      header.append(data, length);
      return;
    }
    //这里找到分界点
    header.append(data, pos + 2);
    /*
     * 尝试找到body的类型, 这里分body和chunked body
     * */
    pos = view.find(content_length);
    need_length = 0;

    /* 这里是收到了content-length */
    if( pos != toolkit::string_view::npos ){
      _next_func = [this](const char* data_, size_t length_){
        return this->onBody(data_, length_);
      };
      /* 这里解析content-length */
      /*begin*/
      pos += 16;
      auto end = view.find("\r\n", pos);
      if( end == toolkit::string_view::npos){
        WarnL << "HTTP Content-Length 解析失败";
        onSplitterError();
        clear();
        return;
      }

      try{
        need_length = std::stol(std::string(data + pos, end - pos));
      }catch(const std::exception& e){
        WarnL << "HTTP Content-Length字段解析失败";
        onSplitterError();
        reset();
        return;
      }
      onRecvHeader(header, 1);
      return _next_func(data + header_end + 4, length - header_end - 4);
    }

    pos = view.find("Transfer-Encoding: chunked");
    /*  如果找到了chunked */
    if( pos != toolkit::string_view::npos ){
      _next_func = [this](const char* data_, size_t length_){
        return this->onChunkedBody(data_, length_);
      };
      onRecvHeader(header, 2);
      return _next_func(data + header_end + 4, length - header_end - 4);
    }

    onRecvHeader(header, 0);
    //调用之后清除上下文
    clear();
    /* 如果没有找到任何关于消息体的字段，那么继续接收头字段 */
    return _next_func(data + header_end + 4, length - header_end - 4);
  }

  void HttpSplitter::onBody(const char* data, size_t length){
    if(need_length > length){
      body.append(data, length);
      need_length -= length;
      return;
    }
    body.append(data, need_length);
    auto move_length = need_length;
    need_length = 0;
    _next_func = [this](const char* data_, size_t length_){
      return this->onHeader(data_, length_);
    };
    onRecvBody(body);
    /* 重置上下文 */
    clear();
    /* 重新接收头部字段 */
    return _next_func(data + move_length, length - move_length);
  }


  // H3.6.1 Chunked Transfer Coding
  // Chunked-Body		  = *chunk
  //					          last-chunk
  //					          trailer
  //					          CRLF
  //	chunk			      = chunk-size [ chunk-extension ] CRLF
  //					          chunk-data CRLF
  //	chunk-size		  = 1*HEX
  //	last-chunk		  = 1*("0") [ chunk-extension ] CRLF
  //	chunk-extension	= *( ";" chunk-ext-name [ "=" chunk-ext-val ] )
  //	chunk-ext-name	= token
  //	chunk-ext-val	  = token | quoted-string
  //	chunk-data		  = chunk-size(OCTET)
  //	trailer			    = *(entity-header CRLF)
  void HttpSplitter::onChunkedBody(const char* data, size_t length){
    if( length <= 0)return;
    toolkit::string_view view(data, length);
    try{
      auto length_end = view.find_first_of("\t ;\r\n");
      if(length_end == toolkit::string_view::npos){
        throw std::logic_error("没有找到结束符");
      }
      auto length_view = view.substr(0, length_end);
      need_length = getChunkedLength(length_view.data(), length_view.size()) + 2;
      if(need_length - 2 == 0){
        //说明读到了最后一个chunk
        _next_func = [this](const char* data_, size_t length_){
          this->onRecvChunkedBodyTailer();
          this->reset();
        };
      }
      else{
        _next_func = [this](const char* data_, size_t length_){
          return this->onChunkedBody_l(data_, length_);
        };
      }
      return _next_func(data, length);
    }catch(const std::exception& e){
      WarnL << "HTTP Chunked头部长度解析失败： " << e.what();
      onSplitterError();
      clear();
      return;
    }
  }

  void HttpSplitter::onChunkedBody_l(const char* data, size_t length){
    if( length <= 2)return;
    toolkit::string_view view(data, length);
    /*
     * 独占一行
     * */
    auto pos = view.find("\r\n");
    if( pos == toolkit::string_view::npos)return;
    pos += 2;
    _next_func = [this](const char* data_, size_t length_){
      return this->onChunkedBody_l2(data_, length_);
    };
    return _next_func(data + pos, length - pos);
  }
  // 处理data数据部分
  void HttpSplitter::onChunkedBody_l2(const char* data, size_t length){
    if(need_length > length){
      body.append(data, length);
      need_length -= length;
      return;
    }
    //此时数据已经读完
    body.append(data, need_length);
    auto move_length = need_length;
    need_length = 0;
    //检查末尾是否为"\r\n"
    if(body.find("\r\n") != body.size() - 2){
      WarnL << "HTTP Chunked body 内容长度不正确";
      onSplitterError();
      reset();
      return;
    }
    //清除尾巴
    body.erase(body.size() - 2);
    _next_func = [this](const char* data_, size_t length_){
      return this->onChunkedBody(data_, length_);
    };
    //先调用
    onRecvChunkedBody(body);
    return _next_func(data + move_length, length - move_length);
  }

  void HttpSplitter::onChunkedTailer(const char* data, size_t length){
    if( length < 4)return;
    toolkit::string_view view(data, length);
    auto pos = view.find("\r\n\r\n");
    if( pos == toolkit::string_view::npos)
      return;
    _next_func = [this](const char* data_, size_t length_){
      return this->onHeader(data_, length_);
    };
    //表明一个http请求数据读取完成
    onRecvChunkedBodyTailer();
    //重新返回读取头状态
    clear();
    return _next_func(data + pos + 4, length - pos - 4);
  }

  void HttpSplitter::clear(){
    header.clear();
    body.clear();
    need_length = 0;
  }

  void HttpSplitter::reset(){
    clear();
    _next_func = [this](const char* data, size_t length){
      return this->onHeader(data, length);
    };
  }

}