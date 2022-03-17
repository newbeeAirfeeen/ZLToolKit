/*
* @file_name: HttpRequestHandler.hpp
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
#ifndef SHTOOLKIT_HTTPREQUESTHANDLER_HPP
#define SHTOOLKIT_HTTPREQUESTHANDLER_HPP
#include "HttpInterceptor.hpp"
#include <functional>
namespace Http{
  class HttpRequestHandler{
  public:
    using HttpCallback = std::function<void(const HttpRequest&, HttpResponse&)>;
  public:
    static const HttpInterceptor::Ptr& apiRegister(const std::string& path, const HttpCallback& f);
    static const HttpInterceptor::Ptr& apiRegister(const std::string& path, const HttpMethod&  method, const HttpCallback& f);
  /*
    * @description: 注册http请求, 此函数需要在http服务器启动之前调用。不然
    *               会有线程安全问题
    * @date: 2022/3/1
    * @param path: 请求路径
    * @return:
   */
    static const HttpInterceptor::Ptr& getInvokes(const std::string& path);

  private:
    static std::unordered_map<std::string, HttpInterceptor::Ptr> _http_handle_map_;
  };
};
#endif // SHTOOLKIT_HTTPREQUESTHANDLER_HPP
