#pragma once

class manage_file_storage {
public:
	static void load_users_json_file();
	static void load_posts_json_file();
	static void load_hashtags_json_file();
	static void save_users_json_file();
	static void save_posts_json_file();
	static void save_hashtags_json_file();
};