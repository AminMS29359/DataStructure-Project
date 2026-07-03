#pragma once
#include <vector>
class post;

class max_heap {
private:
	std::vector<std::pair<double ,post*>> nodes;
public:
	bool compare(double& x, double& y);
	void shift_up();
	void shift_down();
	void swap(std::pair<double , post*>& x, std::pair<double, post*>& y);
	void push(std::pair<double , post*>);
	void pop();
	std::pair<double , post*> top();
	int size();
};

