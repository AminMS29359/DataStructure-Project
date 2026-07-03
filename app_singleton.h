#pragma once
#include <string>
#include <queue>
#include "trie.h"
#include "hash_map.h"
#include "hash_set.h"
class user;
class post;

class app_singleton {
private:
    hash_map<std::string, user*> users_u_map;
    trie userames_trie;
    hash_map<int, post*> posts_u_map;
    hash_map<std::string, hash_set<int>> hashtags_u_map;
    std::queue<std::string >last_five_searchs;
    user* logged_in_user;

    app_singleton();
    app_singleton& operator=(const app_singleton& ) = delete;
    app_singleton(const app_singleton&) = delete;
public :
    static app_singleton& get_instance();
    void run();
    void handle_commands(std::string & line);
    void add_user_to_map( const std::string& _username ,  user* _user);
    void add_post_to_map( const int& _id ,  post* _post);
    void add_username_to_trie(const std::string& );
    void add_hashtag_to_map(const std::string&, const int&);
    user* login(const std::string &username, const std::string &password);
    user* signup(const std::string &username , const std::string& password );
    void search_user(std::string &prefix);
    void follow(std::string &_username  );
    void unfollow(std::string& _username);
    void user_posted(std::string & line);
    void trend(const std::string &);
    void like(const int& id);
    void show_feed();
    void smart_search( std::string & username);
    double calculate_score(int, int, double);
    void push_into_queue(std::string &);
    void show_history();
    hash_map < std::string,user*>& get_users_map();
    hash_map < int, post*>& get_posts_map();
    hash_map < std::string, hash_set<int>>& get_hashtags_map();
    void exit_app();
    void load_files();
    void get_user_input();
    void bfs_friends(const std::string& root, const int& step, std::vector<std::string>& results);
    void suggest_friends();

};