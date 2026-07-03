#pragma once
#include <string>
#include <vector>
#include <istream>
#include <sstream>

inline std::vector<std::string> split_line(const std::string& line) {
	std::vector<std::string> tokens;
	std::istringstream ss(line);
	std::string word;
	while (ss >> word) {
		tokens.push_back(word);
	}
	return tokens;
}

