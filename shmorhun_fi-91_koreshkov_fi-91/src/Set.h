#pragma once
#include <vector>
#include <algorithm>

// Set

class Set {
public:
	static const size_t DEFAULT_SZ = 8;
	std::vector<int> data;

	Set(std::vector<int> _data) {
		std::sort(_data.begin(), _data.end());
		std::unique(_data.begin(), _data.end());
		data = _data;
	}
	Set() {}

	bool Contains(int x);
	bool Insert(int x);
	bool Delete(int x);

	Set Union(Set& other);
	Set Intersection(Set& other);
	Set Minus(Set& other);

	bool IsSubsetOf(Set& other);
	bool IsDisjointWith(Set& other);
};
