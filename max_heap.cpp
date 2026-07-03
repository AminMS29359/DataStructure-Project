#include "max_heap.h"

bool max_heap::compare(double& x, double& y) {
	return x > y;
}

void max_heap::shift_up() {
	int i = nodes.size() - 1;
	while (i > 0)
	{
		int parent_index = (i - 1) / 2;
		if (compare(nodes[i].first, nodes[parent_index].first))
		{
			swap(nodes[i], nodes[parent_index]);
			i = parent_index;
		}
		else
			break;
	}
}

void max_heap::shift_down() {
	int i = 0;
	int size = nodes.size();

	while (true) {
		int left_child_index = 2 * i + 1;
		int right_child_index = 2 * i + 2;
		int target_child_index = i; 

		if (left_child_index < size && compare(nodes[left_child_index].first, nodes[target_child_index].first)) {
			target_child_index = left_child_index;
		}

		if (right_child_index < size && compare(nodes[right_child_index].first, nodes[target_child_index].first)) {
			target_child_index = right_child_index;
		}

		if (target_child_index == i) break; 

		swap(nodes[i], nodes[target_child_index]);
		i = target_child_index;
	}
}

void max_heap::swap(std::pair<double , post*>& x, std::pair<double, post*>& y) {
	std::pair<double, post*> temp = x;
	x = y;
	y = temp;
}
void max_heap::push(std::pair<double , post*> p) {
	nodes.push_back(p);
	shift_up();
}
void max_heap::pop() {
	if (nodes.empty()) return;
	nodes[0] = nodes[nodes.size() - 1];
	nodes.pop_back();
	shift_down();
}
std::pair<double , post*> max_heap::top()
{
	if (nodes.empty()) return std::pair<double, post*>{0, nullptr};
	return nodes[0];
}
int max_heap::size()
{
	return nodes.size();
}
