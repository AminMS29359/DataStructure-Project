#include "app_singleton.h"
#include <exception>  
#include <stdexcept>   
#include <new>     
#include <iomanip>
#include <iostream>
#include <string.h>
#include "user.h"
#include "post.h"
#include "util.h"
#include "manage_file_storage.h"
#include "max_heap.h"
app_singleton::app_singleton() :logged_in_user(nullptr) ,posts_u_map(11, 1) {}

app_singleton& app_singleton::get_instance()
{
	static app_singleton instance;
	return instance;
}
void app_singleton::load_files()
{
	std::cout << "Loading data.........." << std::endl;
	manage_file_storage::load_users_json_file();
	manage_file_storage::load_posts_json_file();
	manage_file_storage::load_hashtags_json_file();
	system("cls");
	std::cout << "Loading data completed" << std::endl;
}

void app_singleton::get_user_input()
{
	std::string line;
	while (true)
	{
		std::cout << std::endl << ">>";
		std::getline(std::cin, line);
		handle_commands(line);
	}
}
void app_singleton::run()
{
	
	load_files();
	get_user_input();
}
void app_singleton::handle_commands(std::string& line)
{
	std::vector<std::string> tokens;
	std::string command;

	if (logged_in_user == nullptr)
	{
		tokens = split_line(line);
		if (tokens.empty())
			return;

		command = tokens[0];
		try {
			if (command == "signup")
			{
				if (tokens.size() != 3)
				{
					throw std::exception("invalid input for signup command \nuse signup <username> <password>\n");
				}
				logged_in_user = signup(tokens[1], tokens[2]);
			}

			else if (command == "login")
			{
				if (tokens.size() != 3)
				{
					throw std::exception("invalid input for login command\nuse login <username> <password>\n");
				}
				logged_in_user = login(tokens[1], tokens[2]);
			}

			else throw std::out_of_range("invalid command");
		}
		catch (std::out_of_range& e)
		{
			std::cout << "Error : " << e.what() << std::endl;
			std::cout << "signup <username> <password>" << std::endl << "login <username> <password>" << std::endl;
		}
		catch (std::bad_alloc& e) {
			std::cout << "Error : out of memory";
			exit(0);
		}
		catch (std::exception& e)
		{
			std::cout << "Error : " << e.what() << std::endl;
		}
	}

	else
	{
		tokens = split_line(line);
		if (tokens.empty())
			return;

		command = tokens[0];
		try {
			if (command == "search_user")
			{
				if (tokens.size() != 2)
				{
					throw std::exception("invalid input for search_user command\nuse search_user <prefix>");
				}
				search_user(tokens[1]);
			}
			else if (command == "follow")
			{
				if (tokens.size() != 2)
				{
					throw std::exception("invalid input for follow command\nuse follow <username>");
				}
				follow(tokens[1]);
			}
			else if (command == "unfollow")
			{
				if (tokens.size() != 2)
				{
					throw std::exception("invalid input for unfollow command\nuse unfollow <username>");
				}
				unfollow(tokens[1]);
			}
			else if (command == "post")
			{
				if (tokens.size() == 1)
				{
					throw std::exception("invalid input for post command\nuse post <\"content\">");
				}
				user_posted(line);
			}
			else if (command == "trend")
			{
				if (tokens.size() != 2)
					throw std::exception("invalid input for trend command\nuse trend <#word>");
				if (tokens[1].at(0) != '#')
					throw std::exception("you can use trend command for hashtags only ( #word )");
				trend(tokens[1]);
			}
			else if (command == "like")
			{
				if (tokens.size() != 2)
					throw std::exception("invalid input for like command\nuse like <post_id>");
				for (auto& c : tokens[1])
				{
					if (!std::isdigit(c))
						throw std::invalid_argument("invalid post id");
				}
				like(std::stoi(tokens[1]));
			}
			else if (command == "show_feed")
			{
				if (tokens.size() != 1)
					throw std::exception("invalid input for show_feed command\nuse show_feed");

				show_feed();
			}
			else if (command == "smart_search")
			{
				if (tokens.size() != 2)
					throw std::exception("invalid input for smart_search command\nuse smart_search <username>");
				smart_search(tokens[1]);
			}
			else if (command == "show_history")
			{
				if (tokens.size() != 1)
					throw std::exception("invalid input for show_history command\nuse show_history");
				show_history();
			}
			else if (command == "suggest_friends")
			{
				if (tokens.size() != 1)
					throw std::exception("invalid input for suggest_friends command\nuse suggest_friends");
				suggest_friends();
			}
			else if (command == "exit")
			{
				if (tokens.size() != 1)
					throw std::exception("invalid input for exit command\nuse exit");
				exit_app();
			}
			else throw std::out_of_range("invalid command");
		}
		catch (std::out_of_range& e)
		{
			std::cout << "Error : " << e.what() << std::endl;
			std::cout << "search_user <prefix>" << std::endl
				<< "follow <username>" << std::endl
				<< "unfollow <username>" << std::endl
				<< "post <\"content\"" << std::endl
				<< "like <post_id>" << std::endl
				<< "trend <#hashtag>" << std::endl
				<< "show_feed" << std::endl
				<< "smart_search <username>" << std::endl
				<< "show_history" << std::endl
				<< "suggest_friends" << std::endl
				<< "exit" << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << "Error : " << e.what() << std::endl;
		}

	}
}
void app_singleton::add_user_to_map(const std::string& _username, user* _user)
{
	users_u_map.insert(_username, _user);
}
void app_singleton::add_username_to_trie(const std::string& _username)
{
	userames_trie.insert(_username);
}
void app_singleton::add_post_to_map(const int& _id, post* _post)
{
	posts_u_map.insert(_id, _post);
}
void app_singleton::add_hashtag_to_map(const std::string& _word, const int& _id)
{
	auto it = hashtags_u_map.find(_word);
	if (it != hashtags_u_map.end())
		(*it).second.insert(_id);

	else {
		hash_set<int> empty(11, 1);
		empty.insert(_id);
		hashtags_u_map.insert(_word, empty);
	}
}
user* app_singleton::signup(const std::string& _username, const std::string& _passwrod) {
	if (users_u_map.find(_username) != users_u_map.end())
		throw std::invalid_argument("This username already exists");
	user* new_user = new user(_username, _passwrod, 0);
	users_u_map.insert(_username, new_user);
	userames_trie.insert(_username);
	std::cout << "signed up successfully" << std::endl;
	return new_user;
}

