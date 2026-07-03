#pragma once
#include <string>
#include <vector>
#include "hash_map.h"
class trie {
private:
    struct trie_node {
        hash_map<char, trie_node*> children;
        bool is_end_of_word;

        trie_node() : is_end_of_word(false) , children(11,1){}
    };

    trie_node* root;

    void collect_all_words(trie_node* node, std::string prefix, std::vector<std::string>& results);
    void compare_char(trie_node*, std::string, const std::string&, int, int, const int&, std::vector<std::string>&, bool);
public:
    trie();
    void insert(const std::string& key);
    std::vector<std::string> starts_with(const std::string& prefix);
    std::vector<std::string>& hamming(const std::string& ,const int& , std::vector<std::string>&);
  
};
