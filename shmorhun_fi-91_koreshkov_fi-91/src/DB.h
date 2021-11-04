#pragma once
#include "Collection.h"
#include "Request.h"
#include <string>
#include <map>

class DB {
public:
	struct Result {
		bool error;
		std::vector<Set> data;
		std::string msg;

		Result() : error(false) {}

		static Result Error(const std::string& msg) {
			Result res;
			res.error = true;
			res.msg = msg;
		}
		static Result Success(const std::string& msg) {
			Result res;
			res.error = false;
			res.msg = msg;
		}

		static const Result ERR_UNIMPLEMENTED = Error("Unimplemented");

	};

private:
	std::string db_name;
	std::map<const std::string, Collection> collections;

public:

	Result Create(std::string collection_name) {
		const auto [it, collectionCreated] = collections.try_emplace(collection_name);
		
		if (!collectionCreated) {
			std::string msg = "Collection '" + collection_name + "' already exists";
			return Result::Error(msg);
		}

		std::string msg = "Collection '" + collection_name + "' created successfully";
		return Result::Success(msg);
	}

	Result Insert(std::string collection_name, Set insert_set) {
		const auto it = collections.find(collection_name);

		if (it == collections.end()) {
			std::string msg = "Collection '" + collection_name + "' does not exist";
			return Result::Error(msg);
		}

		it->second.Insert(insert_set);

		return Result::Success("1 set inserted.");
	}

	Result InsertAll(std::string collection_name, std::vector<Set> insert_sets) {
		const auto it = collections.find(collection_name);

		if (it == collections.end()) {
			std::string msg = "Collection '" + collection_name + "' does not exist";
			return Result::Error(msg);
		}

		for (const Set& insert_set: insert_sets) {
			it->second.Insert(insert_set);
		}

		return Result::Success(std::to_string(insert_sets.size) + " sets inserted.");
	}

	Result PrintTree(std::string collection_name, std::ostream& os) {
		const auto it = collections.find(collection_name);

		if (it == collections.end()) {
			std::string msg = "Collection '" + collection_name + "' does not exist";
			return Result::Error(msg);
		}

		it->second.Print(os);
		
		return Result::Success("");
	}

};
