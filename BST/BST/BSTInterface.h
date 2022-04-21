//**** YOU MAY NOT MODIFY THIS DOCUMENT ****/
#ifndef BST_INTERFACE_H
#define BST_INTERFACE_H
#include <string>

/** A binary tree node with data, left and right child pointers */
template<typename T>
class BSTInterface
{
public:
	BSTInterface(void) {}
	virtual ~BSTInterface(void) {}

	/** Return true if node added to BST, else false */
	virtual bool addNode(const T&) = 0;

	/** Return true if node removed from BST, else false */
	virtual bool removeNode(const T&) = 0;

	/** Return true if BST cleared of all nodes, else false */
	virtual bool clearTree() = 0;

	/** Return a level order traversal of a BST as a string */
	virtual std::string toString() const = 0;
};
#endif	// BST_INTERFACE_H