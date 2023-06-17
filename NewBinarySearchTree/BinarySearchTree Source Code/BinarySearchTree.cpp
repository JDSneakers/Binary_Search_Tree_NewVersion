//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : John Austin
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// FIXME (1): Internal structure for tree node
struct Node {
	Bid bid;
	Node* bidLeft;
	Node* bidRight;
    Node* amountParent;
    Node* amountLeft;
    Node* amountRight;

	//default constructor
	Node() {
		bidLeft = nullptr;
		bidRight = nullptr;
        amountParent = nullptr;
		amountLeft = nullptr;
		amountRight = nullptr;
	}

    //destructor implementation
    ~Node()
    {
        if (bidLeft != nullptr)
        {
			delete bidLeft;
		}
        if (bidRight != nullptr)
        {
            delete bidRight;
        }
        if (amountLeft != nullptr)
        {
			delete amountLeft;
		}
        if (amountRight != nullptr)
        {
            delete amountRight;
        }
        if (amountParent != nullptr)
        {
			delete amountParent;
		}
    }

	//initialize with a given bid
	Node(Bid aBid) : Node() {
		this->bid = aBid;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    Node* amountRoot;

    void addBid(Node* node, Bid bid);
    void inBidOrder(Node* node);
    void addAmountNode(Node* node, Bid bid);
    void inAmountOrder(Node* node);
    void amountSearch(Node* node, double lowAmount, double highAmount);
    Node* removeNode(Node* node, string bidId);
    

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InBidOrder();
    void InAmountOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid BidSearch(string bidId);
    void AmountSearch(double lowAmount, double highAmount);

    // Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // initialize housekeeping variables
	root = nullptr;
    amountRoot = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    if (root != nullptr) {
		this->removeNode(root, root->bid.bidId);
        this->removeNode(amountRoot, amountRoot->bid.bidId);
    }

}

/**
 * Traverse the tree bids in order
 */
void BinarySearchTree::InBidOrder() {
	this->inBidOrder(root);

}

/**
 * Traverse the tree amounts in order
 */
void BinarySearchTree::InAmountOrder() {
    this->inAmountOrder(amountRoot);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {

	if (root == nullptr) {
		root = new Node(bid);
	} else {
		this->addBid(root, bid);
	}

    if (amountRoot == nullptr) {
		amountRoot = new Node(bid);
    }
    else {
		this->addAmountNode(amountRoot, bid);
	}
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
	this->removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::BidSearch(string bidId) {

	//start searching from the root
	Node* current = root;

	//keep looping downwards until bottom reached or bid is found
	while (current != nullptr) {
		//if current node matches, return it
		if (current->bid.bidId.compare(bidId) == 0) {
			return current->bid;
		}
		//if bid is smaller than current traverse left
		if (bidId.compare(current->bid.bidId) <  0) {
			current = current->bidLeft;
		} else {
			current = current->bidRight;
		}
	}

	Bid bid;
    return bid;
}

/**
 * Search for bids within a range of values
 */
void BinarySearchTree::AmountSearch(double lowAmount, double highAmount) {
	//start searching from the root
    amountSearch(amountRoot, lowAmount, highAmount);
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addBid(Node* node, Bid bid) {

	//if node is larger than the bid, add to left subtree
	if (node->bid.bidId.compare(bid.bidId) > 0) {
		if (node->bidLeft == nullptr) {
			node->bidLeft = new Node(bid);
		} else {
			this->addBid(node->bidLeft, bid);
		}
	}

	//add right subtree
	else {
		if (node->bidRight == nullptr) {
			node->bidRight = new Node(bid);
		} else {
			this->addBid(node->bidRight, bid);
		}
	}
}

void BinarySearchTree::inBidOrder(Node* node) {
	if (node != nullptr) {
		inBidOrder(node->bidLeft);
        std::cout << node->bid.bidId << ": "
			 << node->bid.title << "| "
			 << node->bid.amount << "| "
			 << node->bid.fund << endl;
		inBidOrder(node->bidRight);
	}
}

void BinarySearchTree::inAmountOrder(Node* node) {

    if (node != nullptr) {
		inAmountOrder(node->amountLeft);
		std::cout << node->bid.bidId << ": "
			 << node->bid.title << "| "
			 << node->bid.amount << "| "
			 << node->bid.fund << endl;
		inAmountOrder(node->amountRight);
	}
}

void BinarySearchTree::addAmountNode(Node* node, Bid bid) {
	//if node is larger than the bid, add to left subtree
    if (node->bid.amount > bid.amount) {
        if (node->amountLeft == nullptr) {
			node->amountLeft = new Node(bid);
		}
        else {
			this->addAmountNode(node->amountLeft, bid);
		}
	}

		//add right subtree
    else {
        if (node->amountRight == nullptr) {
			node->amountRight = new Node(bid);
		}
        else {
			this->addAmountNode(node->amountRight, bid);
		}
	}
}

Node* BinarySearchTree::removeNode(Node* node, string bidId) {
	//if this node is null then return (avoid crashinng)
	if (node == nullptr) {
		return node;
	}

	//recurse down left subtree
	if (bidId.compare(node->bid.bidId) < 0) {
		node->bidLeft = removeNode(node->bidLeft, bidId);
	} else if (bidId.compare(node->bid.bidId) > 0) {
		node->bidRight = removeNode(node->bidRight, bidId);
	} else {
		// no children so this is a leaf node
		if (node->bidLeft == nullptr && node->bidRight == nullptr) {
			delete node;
			node= nullptr;
		}
		//one child to the left
		else if (node->bidLeft != nullptr && node->bidRight == nullptr) {
			Node* temp = node;
			node = node->bidRight;
			delete temp;
		}
		//one child to the right
		else if  (node->bidRight != nullptr && node->bidLeft == nullptr) {
			Node* temp = node;
			node = node->bidRight;
			delete temp;
		}
		//two children
		else {
			Node* temp = node->bidRight;
			while (temp->bidLeft != nullptr) {
				temp = temp->bidLeft;
			}
			node->bid = temp->bid;
			node->bidRight = removeNode(node->bidRight, temp->bid.bidId);
		}
	}
	return node;
}

void BinarySearchTree::amountSearch(Node* node, double lowAmount, double highAmount) {
    if (node != nullptr) {
		amountSearch(node->amountLeft, lowAmount, highAmount);
        if (node->bid.amount >= lowAmount && node->bid.amount <= highAmount) {
			std::cout << node->bid.bidId << ": "
				 << node->bid.title << "| "
				 << node->bid.amount << "| "
				 << node->bid.fund << endl;
		}
		amountSearch(node->amountRight, lowAmount, highAmount);
	}
}


//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    std::cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    std::cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        std::cout << c << " | ";
    }
    std::cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    double amountLow, amountHigh;
    
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree(); //create a new binary search tree

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        std::cout << "Menu:" << endl;
        std::cout << "  1. Load Bids" << endl;
        std::cout << "  2. Display All Bids" << endl;
        std::cout << "  3. Find Bid" << endl;
        std::cout << "  4. Find Bid by Amount" << endl;
        std::cout << "  5. Remove Bid" << endl;
        std::cout << "  9. Exit" << endl;
        std::cout << "Enter choice: ";
        
        // validate the input
        while (!(std::cin >> choice))
        {
            // clear the error stream
            std::cin.clear();

            // ignore the rest of the line
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // report the error
            std::cout << "Invalid input, please re-enter a valid choice: ";
        }
        

        switch (choice) {

        case 1:
            // Load bids
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
           
            // caculate elapsed time and display results
            std::cout << "time: " << ticks << " clock ticks" << endl;
            std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            // Display entire bid collection
            bst->InBidOrder();
            break;

        case 3:
            // Find a bid
            std::cout << "Enter bid id: "; //prompt user for bid id
            cin >> bidKey; //store bid id in bidKey
            ticks = clock(); //start clock
            bid = bst->BidSearch(bidKey); //search for bid id

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            // Display bid information
            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                std::cout << "Bid Id " << bidKey << " not found." << endl;
            }
            // Calculate elapsed time and display result
            std::cout << "time: " << ticks << " clock ticks" << endl;
            std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            // Find bids by amount
            std::cout << "Enter low amount: "; //prompt user for low amount
            cin >> amountLow; //store low amount in amountLow
            std::cout << "Enter high amount: "; //prompt user for high amount
            cin >> amountHigh; //store high amount in amountHigh
            bst->AmountSearch(amountLow, amountHigh);
            break;

        case 5:
            // Remove a bid
            std::cout << "Enter bid id: "; //prompt user for bid id
            cin >> bidKey; //store bid id in bidKey
            bst->Remove(bidKey);
            break;
        }
    }

    std::cout << "Good bye." << endl;

	return 0;
}
