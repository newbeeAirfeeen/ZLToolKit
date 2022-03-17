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
#include "HttpReciever.hpp"
#include "HttpCommon.hpp"
#include <Network/Buffer.h>
#include <Network/TcpSession.h>
#include <functional>
#include <map>
namespace Http {
class HttpSession : public toolkit::TcpSession, public HttpReciever{
  friend HttpResponseInvoker;
public:
  using Buffer = toolkit::Buffer;
  using SockException = toolkit::SockException;
  using Ptr = std::shared_ptr<HttpSession>;
  using Socket = typename toolkit::Socket;
  using header_type = typename HttpReciever::header_type;
public:
  explicit HttpSession(const Socket::Ptr &sock);
  ~HttpSession() override;

protected:
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

  const Socket::Ptr& getSock() const;
public:
  void onRecv(const Buffer::Ptr &buf) override;
  void onError(const SockException &err) override;
  void onManager() override {}
};
}; // namespace http

#endif