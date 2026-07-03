#define _CRT_SECURE_NO_WARNINGS

#include <stdexcept> 
#include <chrono>
#include <ctime>
#include <iostream>
#include "date_time.h"

date_time::date_time()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time_t);

    year = local_tm.tm_year + 1900;
    month = local_tm.tm_mon + 1;    
    day = local_tm.tm_mday;
    hour = local_tm.tm_hour;
    minute = local_tm.tm_min;
    second = local_tm.tm_sec;

}
date_time::date_time(const int& _month, const int &_day, const int &_year, const int& _hour, const int &_minute, const int &_second) {
	if (_day < 1 ||_day > 31 || _month < 1 ||_month>12 || _year < 1)
		throw std::invalid_argument("date_time object can't be initialized (invalid date)");
	if (_hour < 0 || _hour > 23|| _minute < 0 || _minute >59 || _second < 0 || _second >59)
		throw std::invalid_argument("date_time object can't be initialized (invalid date)");
	day = _day;
	month = _month;
	year = _year;
	hour = _hour;
	minute = _minute;
	second = _second;
}
double date_time::seconds_passed(){
    std::tm t = { 0 };
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    std::time_t given_time = std::mktime(&t);
    std::time_t now = std::time(nullptr);
    double diff = std::difftime(now, given_time);

    return diff;
}
void date_time::print_date_time()
{
    std::cout <<"date : " << month << "/" << day << "/" << year << std::endl
        <<"time : " << hour << ":" << minute << ":" << second<<std::endl;
}
int date_time::get_day()
{
    return day;
}
int date_time::get_month()
{
    return month;
}
int date_time::get_year()
{
    return year;
}
int date_time::get_hour()
{
    return hour;
}
int date_time::get_minute()
{
    return minute;
}
int date_time::get_second()
{
    return second;
}
