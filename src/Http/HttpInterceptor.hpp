/*
* @file_name: HttpInterceptor.hpp
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
#ifndef SHTOOLKIT_HTTPINTERCEPTOR_HPP
#define SHTOOLKIT_HTTPINTERCEPTOR_HPP
#include <functional>
#include <memory>
#include <vector>
#include <Poller/EventPoller.h>
#include "Util/Url.hpp"
#include "HttpCommon.hpp"
#include "HttpReciever.hpp"
namespace Http{
#define HTTP_INTERCEPTOR_ARGS typename HttpReciever::header_type& request_header_map,\
                              std::string& request_method, \
                              Url& request_path, \
                              std::string& request_header,\
                              std::string& request_body,                             \
                              const std::shared_ptr<HttpSession>& session
#define HTTP_INTERCEPTOR_CONST_ARGS const typename HttpReciever::header_type& request_header_map,\
                                    const std::string& request_method, \
                                    const Url& request_path, \
                                    const std::string& request_header,\
                                    const std::string& request_body, \
                                    const std::shared_ptr<HttpSession>& session
//#define HTTP_INTERCEPTOR_VALUES request_header_map, request_method, request_path, request_header, request_body
#define HTTP_INTERCEPTOR_INVOKE_VALES request_header_map, request_method, request_path, request_header, request_body, session

  /*
  * 此类为了方便HttpRequestHandler调用
  * */
  class HttpInterceptor {
  public:
    using Ptr = std::shared_ptr<HttpInterceptor>;
  public:
    ~HttpInterceptor() = default;
    void setInvoke(const std::function<void(HTTP_INTERCEPTOR_ARGS)>& invoke);
    /*
    * @description: 增加拦截器，线程不安全
    * @date: 2022/3/1
    */
    HttpInterceptor & addIntercept(const std::function<bool(HTTP_INTERCEPTOR_CONST_ARGS)>&);
    /*
    * @description: 调用对应的http回调
    * @date: 2022/3/1
    */
    void invoke(HTTP_INTERCEPTOR_ARGS);
  private:
    std::function<void(HTTP_INTERCEPTOR_ARGS)> http_invoke;
    std::vector<std::function<bool(HTTP_INTERCEPTOR_CONST_ARGS)>> interceptors;
  };
}



#endif // SHTOOLKIT_HTTPINTERCEPTOR_HPP
