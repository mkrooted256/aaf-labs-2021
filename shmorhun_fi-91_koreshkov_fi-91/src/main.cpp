#include "Set.h"
#include <iostream>
#include <vector>
using namespace std;


int main() {
	Set a({ 2,5,4,3,1,6,7,2,8,5,2, 10,12,45 });
	Set b({ 2,5,4,3,7,2,8,5,2,120 });
	cout << a << endl;
	cout << b << endl;
	cout << a.Minus(b) << endl;
}
