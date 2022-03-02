/*
 * @file_name: http_response_impl.hpp
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
#ifndef HTTP_RESPONSE_IMPL_HPP
#define HTTP_RESPONSE_IMPL_HPP
#include "HeaderFields.hpp"
#include "HttpBase.hpp"
#include "HttpStatus.hpp"
#include "HttpReciever.hpp"
#include <string>
#include "impl/rfc822-datetime.hpp"
namespace Http {

  template <typename _body_type>
  class basic_message<false, HttpResponseHeader, _body_type> : protected http_message_base<HttpResponseHeader, _body_type>{
    friend class HttpResponseInvoker;
  public:
    using base_header_type = http_message_base<HttpResponseHeader, _body_type>;
    using header_type = typename base_header_type::header_type;
    using body_type = typename base_header_type::body_type;
    using value_type = typename base_header_type::value_type;
    using this_type  = basic_message<false, HttpResponseHeader, body_type>;
    using Ptr        = std::shared_ptr<this_type>;
  public:
    template<typename T, typename = typename std::enable_if<Http::is_response_field<T>::value>::type>
    const value_type& getHeader() const{
      return base_header_type::getHeader(T::value);
    }

    template<typename T, typename R, typename = typename std::enable_if<Http::is_response_field<T>::value>::type>
    void setHeader(const R& val){
      return base_header_type::setHeader(T::value, std::move(std::to_string(val)));
    }

    template<typename T, typename = typename std::enable_if<Http::is_response_field<T>::value>::type>
    void setHeader(const char* val){
        return base_header_type::set_header(T::value, val);
    }

    template<typename T, typename = typename std::enable_if<Http::is_request_field<T>::value>::type>
    void setHeader(const std::string& val){
      return base_header_type::setHeader(T::value, val);
    }

    unsigned int getStatus() const {
        return status_code;
    }

    template<typename T, typename = typename std::enable_if<Http::is_http_status<T>::value>::type>
    void setStatus(){
        status_code = (unsigned int)T::value;
        status_descrip = T::desciptor;
    }

    const body_type& getBody() const {
      return base_header_type::getBody_l();
    }

    void setBody(const body_type& body){
      return base_header_type::setBody_l(body);
    }

    body_type& body() {
      return base_header_type::body_l();
    }

    std::weak_ptr<HttpSession> getHttpSession() const{
      return session_ptr;
    }

    std::string toString() const {
      std::stringstream strs;
      strs << "HTTP/1.1 " << getStatus() << " " << status_descrip << "\r\n";
      //刷新一下body内容
      getBody().flush();
      auto body_str = std::move(getBody().toString());
      if(body_type::need_content_length){
        strs << Http::HttpResponseHeader::content_length::value << ": " << body_str.size() << "\r\n";
        strs << Http::HttpResponseHeader::content_type::value   << ": " << body_type::content_type << "\r\n";
      }
      strs << std::move(base_header_type::toString());
      auto it = getHeader<Http::HttpResponseHeader::server>();
      if(!it.size()) strs << "Server: " << HTTP_SERVER << "\r\n";
      it = getHeader<Http::HttpResponseHeader::date>();
      if(!it.size()) strs << "Date: " << rfc822_datetime_format(std::chrono::system_clock::now()) << "\r\n";
      strs << "\r\n";
      strs << body_str;
      return std::move(strs.str());
    }
  public:
    bool isHttpRequest() const override{
      return false;
    }

    typename base_header_type::Ptr clone() const override{
      return nullptr;
    }
   private:
      unsigned int status_code = Http::Status::ok::value;
      std::string  status_descrip{Http::Status::ok::desciptor};
      std::weak_ptr<HttpSession> session_ptr;
  };
  template <typename T> using HttpResponse = basic_message<false, HttpResponseHeader, T>;
}; // namespace http
#endif