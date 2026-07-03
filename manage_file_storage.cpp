#include <iostream>
#include <fstream>
#include "manage_file_storage.h"
#include "hash_set.h"
#include "app_singleton.h"
#include "json.hpp"
#include "util.h"
#include "post.h"
#include "user.h"

using json = nlohmann::ordered_json;
void manage_file_storage::load_users_json_file()
{
	app_singleton& app = app_singleton::get_instance();
	json user_json_object;
	std::ifstream read_file;
	try {
		read_file.open("users.json");
		if (!read_file.is_open())
			throw std::runtime_error("can not open file users.json");
		read_file >> user_json_object;
		read_file.close();
	}
	catch (std::exception& e) {
		std::ofstream write_file("users.json", std::ios::out | std::ios::trunc);
		write_file.close();
		return;
	}
	catch (const nlohmann::json::exception& e) {
		read_file.close();
		std::ofstream write_file("users.json", std::ios::out | std::ios::trunc);
		write_file.close();
		return;
	}

	if (!user_json_object.contains("users")) user_json_object["users"] = json::array();

	for (const auto& user_record : user_json_object["users"])
	{
		try {
			std::string username = user_record.at("username").get<std::string>();
			std::string password = user_record.at("password").get<std::string>();
			int follower_count = user_record.at("follower_count").get<int>();
			user* new_user = new user(username, password, follower_count);

			for (const auto& f : user_record.at("following")) {
				if (split_line(f.get<std::string>()).size() != 1)
					throw std::invalid_argument("invalid username");
				new_user->add_following(f.get<std::string>());
			}
			for (const auto& p : user_record.at("post_ids"))
			{
				if (p.get<int>() < 1)
					throw std::invalid_argument("invalid post id");
				new_user->add_post(p.get<int>());
			}
			for (const auto& l : user_record.at("liked_posts"))
			{
				if (l.get<int>() < 1)
					throw std::invalid_argument("invalid post id");
				new_user->add_liked_post(l.get<int>());
			}
			app.add_user_to_map(username, new_user);
			app.add_username_to_trie(username);
		}
		catch (const nlohmann::json::exception& e) {
			continue;
		}
		catch (const std::exception& e) {
			continue;
		}
	}
}

void manage_file_storage::load_posts_json_file()
{
	app_singleton& app = app_singleton::get_instance();
	json post_json_object;
	std::ifstream read_file;
	try {
		read_file.open("posts.json");
		if (!read_file.is_open())
			throw std::runtime_error("can not open file posts.json");
		read_file >> post_json_object;
		read_file.close();
	}
	catch (std::exception& e) {
		std::ofstream write_file("posts.json", std::ios::out | std::ios::trunc);
		write_file.close();
		return;
	}
	catch (const nlohmann::json::exception& e) {
		read_file.close();
		std::ofstream write_file("posts.json", std::ios::out | std::ios::trunc);
		write_file.close();
		return;
	}

	if (!post_json_object.contains("posts")) post_json_object["posts"] = json::array();
	if (!post_json_object.contains("count") || !post_json_object["count"].is_number())
		post_json_object["count"] = 0;

	post::set_count(post_json_object["count"]);
	for (const auto& post_record : post_json_object["posts"])
	{
		try {
			int id = post_record.at("id").get<int>();
			std::string content = post_record.at("content").get<std::string>();
			int like_count = post_record.at("like_count").get<int>();
			std::string author = post_record.at("author").get<std::string>();
			int month = post_record.at("published_date_time").at("month").get<int>();
			int day = post_record.at("published_date_time").at("day").get<int>();
			int year = post_record.at("published_date_time").at("year").get<int>();
			int hour = post_record.at("published_date_time").at("hour").get<int>();
			int minute = post_record.at("published_date_time").at("minute").get<int>();
			int second = post_record.at("published_date_time").at("second").get<int>();

			date_time published_date_time(month, day, year, hour, minute, second);
			post* new_post = new post(id, content, like_count, author, published_date_time);
			app.add_post_to_map(id, new_post);
		}
		catch (const std::bad_alloc& e)
		{
			std::cout << "Error : out of memory\n";
			exit(0);
		}
		catch (const nlohmann::json::exception& e) {
			continue;
		}
		catch (const std::exception& e) {
			continue;
		}
	}
}

