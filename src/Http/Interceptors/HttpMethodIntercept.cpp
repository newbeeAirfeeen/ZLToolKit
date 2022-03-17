/*
* @file_name: HttpBasicInterceptor.cpp
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
#include "HttpMethodIntercept.hpp"
static const char* getMethodDescribe(Http::HttpMethod method){
  switch(method) {
    case Http::HttpMethod::GET:
      return "GET";
    case Http::HttpMethod::POST:
      return "POST";
    default:
      return "";
  }
}
namespace Http{
  HttpMethodIntercept::HttpMethodIntercept(const HttpMethod& m):_method(m), _method_describe(getMethodDescribe(_method)){}
  bool HttpMethodIntercept::operator()(const HttpRequest& request, HttpResponse& response){
    if(request.getMethod() == _method_describe)
      return true;
    response.setStatus<Http::Status::method_not_allowed>();
    return false;
  }
};
