/*
 * @file_name: http_session.cpp
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
 * SOFTWARE.x
 */
#include <Util/logger.h>
#include "HttpSession.hpp"
#include "HttpReciever.hpp"
#include <utility>
#include <vector>
using namespace toolkit;
namespace Http {

    std::unordered_map<std::string, HttpInterceptor::Ptr> HttpRequestHandler::_http_handle_map_;
    HttpInterceptor::Ptr HttpRequestHandler::getInvokes(const std::string& path){
      auto it = _http_handle_map_.find(path);
      if(it != _http_handle_map_.end()){
        return it->second;
      }
      return nullptr;
    }

    HttpSession::HttpSession(const Socket::Ptr &sock)
        : toolkit::TcpSession(sock) {

    }

    HttpSession::~HttpSession(){
        InfoL << "~HttpSession";
    }

    void HttpSession::onRecv(const Buffer::Ptr &buf) {
        try {
            //输入分包器
            HttpReciever::input(buf->data(), buf->size());
        } catch (const std::exception &e) {
            InfoL << e.what();
            this->shutdown();
        }
    }
    void HttpSession::onError(const SockException &err) { ErrorL << err.what(); }

    void HttpSession::sendHttp(const std::string& buf){
      SocketHelper::send(std::make_shared<BufferLikeString>(buf));
    }

    void HttpSession::onRecvHttp(header_type& header_map, toolkit::string_view line, std::string& header, std::string& body){
      auto vec = line.split(" ");

      if(vec.size() != 3){
        ErrorL << "HTTP请求格式错误,不符合RFC2616规范";
        return;
      }
      std::string method(vec[0].data(), vec[0].size());
      std::string uri;
      Url url;
      {
        if(!vec[1].begin_with("http://") && !vec[1].begin_with("HTTP://")){
          uri.append("http://", 7);
        }
        //uri.append()
        auto it = header_map.find(Http::HttpRequestHeader::host::value);
        if( it != header_map.end()) uri.append(it->second.data(), it->second.size());
        else uri.append("unknown");
        uri.append(vec[1].data(), vec[1].end());
        std::string version(vec[2].data(), vec[2].size());
        url.fromString(uri);
      }
      auto interceptor = HttpRequestHandler::getInvokes(url.getPath());
      auto self = std::static_pointer_cast<HttpSession>(shared_from_this());
      if(interceptor)
        return interceptor->invoke(header_map, method, url, header, body, self);
      InfoL << "未注册的path: " << url.getPath();
    }

    void HttpSession::onRecvHttpChunked(header_type& , toolkit::string_view line, std::string& header, std::string& chunked_body, bool isEnd){
      WarnL << chunked_body.size();
    }
}