void manage_file_storage::load_hashtags_json_file()
{
	app_singleton& app = app_singleton::get_instance();
	json hashtag_json_object;
	std::ifstream read_file;
	try {
		read_file.open("hashtags.json");
		if (!read_file.is_open())
			throw std::runtime_error("can not open file hashtags.json");
		read_file >> hashtag_json_object;
		read_file.close();
	}
	catch (std::exception& e) {
		std::ofstream write_file("hashtags.json", std::ios::out | std::ios::trunc);
		write_file.close();
		return;
	}
	catch (const nlohmann::json::exception& e) {
		read_file.close();
		std::ofstream write_file("hashtags.json", std::ios::out | std::ios::trunc);
		write_file.close();
		return;
	}

	if (!hashtag_json_object.contains("hashtags")) hashtag_json_object["hashtags"] = json::array();

	for (const auto& hash_record : hashtag_json_object["hashtags"])
	{
		try {
			std::string word = hash_record.at("hashtag").get<std::string>();
			if (split_line(word).size() != 1)
				throw std::invalid_argument("invalid hashtag");


			for (const auto& p : hash_record.at("id"))
			{
				if (p.get<int>() < 1)
					throw std::invalid_argument("invalid post id");
				app.add_hashtag_to_map(word, p);
			}
		}
		catch (const std::bad_alloc& e)
		{
			std::cout << "Error : out of memory\n";
			exit(0);
		}
		catch (const nlohmann::json::exception& e) {
			continue;
		}
		catch (const std::exception& e) {
			continue;
		}
	}
}

void manage_file_storage::save_users_json_file()
{
	app_singleton& app = app_singleton::get_instance();
	json root;
	root["users"] = json::array();
	std::ofstream write_file;
	hash_map<std::string, user*>& user_map = app.get_users_map();
	for (const auto u : user_map)
	{
		try {
			json curr_user;
			json post_ids = json::array();
			json like_ids = json::array();
			json followings = json::array();
			for (const auto name : u.second->get_followings())
			{
				followings.push_back(name);
			}
			for (const auto id : u.second->get_post_ids())
			{
				post_ids.push_back(id);
			}
			for (const auto like_id : u.second->get_liked_post_ids())
			{
				like_ids.push_back(like_id);
			}
			curr_user["username"] = u.second->get_username();
			curr_user["password"] = u.second->get_password();
			curr_user["follower_count"] = u.second->get_follower_count();
			curr_user["following"] = followings;
			curr_user["post_ids"] = post_ids;
			curr_user["liked_posts"] = like_ids;



			root["users"].push_back(curr_user);
		}
		catch (std::exception& e) {
			continue;
		}
	}


	try {
		write_file.open("users.json", std::ios::out | std::ios::trunc);
		if (!write_file.is_open())
			throw std::runtime_error("can not open file users.json\nsaving users.json failed");
		write_file << root.dump(1);
		write_file.close();
	}

	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

}
void manage_file_storage::save_posts_json_file()
{
	app_singleton& app = app_singleton::get_instance();
	json root;
	root["count"] = post::get_post_count();
	root["posts"] = json::array();
	std::ofstream write_file;
	hash_map<int, post*>& post_map = app.get_posts_map();
	for (const auto p : post_map)
	{
		try {
			json curr_post;
			json date_json;

			curr_post["id"] = p.second->get_id();
			curr_post["content"] = p.second->get_content();
			curr_post["author"] = p.second->get_author();
			curr_post["like_count"] = p.second->get_like_count();
			date_time p_date_time = p.second->get_published_date_time();

			date_json["month"] = p_date_time.get_month();
			date_json["day"] = p_date_time.get_day();
			date_json["year"] = p_date_time.get_year();
			date_json["hour"] = p_date_time.get_hour();
			date_json["minute"] = p_date_time.get_minute();
			date_json["second"] = p_date_time.get_second();

			curr_post["published_date_time"] = date_json;

			root["posts"].push_back(curr_post);
		}
		catch (std::exception& e) {
			continue;
		}
	}


	try {
		write_file.open("posts.json", std::ios::out | std::ios::trunc);
		if (!write_file.is_open())
			throw std::runtime_error("can not open file posts.json\nsaving posts.json failed");
		write_file << root.dump(1);
		write_file.close();
	}

	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

}
void manage_file_storage::save_hashtags_json_file()
{

	app_singleton& app = app_singleton::get_instance();
	json root;
	root["hashtags"] = json::array();

	std::ofstream write_file;
	hash_map<std::string, hash_set<int>>& hashtag_map = app.get_hashtags_map();
	for (const auto h : hashtag_map)
	{
		try {
			json curr_hash;
			json ids  = json::array();

			curr_hash["hashtag"] = h.first;
			for (const auto id : h.second)
			{
				ids.push_back(id);
			}
			curr_hash["id"] = ids;

			root["hashtags"].push_back(curr_hash);
		}
		catch (std::exception& e) {
			continue;
		}
	}


	try {
		write_file.open("hashtags.json", std::ios::out | std::ios::trunc);
		if (!write_file.is_open())
			throw std::runtime_error("can not open file hashtags.json\nsaving hashtags.json failed");
		write_file << root.dump(1);
		write_file.close();
	}

	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}