/*
 * @file_name: HttpResponse.hpp
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
#ifndef HTTP_RESPONSE_IMPL_HPP
#define HTTP_RESPONSE_IMPL_HPP
#include "HttpBase.hpp"
#include "HttpStatus.hpp"
#include <string>
namespace Http
{

  class HttpResponse: public HttpMessage
  {
    friend class HttpResponseInvoker;
  public:
    using Super = HttpMessage;
    using Ptr = std::shared_ptr<HttpResponse>;
  public:
    HttpResponse();
    unsigned int getStatus() const;

    template <typename T, typename = typename std::enable_if<Http::is_http_status<T>::value>::type>
    void setStatus(){
      status_code = (unsigned int)T::value;
      status_descrip = T::desciptor;
    }

    std::string toString() const;
  private:
    unsigned int status_code = Http::Status::ok::value;
    std::string status_descrip{Http::Status::ok::desciptor};
    std::weak_ptr<HttpSession> session_ptr;
  };
}; // namespace http
#endif