#pragma once
#include "Collection.h"

/*
* cmd:
* - CREATE <target>;                    ::  create collection
*     - <target>  :: str, collection name
*     - (<payload> is empty)
*     - (<filter> is empty)
*
* - INSERT [INTO] <target> <payload>;   ::  insert set in colection
*     - <target>  :: str, collection where to insert
*     - <payload> :: vector<int>
*     - (<filter> is empty)
*
* - SEARCH <target>;                    :: show all from <target> collection
*     - <target>  :: str, collection where to search
*     - (<payload> is empty)
*     - (<filter> is empty)
*
* - SEARCH <target> where <filter> <payload>;                    :: show all from <target> collection
*     - <target>  :: str, collection where to search
*     - <payload> :: vector<int>
*     - <filter>  :: int
*
*/

static const int CMD_UNPARSED = -1;
static const int CMD_CREATE = 10;
static const int CMD_INSERT = 20;
static const int CMD_SEARCH = 30;
static const int CMD_SEARCH_WHERE = 40;
static const int CMD_CONTAINS = 50;

static const int FILTER_INTERSECTS = 100;
static const int FILTER_SUBSET = 200;
static const int FILTER_SUPERSET = 300;

class Request {
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
	};

	int command;
	std::string target;
	Set payload;
	int filter;

	friend std::ostream& operator<<(std::ostream& os, const Request& req);
};

std::ostream& operator<<(std::ostream& os, const Request& req) {
	os << "Request {" << std::endl << "  command: ";
	if (req.command == CMD_CREATE) {
		os << "CREATE";
	}
	else if (req.command == CMD_INSERT) {
		os << "INSERT";
	}
	else if (req.command == CMD_CONTAINS) {
		os << "CONTAINS";
	}
	else if (req.command == CMD_SEARCH) {
		os << "SEARCH";
	}
	else if (req.command == CMD_SEARCH_WHERE) {
		os << "SEARCH_WHERE";
	}
	else if (req.command == CMD_UNPARSED) {
		os << "unparsed";
	}
	os << std::endl << "  target: '";
	os << req.target << "'" << std::endl << "  payload: " << req.payload << std::endl << "  filter: ";
	if (req.filter == FILTER_INTERSECTS) {
		os << "(intersects)";
	}
	else if (req.filter == FILTER_SUBSET) {
		os << "(subset)";
	}
	else if (req.filter == FILTER_SUPERSET) {
		os << "(superset)";
	}
	os << std::endl << "}" << std::endl;
	return os;
}
