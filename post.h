#pragma once
#include <string>
#include "date_time.h"
class post {
private:
	 int id;
	 std::string content;
	 int like_count;
	 std::string author;
	 date_time published_date_time;
	 static int post_count;
public:
	post(const int &_id, std::string & content,const int &_like_count, const std::string &_author, date_time &_published_date_time);
	post(const std::string& , const std::string &);
	static void set_count(const int&);
	int get_id();
	void print_post();
	void add_like_count();
	double get_time_passed();
	int get_like_count();
	static int get_post_count();
	std::string get_content();
	std::string get_author();
	date_time get_published_date_time();
};
