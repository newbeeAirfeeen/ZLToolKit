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
    void input(const char* data, size_t length);
    /*
    * @description: 当收到整个http请求
    * @date: 2022/3/1
    * @param:
    * @return:
    */
    virtual void onRecvHttp(header_type&, toolkit::string_view line, std::string& header, std::string& body) = 0;
    virtual void onRecvHttpChunked(header_type& , toolkit::string_view line, std::string& header, std::string& chunked_body, bool isEnd) = 0;
  private:
    void onHeader(std::string& head);
    void onHttpMessage();
    void onRecvHeader(std::string& header, int body_type) override;
    void onRecvBody(std::string& body)override;
    void onRecvChunkedBody(std::string& _body) override;
    void onRecvChunkedBodyTailer() override;
    void onSplitterError() override;
    void reset();
  private:
    std::string header;
    std::string body;
    toolkit::string_view request_line;
    header_type _header_map;
  };


}



#endif // SHTOOLKIT_HTTPRECIEVER_HPP
