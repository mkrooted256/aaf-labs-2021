#pragma once
#include "Collection.h"
#include "Request.h"

class DB {
public:
	// Map<string, Collection> collections;
	// string name;

	Request::Result Execute(const Request& req);

private:
	Request::Result ExecuteCreate(/*...*/);
	Request::Result ExecuteInsert(/*...*/);
	Request::Result ExecuteSearch(/*...*/);
	Request::Result ExecuteSearchWhere(/*...*/);
};
