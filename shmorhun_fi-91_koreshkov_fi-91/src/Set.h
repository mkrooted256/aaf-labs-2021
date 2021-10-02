#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

// Set

class Set {
public:
	static const size_t DEFAULT_SZ = 8;
	bool is_user_set = 0;
	std::vector<int> data;

	Set(std::vector<int> _data) {
		std::sort(_data.begin(), _data.end());
		auto last = std::unique(_data.begin(), _data.end());
		data.insert(data.begin(), _data.begin(), last);
	}
	Set() {}

	bool Contains(int x);
	bool Insert(int x);
	bool Delete(int x);

	Set Union(const Set& other);
	Set Intersection(const Set& other);
	Set Minus(const Set& other);

	bool IsSubsetOf(const Set& other);
	bool IsDisjointWith(const Set& other);

	friend std::ostream& operator<<(std::ostream&, const Set&);
};

