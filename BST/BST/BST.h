#ifndef BST_H
#define BST_H

#include "BSTInterface.h"

#include <sstream>
#include <string>

using namespace std;

template<typename Teml>
class BST : public BSTInterface<Teml> {
private:
	//NODE nested struct--------------------------------------------------------------------
	struct Node {
		Teml nodeValue;
		Node* leftChild;
		Node* rightChild;
		//CONSTRUCTORS
		Node(const Teml& listNode) :
			nodeValue(listNode),
			leftChild(NULL),
			rightChild(NULL)
		{}
		Node(const Teml& listNode, Node* leftPtr, Node* rightPtr) :
			nodeValue(listNode),
			leftChild(leftPtr),
			rightChild(rightPtr)
		{}
		//SETTERS
		void setValue(const Teml nodeValue) {
			this->nodeValue = nodeValue;
		}
		void setLeftChild(Node* ptr) {
			leftChild = ptr;
		}
		void setRightChild(Node* ptr) {
			rightChild = ptr;
		}
		//PRINT
		string toString() const {
			stringstream out;
			out << this->nodeValue << " ";
			return out.str();
		}
		friend ostream& operator<< (ostream& os, Node& thing) {
			os << thing.toString();
			return os;
		}
	};
	//--------------------------------------------------------------------------------------
	unsigned int numNodes;
	Node* root;
public:
	BST(void) {
		root = NULL;
		numNodes = 0;
	}
	~BST(void) {
		//clearTree();
	}
	Node* getRoot(void) {
		return root;
	}

	//Return true if node added to BST, else false
	virtual bool addNode(const Teml& addN) { return addNode(addN, root); }
	bool addNode(const Teml& addN, Node*& nodePos);

	//Return true if node removed from BST, else false
	virtual bool removeNode(const Teml& removeN) { return removeNode(removeN, root, NULL); } 
	bool removeNode(const Teml& removeN, Node* nodePos, Node* parent);

	// Return true if findN is found
	virtual bool find(const Teml& findN) { return find(findN, root); }
	bool find(const Teml& findN, const Node* nodePos) {		
		if (nodePos == NULL)				return false;
		if (nodePos->nodeValue == findN)	return true;
		if (findN < nodePos->nodeValue)		return find(findN, nodePos->leftChild);
		if (findN > nodePos->nodeValue)		return find(findN, nodePos->rightChild);
		return false;
	}
	//Return number of nodes
	int size(void) {			
		return numNodes;
	}
	//Return true if BST cleared of all nodes, else false
	bool clearTree() {			
		while (root != NULL)
			removeNode(root->nodeValue, root, NULL);
		if (numNodes != 0)
			return false;
		return true;
	}
	//this would print the tree in order if I had the iterator working
	string inOrder() {
		if (numNodes == 0)
			return "empty";
		else
			return "not implemented";
	}

	// Output nodes at a given level
	bool outLevel(Node* root, int level, stringstream& out) const;
	// Return a level order traversal of a BST as a string
	string toString() const;
	friend ostream& operator<< (ostream& os, BST<Teml>& thing) {
		os << thing.toString();
		return os;
	}

	//ITERATOR nested class ================================================================
	class Iterator {
	private:
		size_t index;
		Node* iterNode;
		Node* root;
	public:
		Iterator(Node* root) : iterNode(root), index(0) {}
		Iterator(Node* root, size_t index) : iterNode(root), index(index) {}
		~Iterator() {}
		Teml operator*() const {
			return iterNode->nodeValue;
		}
		Iterator& operator++(void) {
			if (iterNode->rightChild != NULL) {
				iterNode = iterNode->rightChild;
				while (iterNode->leftChild != NULL) {
					iterNode = iterNode->leftChild;
				}
			}
			else {
				Teml value = iterNode->nodeValue;
				Node* nextNode = root;
				if (nextNode->nodeValue > value)
					while (nextNode->leftChild->nodeValue > value) {
						nextNode = nextNode->leftChild;
					}
				else {
					while (nextNode != NULL && nextNode->nodeValue <= value)
						nextNode = nextNode->rightChild;
					while (nextNode != NULL &&  nextNode->leftChild != NULL && nextNode->leftChild->nodeValue > value)
						nextNode = nextNode->leftChild;
				}
				iterNode = nextNode;
				/*Node* parentNode = iterNode->parent;
				while (parentNode != NULL && iterNode == parentNode->rightChild) {
					iterNode = parentNode;
					parentNode = parentNode->parent;
				}
				iterNode = parentNode;*/
			}
			index++;
			return *this;
		}
		Iterator& operator--(void) {
			--index;
			return *this;
		}
		bool operator!=(const Iterator& check) const {
			return iterNode != check.iterNode;
		}
		bool operator==(const Iterator& check) const {
			return iterNode == check.iterNode;
		}
		void setRoot(Node* root) {
			this->root = root;
		}
		string toString() const {
			ostringstream oss;
			oss << "Index " << index << " Data: " << iterNode->nodeValue; 
		}
		friend ostream& operator<< (ostream& os, Iterator& thing) {
			os << thing.toString();
			return os;
		}
	};
	BST::Iterator begin() {
		Node* node = root;
		if (node != NULL) {
			while (node->leftChild != NULL)
				node = node->leftChild;
		}
		return BST<Teml>::Iterator(node, 0);
	}
	BST::Iterator end() {
		Node* node = root;
		if (node != NULL) {
			while (node->rightChild != NULL)
				node = node->rightChild;
		}
		return BST<Teml>::Iterator(node, numNodes);
	}
	//======================================================================================
};
#endif


