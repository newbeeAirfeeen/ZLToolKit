/*
* @file_name: HttpReciever.hpp
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
#ifndef SHTOOLKIT_HTTPRECIEVER_HPP
#define SHTOOLKIT_HTTPRECIEVER_HPP
#include "HttpSplitter.hpp"
#include "Util/string_view.hpp"
#include "Util/logger.h"
#include <unordered_map>
/**
 * 4.2 Message Headers
 * HTTP header fields, which include general-header(section 4.5), request-header(section 5.3)
 * response-header(section 6.2), and entity-header(section 7.1) fields, follow the same generic
 * format as that given in Section 3.1.
 * Each header field consists of a name followd by a colon(":) and the field value. Field names
 * are case-insensitive.
 * the field value MAY be preceded by any amount of LWS, though a single SP is preferred.Header
 * fields can be extended over multiple lines by preceding each extra line with at least one SP or HT. Applications
 * ought to follow “common form”, where one is known or indicated, when generating HTTP constructs, since there
 * might exist some implementations that fail to accept anything beyond the common forms.
 *
 * field-content does not include any leading or trailing LWS
 * It MUST be possible
 * to combine the multiple header fields into one “field-name: field-value” pair, without changing the
 * semantics of the message, by appending each subsequent field-value to the first, each separated by a comma
 */
namespace Http{

  class HttpReciever : protected HttpSplitter{
  public:
    HttpReciever() = default;
    ~HttpReciever() override = default;
  public:
    using header_type = std::unordered_map<std::string, std::string>;
  public:
    inline void input(const char* data, size_t length){
      return HttpSplitter::input(data, length);
    }
    /*
    * @description: 当收到整个http请求
    * @date: 2022/3/1
    * @param:
    * @return:
    */
    virtual void onRecvHttp(header_type&, toolkit::string_view line, std::string& header, std::string& body) = 0;
    virtual void onRecvHttpChunked(header_type& , toolkit::string_view line, std::string& header, std::string& chunked_body, bool isEnd) = 0;
  private:
    void onHeader(std::string& head){
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

    void onHttpMessage(){
      onRecvHttp(std::ref(_header_map), request_line,std::ref(header), std::ref(body));
      this->reset();
    }

  private:
    void onRecvHeader(std::string& header, int body_type) override{
      onHeader(header);
      if(!body_type)
        return onHttpMessage();
    }

    void onRecvBody(std::string& body) override{
      this->body = std::move(body);
      return onHttpMessage();
    }

    void onRecvChunkedBody(std::string& _body) override{
      this->body = std::move(_body);
      onRecvHttpChunked(_header_map, request_line, header, this->body, false);
    }

    void onRecvChunkedBodyTailer() override{
      this->body.clear();
      onRecvHttpChunked(_header_map, request_line, header, body, true);
      reset();
    }

    void onSplitterError() override{
      HttpSplitter::reset();
    }

    void reset(){
      header.clear();
      body.clear();
      _header_map.clear();
    }
  private:
    std::string header;
    std::string body;
    toolkit::string_view request_line;
    header_type _header_map;
  };


}



#endif // SHTOOLKIT_HTTPRECIEVER_HPP
