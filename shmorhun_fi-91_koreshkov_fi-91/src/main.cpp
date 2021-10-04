#include "Set.h"
#include "Parser.h"
#include "Collection.h"
#include <iostream>
#include <vector>
using namespace std;


int main() {

	//Tree first_tree = Tree();
	//Set a1 = Set({ 1 });//+
	//Set a2 = Set({ 1,2,5 });//+
	//Set a3 = Set({ 1,3,7 }); //+
	//Set a4 = Set({ 1,10,20 });
	//Set a5 = Set({ 1,2,3,7,10 }); //+
	//Set a6 = Set({ 1,3,5,7,10,20,70 });



	//first_tree.root.Insert(a1);
	//first_tree.root.Insert(a2);
	//first_tree.root.Insert(a3);
	//first_tree.root.Insert(a4);
	//first_tree.root.Insert(a5);
	//first_tree.root.Insert(a6);

	std::string input; //= "SEARCH table_1 WHERE INTERSECTS { 1, 2,3, 4  ,  5}";
	while (input != "exit") {
		std::cout << "> ";
		input = "";
		std::getline(cin, input, ';');
		input += ';';
		std::cout << "< " << RequestParser::parse(input) << std::endl << std::endl;
	}

	return 0;
	
}
