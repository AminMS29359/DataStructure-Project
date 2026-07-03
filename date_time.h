#pragma once
class date_time {
private:
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;

public:
	date_time(const int &_month, const int& _day, const int &_year, const int& _hour, const int &_minute, const int& _second);
	date_time();
	double seconds_passed();
	void print_date_time();
	int get_day();
	int get_month();
	int get_year();
	int get_hour();
	int get_minute();
	int get_second();
};
