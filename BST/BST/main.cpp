#include "BST.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

using namespace std;
static const string INT = "INT";

string boolToStr(bool print) {
	if (print) return "true";
	return "false";
}
string findToStr(bool print) {
	if (print) return "found";
	return "not found";
}

int main(int argc, char* argv[]) {
	VS_MEM_CHECK

	//open files
	if (argc < 3) {
		cerr << "Please provide name of input and output files";
		return 1;
	}
	ifstream in(argv[1]);
	if (!in.is_open()) {
		cout << "Uh oh there was an inputFile error..." << endl;
		return 2;
	}
	ofstream out(argv[2]);
	if (!out.is_open()) {
		cout << "Uh oh there was an outputFile error..." << endl;
		return 3;
	}

	//instantiate objects
	string command;
	string dataType;
	int iValue;
	string sValue;
	BST<int> intBST;
	BST<string> stringBST;

	while (in >> command) {
		if (command == "INT") {				//INT
			dataType = command;
			out << dataType << " true" << endl;
		}
		else if (command == "STRING") {		//STRING
			dataType = command;
			out << command << " true " << endl;
		}
		else if (command == "add") {		//ADD
			if (dataType == INT) {
				in >> iValue;
				out << command << " " << iValue << " " << boolToStr(intBST.addNode(iValue)) << endl;
			}
			else {
				in >> sValue;
				out << command << " " << sValue << " " << boolToStr(stringBST.addNode(sValue)) << endl;
			}
		}
		else if (command == "remove") {		//REMOVE
			if (dataType == INT) {
				in >> iValue;
				out << command << " " << iValue << " " << boolToStr(intBST.removeNode(iValue)) << endl;
			}
			else {
				in >> sValue;
				out << command << " " << sValue << " " << boolToStr(stringBST.removeNode(sValue)) << endl;
			}
		}
		else if (command == "print") {		//PRINT
			if (dataType == INT) {
				out << command << ":" << intBST << endl;
			}
			else {
				out << command << ":" << stringBST << endl;
			}
		}
		else if (command == "clear") {		//CLEAR
			if (dataType == INT) {
				out << command << " " << boolToStr(intBST.clearTree()) << endl;
			}
			else {
				out << command << " " << boolToStr(stringBST.clearTree()) << endl;
			}
		}
		else if (command == "size") {		//SIZE
			if (dataType == INT) {
				out << command << " " << intBST.size() << endl;
			}
			else {
				out << command << " " << stringBST.size() << endl;
			}
		}
		else if (command == "find") {		//FIND
			if (dataType == INT) {
				in >> iValue;
				out << command << " " << iValue << " " << findToStr(intBST.find(iValue)) << endl;
			}
			else {
				in >> sValue;
				out << command << " " << sValue << " " << findToStr(stringBST.find(sValue)) << endl;
			}
		}
		else if (command == "tree") {		//TREE
			out << command << ": ";
			if (dataType == INT) {
				if (intBST.size() > 0) {
					BST<int>::Iterator iter = intBST.begin();
					iter.setRoot(intBST.getRoot());
					for (; iter != intBST.end(); ++iter) {
						out << *iter << " ";
					}
					out << *intBST.end();
				}
				else out << "empty";
			}
			else {
				if (stringBST.size() > 0) {
					BST<string>::Iterator iter = stringBST.begin();
					iter.setRoot(stringBST.getRoot());
					for (; iter != stringBST.end(); ++iter) {
						out << " " << *iter;
					}
					out << *stringBST.end();
				}
				else out << "empty";
			}
			out << endl;
		}
	}

	//delete and close
	intBST.clearTree();
	stringBST.clearTree();
	in.close();
	out.close();
	return 0;
}