/*
* @file_name: HttpReciever.cpp
* @date: 2022/02/28
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
#include "HttpReciever.hpp"

namespace Http{
  void HttpReciever::input(const char* data, size_t length){
    return HttpSplitter::input(data, length);
  }

  void HttpReciever::onHeader(std::string& head){
    using namespace toolkit;
    this->header = std::move(head);
    toolkit::string_view data(this->header);
    using size_type = typename toolkit::string_view::size_type;
    size_type begin = 0;
    size_type end = toolkit::string_view::npos;
    while((end = data.find("\r\n", begin)) != toolkit::string_view::npos){
      /// world\r\n
      if(!begin){
        request_line = data.substr(begin, end);
        begin = end + 2;
        continue;
      }
      auto line = data.substr(begin, end - begin);
      auto pos = line.find(": ");
      if(pos == toolkit::string_view::npos){
        ErrorL << "Http 头部字段不符合标准";
        return onSplitterError();
      }
      auto _key = line.substr(0, pos);
      auto _val = line.substr(pos + 2);
      std::string key(_key.data(), _key.size());
      std::string val(_val.data(), _val.size());
      _header_map.emplace(std::move(key), std::move(val));
      begin = end + 2;
    }
  }

  void HttpReciever::onHttpMessage(){
    onRecvHttp(std::ref(_header_map), request_line,std::ref(header), std::ref(body));
    this->reset();
  }

  void HttpReciever::onRecvHeader(std::string& header, int body_type){
    onHeader(header);
    if(!body_type)
      return onHttpMessage();
  }

  void HttpReciever::onRecvBody(std::string& body){
    this->body = std::move(body);
    return onHttpMessage();
  }

  void HttpReciever::onRecvChunkedBody(std::string& _body){
    this->body = std::move(_body);
    onRecvHttpChunked(_header_map, request_line, header, this->body, false);
  }

  void HttpReciever::onRecvChunkedBodyTailer(){
    this->body.clear();
    onRecvHttpChunked(_header_map, request_line, header, body, true);
    reset();
  }

  void HttpReciever::onSplitterError(){
    HttpSplitter::reset();
  }

  void HttpReciever::reset(){
    header.clear();
    body.clear();
    _header_map.clear();
  }

};