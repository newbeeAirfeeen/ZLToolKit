/*
* @file_name: HttpInterceptor.cpp
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
#include "HttpInterceptor.hpp"
#include "Util/logger.h"
using namespace toolkit;
namespace Http{
  void HttpInterceptor::setInvoke(const std::function<void(HTTP_INTERCEPTOR_ARGS)>& invoke){
    this-> http_invoke = invoke;
  }

  HttpInterceptor &HttpInterceptor::addIntercept(const std::function<bool(HTTP_INTERCEPTOR_CONST_ARGS)>& f){
    interceptors.push_back(f);
    return *this;
  }
  void HttpInterceptor::invoke(HTTP_INTERCEPTOR_ARGS){
    bool consumed = true;
    for(auto& interceptor : interceptors){
      consumed = interceptor(HTTP_INTERCEPTOR_INVOKE_VALES);
      if(!consumed){
        break;
      }
    }
    if(!consumed){
      InfoL << "请求被拦截器截断";
      return;
    }
    this->http_invoke(HTTP_INTERCEPTOR_INVOKE_VALES);
  }

}