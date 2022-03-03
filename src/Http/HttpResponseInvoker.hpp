/*
* @file_name: HttpCommon.hpp
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
#ifndef SHTOOLKIT_HTTPRESPONSEINVOKER_HPP
#define SHTOOLKIT_HTTPRESPONSEINVOKER_HPP
#include "HttpCommon.hpp"
#include "HttpResponse.hpp"
#include <sstream>
#include <Util/logger.h>
namespace Http{
  class HttpResponseInvoker{
  public:
    using Ptr = std::shared_ptr<HttpResponseInvoker>;
  public:
    template<typename BodyType>
    typename HttpResponse<BodyType>::Ptr createHttpResponse(const std::weak_ptr<HttpSession>& session){
      auto response = std::make_shared<HttpResponse<BodyType>>();
      response->session_ptr = session;
      return std::move(response);
    }

    template<typename BodyType>
    void response(const typename HttpResponse<BodyType>::Ptr& response_ptr){
      using namespace toolkit;
      if(!response_ptr)return;
      auto& response = *response_ptr;
      auto session_self = response.session_ptr.lock();
      if(!session_self)
        return;
      auto str = std::move(response.toString());
      InfoL << str;
      session_self->sendHttp(str);
    }
  };
}
#endif // SHTOOLKIT_HTTPRESPONSEINVOKER_HPP
