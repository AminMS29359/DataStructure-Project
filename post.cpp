#include <stdexcept>
#include <iostream>
#include "post.h"
#include "util.h"
post::post(const int &_id ,std::string &_content, const int& _like_count, const std::string& _author,date_time &_published_date_time) :published_date_time(_published_date_time)
{
	if (_id < 1 )
		throw std::invalid_argument("post object can't be initialized (invalid id)");
	if(split_line(_content).size() < 1)
		throw std::invalid_argument("post object can't be initialized (invalid content)");
	if(_like_count < 0)
		throw std::invalid_argument("post object can't be initialized (invalid like_count)");
	if(split_line(_author).size() != 1)
		throw std::invalid_argument("post object can't be initialized (invalid author)");
	id = _id;
	content = _content;
	like_count = _like_count;
	author = _author;
}
post::post(const std::string& _content, const std::string& _author)
{
	like_count = 0;
	author = _author;
	content = _content;
	id = ++post_count;
}
int post::get_id()
{
	return id;
}
void post::set_count(const int& _count)
{
	post_count = _count;
}
void post::print_post()
{
	std::cout << "id : " << id << std::endl
		<< "content : " <<"\"" << content << "\"" << std::endl
		<< "author : " << author << std::endl
		<< like_count << " like(s)" << std::endl;
		published_date_time.print_date_time();
		std::cout<<"---------------------------------"
		<<std::endl<<std::endl;
}
void post::add_like_count()
{
	++like_count;
}
int post::get_like_count()
{
	return like_count;
}
double post::get_time_passed()
{
	return published_date_time.seconds_passed();
}
int post::get_post_count()
{
	return post_count;
}
std::string post::get_author()
{
	return author;
}
std::string post::get_content()
{
	return content;
}
date_time post::get_published_date_time()
{
	return published_date_time;
}
int post::post_count = 0;