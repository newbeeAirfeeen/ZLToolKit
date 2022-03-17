/*
 * @file_name: HttpConst.cpp
 * @date: 2022/03/17
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
#include "HttpConst.hpp"
#include <mutex>
#include <map>
#include <string.h>
#include <Util/onceToken.h>
namespace Http
{
    const std::string &getHttpContentType(const char *name)
    {
        using namespace toolkit;
        const char *dot;
        dot = strrchr(name, '.');
        static std::multimap<std::string, std::string> mapType;

        static onceToken token([&]()
                               {
        for (unsigned int i = 0; i < sizeof(s_mime_src) / sizeof(s_mime_src[0]); ++i) {
            mapType.emplace(s_mime_src[i][0], s_mime_src[i][1]);
        } });
        static std::string defaultType = "text/plain";
        if (!dot)
        {
            return defaultType;
        }
        auto it = mapType.find(dot + 1);
        if (it == mapType.end())
        {
            return defaultType;
        }
        return it->second;
    }
};