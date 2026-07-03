#include "trie.h"
#include <cstdlib>
void trie::collect_all_words(trie_node* node, std::string prefix, std::vector<std::string>& results) { 
    if (node->is_end_of_word)
        results.push_back(prefix);

    for (auto it : node->children) {
        collect_all_words(it.second, prefix + std::string(1, it.first), results);
    }
}

trie::trie() {
    root = new trie_node();
    root->is_end_of_word = false;
}

void trie::insert(const std::string &key) {
    trie_node* curr = root;
    for (char c : key) {
        auto it = curr->children.find(c);
        if (it == curr->children.end())
        {
            trie_node* new_node = new trie_node();
            curr->children.insert(c,new_node );
            curr = new_node;
            continue;
        }
        curr = (*it).second;
    }
    curr->is_end_of_word = true;
}

std::vector<std::string> trie::starts_with(const std::string& prefix) {
    trie_node* curr = root;
    for (char c : prefix) {
        auto it = curr->children.find(c);
        if (it == curr->children.end()) return {};
        curr = (*it).second;
    }

    std::vector<std::string> results;
    collect_all_words(curr, prefix, results);
    return results;
}
std::vector<std::string>& trie::hamming(const std::string& input,const int& max_distance ,std::vector<std::string>& result)
{
    compare_char(root, "", input, 0, 0,max_distance, result,true);
    return result;
}
void trie::compare_char(trie_node* node , std::string premade_string, const std::string& input,  int index,int distance,const int& max_distance, std::vector<std::string>& result ,bool compare)
{

    if (index == input.size())
        compare = false;

    if (node->is_end_of_word)
    {
        int len_dis = static_cast<int>(input.size()) - static_cast<int>(premade_string.size());
        if (std::abs(len_dis) + distance  <= max_distance)
            result.push_back(premade_string);
    }

    for (auto it : node->children)
    {
        int new_distance = distance;
        if (compare)
        {
            if (input[index] != it.first)
                ++new_distance;
        }
        if (new_distance > max_distance)
            continue;
        compare_char(it.second, premade_string + it.first , input, index + 1,new_distance, max_distance, result, compare);
    }
}