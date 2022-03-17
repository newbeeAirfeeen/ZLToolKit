/*
* @file_name: HttpResponse.cpp
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
#include "HttpResponse.hpp"
#include "impl/rfc822-datetime.hpp"
namespace Http{
  HttpResponse::HttpResponse(){
    setHeader("Server", HTTP_SERVER);
    setHeader("Date", rfc822_datetime_format());
  }

  unsigned int HttpResponse::getStatus() const{
    return status_code;
  }
  std::string HttpResponse::toString() const
  {
    // std::stringstream strs;
    // strs << "HTTP/1.1 " << getStatus() << " " << status_descrip << "\r\n";
    // //刷新一下body内容
    // getBody().flush();
    // auto body_str = std::move(getBody().toString());
    // if (body_type::need_content_length)
    // {
    //   strs << Http::HttpResponseHeader::content_length::value << ": " << body_str.size() << "\r\n";
    //   strs << Http::HttpResponseHeader::content_type::value << ": " << body_type::content_type << "\r\n";
    // }
    // strs << std::move(base_header_type::toString());
    // auto it = getHeader<Http::HttpResponseHeader::server>();
    // if (!it.size())
    //   strs << "Server: " << HTTP_SERVER << "\r\n";
    // it = getHeader<Http::HttpResponseHeader::date>();
    // if (!it.size())
    //   strs << "Date: " << rfc822_datetime_format(std::chrono::system_clock::now()) << "\r\n";
    // strs << "\r\n";
    // strs << body_str;
    return ""; // std::move(strs.str());
  }
};