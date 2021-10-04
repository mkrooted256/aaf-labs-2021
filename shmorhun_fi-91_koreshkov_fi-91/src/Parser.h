// #include<iostream>
#pragma once
#include<vector>
#include<algorithm>
#include<cctype>
#include<cstdlib>
#include<string>
#include "Set.h"

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

size_t strffind(bool (*filter)(char), std::string str) {
	size_t l = str.length();
	for (size_t i = 0; i < l; i++) {
		if (filter(str[i])) return i;
	}
	return std::string::npos;
}

Set parse_set(
	std::vector<std::string>::iterator begin, 
	std::vector<std::string>::iterator end) {
	Set set;

	
	auto i = begin;
	for (; i != end; i++) {
		if (*i == "{") break;
	}
	if (i == end) return set;
	// no set at all
	i++;

	enum { VAL, COMMA, END } state = VAL;
	for (; i != end; i++) {
		if ((*i)[0] == '}') {
			state = END;
			break;
		}
		if (state == VAL) {
			int val = 0;
			if ((*i)[0] != '0') {
				val = strtol((*i).c_str(), nullptr, 10);
				if (val == 0) {
					// Invalid syntax. Return empty set.
					return Set();
				}
			}
			set.Insert(val);
			state = COMMA;
		}
		else {
			if (*i != ",") {
				// Invalid syntax. Return empty set.
				return Set();
			}
			state = VAL;
		}
	}
	if (state != END) {
		// Invalid syntax. Return empty set.
		return Set();
	}

	return set;
}

bool is_in(char c, const char* str) {
	auto i = str;
	while (*i != '\0') {
		if (c == *i) return true;
		i++;
	}
	return false;
}

class Request {
public:
	static const int CMD_UNPARSED = -1;
	static const int CMD_CREATE = 10;
	static const int CMD_INSERT = 20;
	static const int CMD_SEARCH = 30;
	static const int CMD_SEARCH_WHERE = 40;

	static const int FILTER_INTERSECTS = 100;
	static const int FILTER_SUBSET = 200;
	static const int FILTER_SUPERSET = 300;

public:
	Request() : command(CMD_UNPARSED), filter(0) {};

public:

	int command;
	std::string target;
	Set payload;
	int filter;

	std::string parse_error;

	static Request parse(std::string str) {
		Request req;
		const char* ws = " \n\r\t\v\f";
		const char* delims = "{,}";
		
		size_t start = str.find_first_not_of(ws);
		size_t end = str.length();

		std::vector<std::string> tokens;
		size_t pos = start;
		size_t token_start = start;
		while (pos < end) {
			if (str[pos] == ';') {
				tokens.push_back(str.substr(token_start, pos - token_start));
				break;
			}
			if (is_in(str[pos], delims)) {
				if (pos != token_start) {
					tokens.push_back(str.substr(token_start, pos - token_start));
				}
				tokens.push_back(str.substr(pos, 1));
				token_start = pos = pos+1;
			} 
			if (is_in(str[pos], ws)) {
				if (token_start != pos) {
					tokens.push_back(str.substr(token_start, pos - token_start));
				}
				token_start = pos = str.find_first_not_of(ws, pos);
			}
			else {
				pos++;
			}
		}
		
		if (tokens.size() < 2) {
			req.command = CMD_UNPARSED;
			req.parse_error = "too few tokens; parse aborted.";
			return req;
		}

		// token[0] is a command.
		// make cmd uppercase
		auto i_tok = tokens.begin();
		std::string cmd = *i_tok;
		for (auto i = cmd.begin(); i != cmd.end(); i++) {
			*i = toupper(*i);
		}

		if (cmd == "CREATE") {
			req.command = CMD_CREATE;
			i_tok++;
			req.target = *i_tok;
		}
		else if (cmd == "INSERT") {
			req.command = CMD_INSERT;
			i_tok++;
			req.target = *i_tok;
			i_tok++;
			req.payload = parse_set(i_tok, tokens.end());
		}
		else if (cmd == "SEARCH") {
			i_tok++;
			req.target = *i_tok;
			i_tok++;
			if (i_tok == tokens.end()) {
				// select whole collection
				req.command = CMD_SEARCH;
				return req;
			}
			// Skip validation of "WHERE" because i do not care
			i_tok++;
			if (i_tok == tokens.end()) {
				req.command = CMD_UNPARSED;
				req.parse_error = "invalid syntax: SEARCH WHERE command has no filter.";
				return req;
			}

			std::string filter = *i_tok;
			for (auto i = filter.begin(); i != filter.end(); i++) {
				*i = toupper(*i);
			}
			if (filter == "INTERSECTS") {
				req.filter = FILTER_INTERSECTS;
			} 
			else if (filter == "CONTAINS") {
				req.filter = FILTER_SUPERSET;
			}
			else if (filter == "CONTAINED_BY") {
				req.filter = FILTER_SUBSET;
			}
			else {
				req.command = CMD_UNPARSED;
				req.parse_error = "invalid syntax: SEARCH WHERE command has invalid filter.";
				return req;
			}
			req.command = CMD_SEARCH_WHERE;

			i_tok++;
			
			req.payload = parse_set(i_tok, tokens.end());
		}
		return req;
	}

	friend std::ostream& operator<<(std::ostream& os, const Request& req) {
		os << "Request [";
		if (req.command == Request::CMD_CREATE) {
			os << "CREATE";
		}
		else if (req.command == Request::CMD_INSERT) {
			os << "INSERT";
		}
		else if (req.command == Request::CMD_SEARCH) {
			os << "SEARCH";
		}
		else if (req.command == Request::CMD_SEARCH_WHERE) {
			os << "SEARCH_WHERE";
		}
		else if (req.command == Request::CMD_UNPARSED) {
			os << "unparsed";
		}
		os << "] '" << req.target << "' - {" << req.payload << "} | ";
		if (req.filter == Request::FILTER_INTERSECTS) {
			os << "(intersects)";
		} else if (req.filter == Request::FILTER_SUBSET) {
			os << "(subset)";
		} else if (req.filter == Request::FILTER_SUPERSET) {
			os << "(superset)";
		}
		return os;
	}
};