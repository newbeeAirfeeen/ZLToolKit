//
// Created by 沈昊 on 2022/3/2.
//
#include "rfc822-datetime.hpp"
std::string rfc822_datetime_format(const std::chrono::system_clock::time_point& point){
  int r = 0;
  time_t time = std::chrono::system_clock::to_time_t(point);
  struct tm *tm = gmtime(&time);
  char buff[256] = {0};
  r = snprintf(buff, sizeof(buff), "%s, %02d %s %04d %02d:%02d:%02d GMT",
               s_week[(unsigned int)tm->tm_wday % 7],
               tm->tm_mday,
               s_month[(unsigned int)tm->tm_mon % 12],
               tm->tm_year+1900,
               tm->tm_hour,
               tm->tm_min,
               tm->tm_sec);
  if( r <= 0 || r >= 30){
    throw std::logic_error("不符合rfc822规范");
  }
  return std::string(buff, r);
}