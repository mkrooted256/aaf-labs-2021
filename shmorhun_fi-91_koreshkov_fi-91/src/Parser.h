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

struct parse_set_result {
	Set set;
	bool parsed;
	std::vector<std::string>::iterator end;

	static parse_set_result error() {
		parse_set_result r;
		r.parsed = false;
		return r;
	}
};

parse_set_result parse_set(
	std::vector<std::string>::iterator begin, 
	std::vector<std::string>::iterator end) {
	parse_set_result r{ Set(), true, end };

	
	auto i = begin;
	for (; i != end; i++) {
		if (*i == "{") break;
	}
	if (i == end) return parse_set_result::error();
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
					return parse_set_result::error();
				}
			}
			r.set.Insert(val);
			state = COMMA;
		}
		else {
			if (*i != ",") {
				// Invalid syntax. Return empty set.
				return parse_set_result::error();
			}
			state = VAL;
		}
	}
	if (state != END) {
		// Invalid syntax. Return empty set.
		return parse_set_result::error();
	}
	r.end = i;

	return r;
}

bool is_in(char c, const char* str) {
	auto i = str;
	while (*i != '\0') {
		if (c == *i) return true;
		i++;
	}
	return false;
}

std::string toupper(const std::string& str) {
	std::string upper;
	for (char c : str) {
		upper += toupper(c);
	}
	return upper;
}

const std::vector<std::string> KEYWOARDS = { "CREATE", "INSERT", "SEARCH", "WHERE", "INTERSECTS", "CONTAINS", "CONTAINED_BY" };

class RequestParser {
public:
	static const int CMD_UNPARSED = -1;
	static const int CMD_CREATE = 10;
	static const int CMD_INSERT = 20;
	static const int CMD_SEARCH = 30;
	static const int CMD_SEARCH_WHERE = 40;
	static const int CMD_CONTAINS = 50;

	static const int FILTER_INTERSECTS = 100;
	static const int FILTER_SUBSET = 200;
	static const int FILTER_SUPERSET = 300;
	

	static bool is_keyword(std::string str) {
		str = toupper(str);
		for (const std::string& kw : KEYWOARDS) {
			if (str == kw) return true;
		}
		return false;
	}


<<<<<<< HEAD
public:
	Request() : command(CMD_UNPARSED), filter(0) {};
=======
private:
	RequestParser() : command(CMD_UNPARSED), filter(0) {};
>>>>>>> 9a344d81488fe6559235d5c2e94f6f8020cf368e

public:

	int command;
	std::string target;
	Set payload;
	int filter;

	std::string parse_error;

	static RequestParser parse(std::string str) {
		RequestParser req;
		const char* ws = " \n\r\t\v\f";
		const char* delims = "{,}";
		
		size_t start = str.find_first_not_of(ws);
		size_t end = str.length();

		std::vector<std::string> tokens;
		size_t pos = start;
		size_t token_start = start;
		while (pos < end) {
			bool on_start = token_start == pos;
			if (str[pos] == ';') {
				if (!on_start) {
					tokens.push_back(str.substr(token_start, pos - token_start));
				}
				break;
			}
			
			if (is_in(str[pos], delims)) {
				if (!on_start) {
					if (str[pos] == '{') {
						req.parse_error = "invalid syntax: unexpected '{'";
						return req;
					}
					tokens.push_back(str.substr(token_start, pos - token_start));
				}
				tokens.push_back(str.substr(pos, 1));
				token_start = pos = pos+1;
			}
			if (is_in(str[pos], ws)) {
				if (!on_start) {
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
		std::string cmd = toupper(*i_tok);

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
			auto ps = parse_set(i_tok, tokens.end());
			if (!ps.parsed) {
				req.parse_error = "invalid syntax: cannot parse Set";
				return req;
			}
			req.payload = ps.set;
			i_tok = ps.end;
		}
		else if (cmd == "CONTAINS") {
			req.command = CMD_CONTAINS;
			i_tok++;
			req.target = *i_tok;
			i_tok++;
			auto ps = parse_set(i_tok, tokens.end());
			if (!ps.parsed) {
				req.parse_error = "invalid syntax: cannot parse Set";
				return req;
			}
			req.payload = ps.set;
			i_tok = ps.end;
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

			std::string filter = toupper(*i_tok);
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

			auto ps = parse_set(i_tok, tokens.end());
			if (!ps.parsed) {
				req.parse_error = "invalid syntax: cannot parse Set";
				return req;
			}
			req.payload = ps.set;
			i_tok = ps.end;
		}
		else {
			req.parse_error = "unknown command";
			return req;
		}
		i_tok++;
		if (i_tok != tokens.end()) {
			req.parse_error = "invalid syntax: extra tokens after the end of command";
		}
		if (is_keyword(req.target)) {
			req.parse_error = "invalid target: target cannot be a keyword";
		}
		return req;
	}

	friend std::ostream& operator<<(std::ostream& os, const RequestParser& req) {
		if (!req.parse_error.empty()) {
			os << "RequestParseError {" << std::endl << "  " << req.parse_error << std::endl << "}";
			return os;
		}
		os << "Request {" << std::endl << "  command: ";
		if (req.command == RequestParser::CMD_CREATE) {
			os << "CREATE";
		}
		else if (req.command == RequestParser::CMD_INSERT) {
			os << "INSERT";
		}
		else if (req.command == RequestParser::CMD_CONTAINS) {
			os << "CONTAINS";
		}
		else if (req.command == RequestParser::CMD_SEARCH) {
			os << "SEARCH";
		}
		else if (req.command == RequestParser::CMD_SEARCH_WHERE) {
			os << "SEARCH_WHERE";
		}
		else if (req.command == RequestParser::CMD_UNPARSED) {
			os << "unparsed";
		}
		os << std::endl << "  target: '";
		os << req.target << "'" << std::endl << "  payload: " << req.payload << std::endl << "  filter: ";
		if (req.filter == RequestParser::FILTER_INTERSECTS) {
			os << "(intersects)";
		} else if (req.filter == RequestParser::FILTER_SUBSET) {
			os << "(subset)";
		} else if (req.filter == RequestParser::FILTER_SUPERSET) {
			os << "(superset)";
		}
		os << std::endl << "}" << std::endl;
		return os;
	}
};