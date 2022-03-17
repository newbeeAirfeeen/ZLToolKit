/*
 * @file_name: HttpResponseInvoker.cpp
 * @date: 2022/03/01
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
#include "HttpResponseInvoker.hpp"
#include "Util/logger.h"
#include "HttpSession.hpp"
#include <Network/Buffer.h>
#include <sstream>
using namespace toolkit;
namespace Http
{
  HttpResponse::Ptr HttpResponseInvoker::createHttpResponse(const std::shared_ptr<HttpSession> &session_ptr)
  {
    HttpResponse::Ptr response = std::make_shared<HttpResponse>();
    response->session_ptr = session_ptr;
    return response;
  }

  void HttpResponseInvoker::response(const HttpResponse::Ptr &response)
  {
    if (!response){
      return;
    }

    auto session_stronger = response->session_ptr.lock();
    if (!session_stronger){
      return;
    }
    std::stringstream str;
    str << "HTTP/1.1 " << response->status_code << " " << response->status_descrip << "\r\n";
    auto buf_ptr = std::make_shared<BufferLikeString>(str.str());
    const HttpHeader& headers = response->getConstHeaders();
    const auto& _sock = session_stronger->getSock();
    _sock->send(buf_ptr, nullptr, 0, false);
    _sock->send(headers.toString(), nullptr, 0, false);
    const auto& body_ptr = response->getBody();
    if(!body_ptr){
      _sock->send("\r\n", 2, nullptr, 0, true);
      return;
    }
    std::stringstream str_;
    str_ << "Content-Type: " << body_ptr->getContentType() << "\r\n";
    str_ << "Content-Length: " << body_ptr->getContentLength() << "\r\n\r\n";
    _sock->send(str_.str(), nullptr, 0, true);
    body_ptr->sendHttpBody(_sock);
  }
};