//ADD------------------------------------------------------------------------------------
template <typename Teml>
bool BST<Teml>::addNode(const Teml& addN, Node*& nodePos) {
	if (nodePos == NULL) {
		Node* newNode = new Node(addN);
		nodePos = newNode;
		++numNodes;
		return true;
	}
	if (addN == nodePos->nodeValue) return false;
	if (addN < nodePos->nodeValue) return addNode(addN, nodePos->leftChild);
	if (addN > nodePos->nodeValue) return addNode(addN, nodePos->rightChild);
	return false;
}
//REMOVE---------------------------------------------------------------------------------
template <typename Teml>
bool BST<Teml>::removeNode(const Teml& removeN, Node* nodePos, Node* parent) {
	if (nodePos == NULL) return false;
	if (removeN < nodePos->nodeValue) return removeNode(removeN, nodePos->leftChild, nodePos);
	if (removeN > nodePos->nodeValue) return removeNode(removeN, nodePos->rightChild, nodePos);
	if (removeN == nodePos->nodeValue) {
		Node* child = nodePos;
		if (nodePos->leftChild != NULL) {			//check left child
			parent = nodePos->leftChild;			//set temp nodes
			if (parent->rightChild != NULL) {		//check right child of parent
				child = parent->rightChild;
				while (child->rightChild != NULL) {	//iterate until the predecessor is reached
					child = child->rightChild;
					parent = parent->rightChild;
				}
			}
			else {									//if parent is end, update temp nodes
				child = parent;
				parent = nodePos;
			}
		}
		else if (nodePos == root) {
			parent = NULL;
			child = root;
		}
		else if (nodePos->rightChild != NULL) {		//if no left child, check right child
			parent = nodePos->rightChild;
			if (parent->rightChild != NULL) {		//check next level right child
				child = parent->rightChild;
				while (child->rightChild != NULL) {
					child = child->rightChild;
					parent = parent->rightChild;
				}
			}
			else {									//if parent is the end, update nodes
				child = parent;
				parent = nodePos;
			}
		}
		else {										//if nodePos is the last node, update nodes
			child = nodePos;
		}
		if (parent != NULL) {						//update parent appropriately
			Teml oldParentValue = parent->nodeValue;
			if (child->leftChild != NULL) {
				if (parent->leftChild == NULL)
					parent->setRightChild(child->leftChild);
				else
					parent->setLeftChild(child->leftChild);
			}
			else if (child->nodeValue < oldParentValue)
				parent->setLeftChild(NULL);
			else
				parent->setRightChild(NULL);
		}
		else if (child == root)						//update root if necessary
			root = root->rightChild;
		nodePos->setValue(child->nodeValue);		//update child
		delete child;								//update private variables
		--numNodes;
		if (numNodes == 0)
			root = NULL;
		return true;
	}
	return false;
}
//LEVEL PRINT each node on given level input---------------------------------------------
template <typename Teml>
bool BST<Teml>::outLevel(Node* root, int level, stringstream& out) const {
	if (root == NULL)
		return false;
	if (level == 1) {
		out << " " << root->nodeValue;
		if ((root->leftChild != NULL) || (root->rightChild != NULL))
			return true;
		return false;
	}
	if ((level == 2) && !root->leftChild && root->rightChild)
		out << " _";
	bool left = outLevel(root->leftChild, level - 1, out);
	bool right = outLevel(root->rightChild, level - 1, out);
	if ((level == 2) && root->leftChild && !root->rightChild)
		out << " _";
	return left || right;
}
//TO STRING call outLevel for each level of the BST--------------------------------------
template <typename Teml>
string BST<Teml>::toString() const {
	stringstream out;
	if (root == NULL) out << " empty";
	else {
		int level = 0;
		do {
			out << endl << "  " << ++level << ":";
		} while (outLevel(root, level, out));
	}
	return out.str();
}