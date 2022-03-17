/*
 * @file_name: HttpBase.cpp
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
#include "HttpBase.hpp"
namespace Http
{
  using field_type = typename HttpMessage::field_type;
  using value_type = typename HttpMessage::value_type;

  const value_type &HttpMessage::getHeader(const field_type &field) const
  {
    static value_type null_header;
    auto it = _header_.find(field);
    if (it != _header_.end())
      return (*it).second;
    return null_header;
  }

  void HttpMessage::setHeader(const field_type &field, const value_type &val)
  {
    _header_[field] = val;
  }

  void HttpMessage::setConnection(bool keep_alive)
  {
    if (keep_alive)
      return setHeader("Connection", "Keep-Alive");
    return setHeader("Connection", "close");
  }

  bool HttpMessage::getConnection() const
  {
    const auto &it = getHeader("Connection");
    if (it == "Keep-Alive")
      return true;
    return false;
  }

  const char *HttpMessage::getContentType() const
  {
    if (body_ptr)
    {
      return body_ptr->getContentType();
    }
    return getHeader("Content-Type").data();
  }

  void HttpMessage::setBody(const HttpBody::Ptr &body)
  {
    body_ptr = body;
  }

  const typename HttpBody::Ptr &HttpMessage::getBody() const
  {
    return body_ptr;
  }
  const typename HttpMessage::HeaderType &HttpMessage::getConstHeaders() const{
    return _header_;
  }
  HttpMessage::HeaderType &HttpMessage::getHeaders(){
    return _header_;
  }

};