#pragma once
#include <string>
#include "hash_set.h"
class user {
private:
	std::string username;
	std::string password;
	hash_set<int> post_ids_u_set ;
	hash_set<std::string> followings_u_set;
	hash_set<int> liked_posts;
	int follower_count;
public:
	user(const std::string & _username, const std::string& _password, const int& _follower_count);

	bool compare_password(const std::string& input_pass);
	char add_following(const std::string&  );
	void add_post(const int& id);
	char remove_following(const std::string&);
	bool add_liked_post(const int& );
	std::string get_username();
	hash_set<std::string>& get_followings();
	hash_set<int>& get_post_ids();
	int get_follower_count();
	std::string get_password();
	hash_set<int>& get_liked_post_ids();
	void increase_follower_count();
	void decrease_follower_count();
};