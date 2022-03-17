/*
 * @file_name: HttpBase.hpp
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
#ifndef HTTP_IMPL_HPP
#define HTTP_IMPL_HPP
#include <map>
#include <memory>
#include <string>
#include "HttpHeaderBase.hpp"
#include "Body/HttpBody.hpp"
namespace Http
{
  class HttpMessage
  {
  public:
    using HeaderType = HttpHeader;
    using field_type = typename HeaderType::field_type;
    using value_type = typename HeaderType::value_type;

  public:
    HttpMessage() = default;
    virtual ~HttpMessage() = default;
    HttpMessage(const HttpMessage &) = delete;
    HttpMessage &operator=(const HttpMessage &) = delete;
    HttpMessage(HttpMessage &&other) noexcept : _header_(std::move(other._header_)) {}

    const value_type &getHeader(const field_type &field) const;
    /*
    * @description: 设置http头
    * @date: 2022/3/14
    * @param:  field: 头部字段
    *          key: val值
    * @return:
    */
    void setHeader(const field_type &field, const value_type &val);
    /*
    * @description: 设置是否保持连接
    * @date: 2022/3/14
    */
    void setConnection(bool keep_alive);
    /*
     * @description: 是否是持久连接
     * @date: 2022/3/2
     * @return: true为持久连接，false为短连接
     */
    bool getConnection() const;
    /*
    * @description: 得到消息的类型
    * @date: 2022/3/14
    */
    const char* getContentType() const;
    /*
    * @description: 设置http体
    * @date: 2022/3/14
    * @param:
    */
    void setBody(const HttpBody::Ptr& body);
    /*
    * @description: 得到body
    * @date: 2022/3/14
    * @return: body指针
    */
    const HttpBody::Ptr& getBody() const;
    /*
    * @description: 得到全部头
    * @date: 2022/3/17
    * @return:headers
    */
    const HeaderType& getConstHeaders() const;
  protected:
    HeaderType &getHeaders();
  private:
    HeaderType _header_;
    HttpBody::Ptr body_ptr;
  };
}; // namespace http
#endif