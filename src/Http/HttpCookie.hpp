/*
* @file_name: HttpCookie.hpp
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
#ifndef SHTOOLKIT_HTTPCOOKIE_HPP
#define SHTOOLKIT_HTTPCOOKIE_HPP
#include <memory>
#include <Util/string_view.hpp>
#include <unordered_map>
/*
 * RFC 6265 HTTP State Management Mechanism
 * document defines the HTTP Cookie and Set-Cookie header fields.
 * When
 * the user agent makes subsequent requests to the server, the user
 * agent uses the metadata and other information to determine whether to
 * return the name/value pairs in the Cookie header.
 *
 * Set-Cookie
 * set-cookie-header = "Set-Cookie:" SP set-cookie-string
 * set-cookie-string = cookie-pair *( ";" SP cookie-av )
 *    cookie-pair = cookie-name "=" cookie-value
 *      cookie-name = token
 *      cookie-value = *cookie-octet / ( DQUOTE *cookie-octet DQUOTE )
 *        cookie-octet = %x21 / %x23-2B / %x2D-3A / %x3C-5B / %x5D-7E
 *                       ; US-ASCII characters excluding CTLs,
 *                       ; whitespace DQUOTE, comma, semicolon,
 *                       ; and backslash
 *        token = <token, defined in [RFC2616], Section 2.2>
 *        cookie-av = expires-av / max-age-av / domain-av / path-av / secure-av / httponly-av / extension-av
 *        expires-av = "Expires=" sane-cookie-date
 *        sane-cookie-date = <rfc1123-date, defined in [RFC2616], Section 3.3.1>
 *        max-age-av = "Max-Age=" non-zero-digit *DIGIT
 *                      ; In practice, both expires-av and max-age-av
 *                      ; are limited to dates representable by the
 *                      ; user agent.non-zero-digit = %x31-39; digits 1 through 9
 *        domain-av = "Domain=" domain-value
 *        domain-value = <subdomain>; defined in [RFC1034], Section 3.5, as; enhanced by [RFC1123], Section 2.1
 *        path-av = "Path=" path-value
 *        path-value = <any CHAR except CTLs or ";">
 *        secure-av = "Secure"
 *        httponly-av = "HttpOnly"
 *        extension-av = <any CHAR except CTLs or ";">
 * */
namespace Http{

  class HttpCookie{
  public:
    using Ptr = std::shared_ptr<HttpCookie>;
    using value_type = typename toolkit::string_view;
  public:
    /*
    * @description: 取cookie属性
    * @date: 2022/3/2
    * @param: key name
    * @return value
    */
    const value_type& getAttribute(const std::string&) const;
    /*
    * @description: 设置一个key-value
    * @date: 2022/3/2
    * @param key:键
    * @param value: 值
    */
    void setAttribute(const std::string& key, const std::string& value);

    /*
     * The Expires attribute indicates the maximum lifetime of the cookie.
     * rfc1123-date
     * */
    const value_type& getExpires() const;
    /*
    * @description: 设置过期时间
    * @date: 2022/3/2
    * @param hour:几小时后
    */
    void setExpires(size_t hours);

  private:
    std::string raw_content;
    std::unordered_map<value_type, value_type> tokens_map;
  };
}
#endif // SHTOOLKIT_HTTPCOOKIE_HPP
