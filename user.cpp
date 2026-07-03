#include "user.h"
#include "util.h"
#include <iostream>
user::user(const std::string &_username, const std::string &_password, const int &_follower_count) :post_ids_u_set(11,1) ,liked_posts(11,1){
	if (split_line(_username).size() != 1)
		throw std::invalid_argument("user object can't be initialized (invalid username)");
	if (split_line(_password).size() != 1)
		throw std::invalid_argument("user object can't be initialized (invalid password)");
	if (_follower_count < 0)
		throw std::invalid_argument("user object can't be initialized (invalid follower_count)");
	username = _username;
	password = _password,
	follower_count = _follower_count;		
}
bool user::compare_password(const std::string &input_pass)
{
	if (input_pass == password)
		return true;
	else return false;
}
char user::add_following(const std::string& _username )
{
	char state;
	auto is_following = followings_u_set.find(_username);

	if (username == _username)
		state = 'A';
		
	else if (is_following==followings_u_set.end())
	{
		state = 'C';
		followings_u_set.insert(_username);
	}

	else 
	{
		state = 'B';
	}
	return state;
}
char user::remove_following(const std::string& _username)
{
	char state;
	auto is_following = followings_u_set.find(_username);

	if (username == _username)
		state = 'A';

	else if (is_following == followings_u_set.end())
	{
		state = 'B';
	}
	else
	{
		followings_u_set.erase(_username);
		state = 'C';
	}
	return state;
}
void user::add_post(const int& _id)
{
	post_ids_u_set.insert(_id);
}
std::string user::get_username()
{
	return username;
}
bool user::add_liked_post(const int& id)
{
	auto it = liked_posts.find(id);
	if (it != liked_posts.end())
		return false;

	else {
		liked_posts.insert(id);
		return true;
	}
}
 hash_set<std::string>& user::get_followings() {
	return followings_u_set;
}
 hash_set<int>& user::get_post_ids() {
	 return post_ids_u_set;
 }
 int user::get_follower_count()
 {
	 return follower_count;
 }
 hash_set<int>& user::get_liked_post_ids()
 {
	 return liked_posts;
 }
 std::string user::get_password()
 {
	 return password;
 }
 void user::increase_follower_count()
 {
	 follower_count++;
 }
 void user::decrease_follower_count()
 {
	 follower_count--;
 }