/*
 * @file_name: http_session.hpp
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
#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP
#include "HttpInterceptor.hpp"
#include "HttpReciever.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestInvoker.hpp"
#include "HttpResponseInvoker.hpp"
#include <Network/Buffer.h>
#include <Network/TcpSession.h>
#include <Util/logger.h>
#include <functional>
#include <map>
namespace Http {

class HttpRequestHandler{
public:
    template<typename RequestBodyType, typename ResponseBodyType>
    using HttpCallback = std::function<void(const typename HttpRequest<RequestBodyType>::Ptr&,
                                                  typename HttpResponse<ResponseBodyType>::Ptr&)>;
public:
    /*
    * @description: 注册http请求, 此函数需要在http服务器启动之前调用。不然
    *               会有线程安全问题
    * @date: 2022/3/1
    * @param path: 请求路径
    * @return:
    */
    template<typename RequestBodyType, typename ResponseBodyType>
    static HttpInterceptor::Ptr apiRegister(const char* path, const HttpCallback<RequestBodyType,
                                                                                        ResponseBodyType>& _func){
        using namespace toolkit;
        auto interceptor = std::make_shared<Http::HttpInterceptor>();
        interceptor->setInvoke([_func](HTTP_INTERCEPTOR_ARGS){
            HttpRequestInvoker request_invoker;
            HttpResponseInvoker response_invoker;
            auto request = request_invoker.template createHttpRequest<RequestBodyType>(HTTP_INTERCEPTOR_INVOKE_VALES);
            auto response = response_invoker.createHttpResponse<ResponseBodyType>(session);
            //由此调用http_handler回调
            _func(request, response);
            response_invoker.response<ResponseBodyType>(std::cref(response));
        });
        /* 添加基本的拦截器 */
        interceptor->addIntercept([](HTTP_INTERCEPTOR_ARGS){
            auto it = request_header_map.find(Http::HttpRequestHeader::content_type::value);
            if( it != request_header_map.end() && it->second != RequestBodyType::content_type){
              InfoL << "HTTP Content-Type 不正确";
              return false;
            }
            return true;
        });
        _http_handle_map_.emplace(path, interceptor);
        return interceptor;
    }

    static HttpInterceptor::Ptr getInvokes(const std::string& path);
private:
    static std::unordered_map<std::string, HttpInterceptor::Ptr> _http_handle_map_;
};


class HttpSession : public toolkit::TcpSession, public HttpReciever{
  friend HttpResponseInvoker;
public:
  using Buffer = toolkit::Buffer;
  using BufferLikeString = toolkit::BufferLikeString;
  using SockException = toolkit::SockException;
  using Ptr = std::shared_ptr<HttpSession>;
  using Socket = typename toolkit::Socket;
  using header_type = typename HttpReciever::header_type;
public:
  explicit HttpSession(const Socket::Ptr &sock);
  ~HttpSession() override;

protected:
  void sendHttp(const std::string& buf);
  /*
  * @description: 当收到真个http请求时
  * @date: 2022/3/1
  * @param header: 头部解析map
  * @param line: 请求行
  * @param header: 请求头
  * @param body: 请求体
  */
  void onRecvHttp(header_type&, toolkit::string_view line, std::string& header, std::string& body) override;
  /*
  * @description: 当收到chunked数据时
  * @date: 2022/3/1
  * @param header: 头部解析map
  * @param line: 请求行
  * @param header: 请求头
  * @param body: 请求体
  */
  void onRecvHttpChunked(header_type& , toolkit::string_view line, std::string& header, std::string& chunked_body, bool isEnd) override;
public:
  void onRecv(const Buffer::Ptr &buf) override;
  void onError(const SockException &err) override;
  void onManager() override {}
};
}; // namespace http

#endif