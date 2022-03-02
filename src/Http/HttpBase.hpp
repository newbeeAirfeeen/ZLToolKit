/*
 * @file_name: http_base.hpp
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
namespace Http {

  template <bool, typename T, typename R> class basic_message;
  template <typename _header_type, typename _body_type> class http_message_base {
  public:
    using header_type = _header_type;
    using field_type = typename header_type::field_type;
    using value_type = typename header_type::value_type;
    using body_type = _body_type;
    using Ptr = std::shared_ptr<http_message_base<header_type, body_type>>;
  public:
    http_message_base() = default;
    http_message_base(const http_message_base<header_type, body_type> &) = delete;
    http_message_base &operator=(const http_message_base<header_type, body_type>&) = delete;
    http_message_base(http_message_base<_header_type, body_type> &&other) noexcept: _header_(std::move(other.header)){}
  public:
    virtual bool isHttpRequest() const = 0;
    virtual Ptr  clone() const = 0;

    const value_type& getHeader(const field_type &field) const {
      static value_type null_header;
      auto it = _header_.find(field);
      if (it != _header_.end())
        return (*it).second;
      return null_header;
    }

    void setHeader(const field_type &field, const value_type &val) {
      _header_[field] = val;
    }

    const body_type& getBody_l() const {
      return _body;
    }

    void setBody_l(const body_type& body){
      this->_body = body;
    }

    body_type& body_l() {
      return _body;
    }

    void setConnection(bool keep_alive){
      if(keep_alive)
        return setHeader(header_type::connection, "Keep-Alive");
      return setHeader(header_type::connection::value, "close");
    }
    /*
    * @description: 是否是持久连接
    * @date: 2022/3/2
    * @return: true为持久连接，false为短连接
    */
    bool getConnection() const{
      const auto& it = getHeader(header_type::connection::value);
      if(it == "Keep-Alive")
        return true;
      return false;
    }

    inline std::string toString() const{
      return _header_.toString();
    }

  protected:
    void loadHeader(std::string& header, typename header_type::SuperType& _header){
      _header_.loadHeader(header, _header);
    }
  private:
    header_type _header_;
    body_type _body;
};
}; // namespace http
#endif