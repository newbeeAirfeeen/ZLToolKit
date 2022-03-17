/*
 * @file_name: HttpClient.hpp
 * @date: 2022/03/02
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
#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP
#include <Network/TcpClient.h>
#include <functional>
#include "HttpReciever.hpp"
#include <memory>
#include <functional>
namespace Http{
  class HttpClient : public toolkit::TcpClient, protected HttpReciever{
  public:
    using Ptr = std::shared_ptr<HttpClient>;
    using SockException = toolkit::SockException;
    using Buffer = toolkit::Buffer;
    using EventPoller = toolkit::EventPoller;
    using Super = toolkit::TcpClient;
  public:
    explicit HttpClient(const std::string& host, size_t time_out = 10, const EventPoller::Ptr &poller = nullptr);
    /*
    * @description: 设置http请求超时时间,在请求开始前调用
    * @date: 2022/3/2
    * @param:  单位为秒
    */
    void setTimeout(size_t time_out);
    /*
    * @description: 描述
    * @date: 2022/3/2
    * @param: 远程主机ipv4
    */
    void setHost(const std::string&);

  protected:
    void onRecvHttp(header_type&, toolkit::string_view line, std::string& header, std::string& body) override;
    void onRecvHttpChunked(header_type& , toolkit::string_view line, std::string& header, std::string& chunked_body, bool isEnd) override;
  protected:
    void onConnect(const SockException &ex) override;
    void onRecv(const Buffer::Ptr &buf) override;
    void onErr(const SockException &ex) override;
  private:
    void onCheckHost(const std::string&);
  private:
    std::string raw_host;
    std::string host;
    size_t _time_out = 10;
  };
};

#endif // VIDEO_ON_DEMAND_HTTP_CLIENT_HPP
