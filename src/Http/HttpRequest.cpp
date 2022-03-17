/*
 * @file_name: HttpRequest.cpp
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
#include "HttpRequest.hpp"
#include "Body/HttpBodyFactory.hpp"
namespace Http
{
  std::ostream &operator<<(std::ostream &os, const HttpRequest &other)
  {
    os << "\n"
       << other._method << " " << other._url.getPath() << " "
       << "HTTP/1.1\n";
    os << other.getConstHeaders().toString() << "\n";
    const auto &body = other.getBody();
    if (body)
      other.getBody()->print(os);
    return os;
  }

  const std::string &HttpRequest::getMethod() const
  {
    return _method;
  }
  void HttpRequest::setPath(const std::string &path)
  {
    if (path.empty() || path[0] != '/')
      throw std::logic_error("请求路径必须要以\"/\"开始");
    _url.setPath(path);
  }

  const std::string &HttpRequest::getRequestPath() const
  {
    return _url.getPath();
  }

  void HttpRequest::setParameter(const std::string &field, const char *val)
  {
    _url.setParameter(field, val);
  }

  void HttpRequest::setParameter(const std::string &field, const std::string &val)
  {
    _url.setParameter(field, val);
  }

  const std::string &HttpRequest::getParameter(const std::string &key) const
  {
    return _url.getParameter(key.c_str());
  }

  const std::string &HttpRequest::getParameter(const char *key) const
  {
    return _url.getParameter(key);
  }

  void HttpRequest::onHeaderBody(std::unordered_map<std::string, std::string> &_header, std::string &header, std::string &body)
  {
    getHeaders().loadHeader(_header);
    setBody(HttpBodyFactory::createHttpBody(getContentType()));
    getBody()->loadFromString(body);
  }

  /*
      std::string toString() const
      {
        std::string url_encode = _url.getPath();
        const auto &query_map = _url.getQueryParameters();
        if (isHttpMethod<Http::Method::get>(getMethod()))
        {
          if (url_encode[url_encode.size() - 1] == '/')
          {
            url_encode.erase(url_encode.size() - 1);
            if (url_encode.empty() && !query_map.empty())
            {
              throw std::logic_error("当有请求参数的时候,必须指定请求路径");
            }
          }
          url_encode += "?";
          for (const auto &query : query_map)
          {
            url_encode += query.first;
            url_encode += "=";
            url_encode += query.second;
            url_encode += "&";
          }
          url_encode.erase(url_encode.size() - 1);
        }
        std::string str = getMethod() + " " + url_encode + " HTTP/1.1\r\n";
        //刷新一下body内容
        getBody().flush();
        auto body_str = std::move(getBody().toString());
        if (body_type::need_content_length)
        {
          str += Http::HttpRequestHeader::content_length::value;
          str += ": ";
          str += std::to_string(body_str.size());
          str += "\r\n";
        }
        str += std::move(base_header_type::toString());
        str += "\r\n";
        str += body_str;
        return std::move(str);
      }*/
};