user* app_singleton::login(const std::string& _username, const std::string& _passwrod) {
	auto it = users_u_map.find(_username);
	if (it == users_u_map.end())
		throw std::invalid_argument("username or password is incorrect");

	if ((*it).second->compare_password(_passwrod) == false)
		throw std::invalid_argument("username or password is incorrect");

	std::cout << "logged in successfully" << std::endl;
	return (*it).second;
}
void app_singleton::search_user(std::string& prefix) {
	push_into_queue(prefix);
	std::vector<std::string> result = userames_trie.starts_with(prefix);
	if (result.empty())
	{
		std::cout << "No user found with this prefix" << std::endl;
		return;
	}
	for (auto& it : result)
		std::cout << it << std::endl;
}
void app_singleton::follow(std::string& _username)
{
	auto it = users_u_map.find(_username);
	if (it == users_u_map.end())
		throw std::invalid_argument("this username dosen't exist");
	char state = logged_in_user->add_following(_username);
	if (state == 'A')
	{
		std::cout << "You can't follow/unfollow yourself" << std::endl;
	}
	else if (state == 'B')
	{
		std::cout << "You are already following " << _username << std::endl;
	}
	else
	{
		(*it).second->increase_follower_count();
		std::cout << "You've successfully followed " << _username << std::endl;
	}
}
void app_singleton::unfollow(std::string& _username)
{
	auto it = users_u_map.find(_username);
	if (it == users_u_map.end())
		throw std::invalid_argument("this username dosen't exist");
	char state = logged_in_user->remove_following(_username);

	if (state == 'A')
	{
		std::cout << "You can't follow/unfollow yourself" << std::endl;
	}
	else if (state == 'B')
	{
		std::cout << "You are not following " << _username << std::endl;
	}
	else
	{
		(*it).second->decrease_follower_count();
		std::cout << "You've successfully unfollowed " << _username << std::endl;
	}
}
void app_singleton::user_posted(std::string& line)
{
	size_t first = line.find('"');
	size_t last = line.rfind('"');

	if (first == std::string::npos || last == std::string::npos || first == last)
		throw std::exception("invalid input for post command");

	post* new_post = new post(line.substr(first + 1, last - first - 1), logged_in_user->get_username());

	int id = new_post->get_id();
	add_post_to_map(id, new_post);
	logged_in_user->add_post(id);

	for (size_t i = first; i < last; ++i)
	{
		std::string hashtag;
		int lenght = 0;
		if (line[i] == '#')
		{
			while (line[i] != ' ' && i < last)
			{
				hashtag += line[i];
				++i;
				++lenght;
			}
			if (lenght > 1)
			{
				add_hashtag_to_map(hashtag, id);
			}
		}
	}

	std::cout << "Your post has been published successfully" << std::endl;
}
void app_singleton::trend(const std::string& word)
{
	auto it = hashtags_u_map.find(word);
	if (it == hashtags_u_map.end())
	{
		std::cout << "No posts found for this hashtag" << std::endl;
		return;
	}


	for (auto& id : (*it).second)
	{
		auto found_post = posts_u_map.find(id);
		if (found_post == posts_u_map.end())
			continue;
		(*found_post).second->print_post();
	}
}
void app_singleton::like(const int& id)
{

	auto it = posts_u_map.find(id);
	if (it == posts_u_map.end())
		throw std::invalid_argument("invalid id");

	else if (!logged_in_user->add_liked_post(id))
	{
		std::cout << "you have already liked this post" << std::endl;
		return;
	}

	else {
		(*it).second->add_like_count();
		std::cout << "successfully liked post " << id << std::endl;
	}
}
void app_singleton::show_feed()
{
	max_heap top_posts;
	auto& followings_usernames = logged_in_user->get_followings();
	for (auto& name : followings_usernames)
	{
		auto found_user_iterator = users_u_map.find(name);
		if (found_user_iterator == users_u_map.end())
			continue;
		user* user_pointer = (*found_user_iterator).second;
		auto& posts_id = user_pointer->get_post_ids();

		for (auto& id : posts_id)
		{
			auto found_post_iterator = posts_u_map.find(id);
			if (found_post_iterator == posts_u_map.end())
				continue;
			post* post_pointer = (*found_post_iterator).second;
			int like_count = post_pointer->get_like_count();
			int follower_count = user_pointer->get_follower_count();
			double time_elapsed = post_pointer->get_time_passed();
			double score = calculate_score(like_count, follower_count, time_elapsed);
			std::pair<double, post*> heap_node(score, post_pointer);
			top_posts.push(heap_node);
		}
	}
	if (top_posts.size() == 0) {
		std::cout << "No posts found" << std::endl;
		return;
	}
	std::cout << "Top posts :" << std::endl;
	for (int i = 1; i <= 10; ++i)
	{
		std::pair<double, post*> node = top_posts.top();
		if (node.second == nullptr) break;
		std::cout << i << " - score : " << std::fixed << std::setprecision(2) << node.first << std::endl;
		node.second->print_post();
		top_posts.pop();
	}
}
double app_singleton::calculate_score(int like, int followers, double time)
{
	double time_score;
	if (time < 0)
		time_score = 0;
	else if (time < 1)
	{
		time_score = 1000;
	}
	else
	{
		time_score = 1000 / time;
	}
	return like * 2 + followers * 0.5 + time_score;
}
void app_singleton::smart_search(std::string& username)
{
	push_into_queue(username);
	std::vector<std::string > result;
	int hamming_distance;

	if (username.size() < 10)
		hamming_distance = 2;
	else hamming_distance = 3;

	userames_trie.hamming(username, hamming_distance, result);
	if (result.empty())
	{
		std::cout << "No usernames found !" << std::endl;
		return;
	}
	else
	{
		for (auto& s : result)
			std::cout << "Did you mean " << s << " ?" << std::endl;
	}

}
void app_singleton::push_into_queue(std::string& username)
{
	if (last_five_searchs.size() == 5)
		last_five_searchs.pop();
	last_five_searchs.push(username);
}
void app_singleton::show_history()
{
	if (last_five_searchs.empty())
	{
		std::cout << "search history is empty\n";
		return;
	}
	for (int i = 0;i < last_five_searchs.size(); ++i)
	{
		std::string word = last_five_searchs.front();
		std::cout << word << std::endl;
		last_five_searchs.pop();
		last_five_searchs.push(word);
	}
}
hash_map<std::string, user*>& app_singleton::get_users_map()
{
	return users_u_map;
}
hash_map<int, post*>& app_singleton::get_posts_map()
{
	return posts_u_map;
}
hash_map<std::string, hash_set<int>>& app_singleton::get_hashtags_map()
{
	return hashtags_u_map;
}
void app_singleton::exit_app()
{
	std::cout << "Saving data.............." << std::endl;
	manage_file_storage::save_users_json_file();
	manage_file_storage::save_posts_json_file();
	manage_file_storage::save_hashtags_json_file();
	std::cout << "Saving data completed\n";
	exit(0);
}
void app_singleton::suggest_friends()
{
	std::string username = logged_in_user->get_username();
	std::vector<std::string> results;
	bfs_friends(username, 2, results);
	
	if (results.size() == 0)
	{
		std::cout << "No friends found" << std::endl;
		return;
	}

	for (auto& it : results)
	{
		std::cout << it << std::endl;
	}
}
void app_singleton::bfs_friends(const std::string& root, const int& step, std::vector<std::string>& results)
{

	hash_map<std::string, std::pair<std::string, int>> parent_pair_map;
	std::queue<std::string >nodes_queue;

	parent_pair_map.insert(root, std::pair<std::string, int>(root, 0));
	nodes_queue.push(root);

	if (step == 0) {
		results.push_back(root);
		return;
	}

	while (!nodes_queue.empty())
	{
		std::string current = nodes_queue.front();
		nodes_queue.pop();

		auto it = parent_pair_map.find(current);
		int depth = (*it).second.second;

		auto user_it = users_u_map.find(current);
		if (user_it == users_u_map.end())
			continue;

		for (const auto& name : (*user_it).second->get_followings())
		{
			if (parent_pair_map.find(name) != parent_pair_map.end())
				continue;

			parent_pair_map.insert(name, std::pair<std::string, int>(current, depth + 1));

			if (depth + 1 == step)
				results.push_back(name);

			else
				nodes_queue.push(name);
		}
	}
}
