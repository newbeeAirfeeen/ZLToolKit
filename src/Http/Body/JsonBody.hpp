/*
 * @file_name: string_body.hpp
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
#ifndef HTTP_JSON_BODY_HPP
#define HTTP_JSON_BODY_HPP
#include <jsoncpp/json.h>
#include <memory>
namespace Http {

  class JsonParser {
  public:
    using value_type = typename Json::Value;
  public:
    static void serialize(const value_type &val, std::string &target) {
      Json::StreamWriterBuilder builder;
      target = std::move(Json::writeString(builder, val));
    }
    static void from_string(const std::string &source, value_type &val) {
      Json::CharReaderBuilder read_builder;
      std::string err;
      std::unique_ptr<Json::CharReader> reader(read_builder.newCharReader());
      if (!reader)
        throw std::bad_alloc();
      if (!reader->parse(source.data(), source.data() + source.size(), &val,
                         &err)) {
        throw std::logic_error(err);
      }
    }
  };

  class JsonBody{
  public:
    static constexpr const char *content_type = "application/json";
    static constexpr bool need_content_length = true;
    using parser_type = JsonParser;
    using value_type = typename JsonParser::value_type;
    using size_type = size_t;

  public:
    JsonBody() = default;
    JsonBody(const JsonBody &) = delete;
    JsonBody &operator=(const JsonBody &) = delete;

    std::string toString() const{
      if(value.empty())
        return "";
      return val;
    }

    void loadFromString(std::string& val){
      this->val = std::move(val);
      parser_type::from_string(this->val, value);
    }

    template<typename T>
    JsonBody& set(const char* field, const T& _v){
      value[field] = _v;
      return *this;
    }

    void flush() const{
      parser_type::serialize(value, val);
    }

    inline size_t size() const {
        return val.size();
    }
  public:
    bool empty() const { return value.size() == 0; }
  private:
    mutable std::string val;
    value_type value;
  };
}; // namespace http

#endif