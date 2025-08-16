/*
 * CS 300 Project Two: Course Advising Program
 * Author: DALE Davenport
 * 
 * This program implements a course advising system using a Binary Search Tree
 * to store and manage course information. It allows users to load course data
 * from a CSV file, display courses in alphanumeric order, and search for
 * specific course details including prerequisites.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

/**
 * Course structure to hold course information
 */
struct Course {
    string courseId;
    string courseTitle;
    vector<string> prerequisites;
    
    // Default constructor
    Course() {}
    
    // Parameterized constructor
    Course(string id, string title) {
        courseId = id;
        courseTitle = title;
    }
};

/**
 * Node structure for Binary Search Tree
 */
struct Node {
    Course course;
    Node* left;
    Node* right;
    
    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }
    
    // Parameterized constructor
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

/**
 * Binary Search Tree class for managing courses
 */
class BinarySearchTree {
private:
    Node* root;
    
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void postOrder(Node* node);
    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize housekeeping variables
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // Recurse from root deleting every node
    PostOrder();
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // Call inOrder function and pass root
    this->inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // Call postOrder function and pass root
    this->postOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // If root equal to null ptr
    if (root == nullptr) {
        // Root is equal to new node course
        root = new Node(course);
    }
    // Else
    else {
        // Add Node root and course
        this->addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    // Remove node root courseId
    this->removeNode(root, courseId);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // Set current node equal to root
    Node* current = root;

    // Keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // If match found, return current course
        if (current->course.courseId.compare(courseId) == 0) {
            return current->course;
        }
        // If course is smaller than current node then traverse left
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }
        // Else larger so traverse right
        else {
            current = current->right;
        }
    }

    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // If node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {
        // If no left node
        if (node->left == nullptr) {
            // This node becomes left
            node->left = new Node(course);
        }
        // Else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    // Else
    else {
        // If no right node
        if (node->right == nullptr) {
            // This node becomes right
            node->right = new Node(course);
        }
        // Else
        else {
            // Recurse down the left node
            this->addNode(node->right, course);
        }
    }
}

/**
 * Print courses in order (recursive)
 */
void BinarySearchTree::inOrder(Node* node) {
    // If node is not equal to null ptr
    if (node != nullptr) {
        // InOrder left
        inOrder(node->left);
        
        // Output courseId, title
        cout << node->course.courseId << ", " << node->course.courseTitle << endl;
        
        // InOrder right
        inOrder(node->right);
    }
}

/**
 * Post order deletion (recursive)
 */
void BinarySearchTree::postOrder(Node* node) {
    // If node is not equal to null ptr
    if (node != nullptr) {
        // PostOrder left
        postOrder(node->left);
        
        // PostOrder right
        postOrder(node->right);
        
        // Delete node
        delete node;
    }
}

/**
 * Remove node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    // If node is null, return node
    if (node == nullptr) {
        return node;
    }
    
    // Recurse down the left subtree
    if (courseId.compare(node->course.courseId) < 0) {
        node->left = removeNode(node->left, courseId);
    }
    // Recurse down the right subtree
    else if (courseId.compare(node->course.courseId) > 0) {
        node->right = removeNode(node->right, courseId);
    }
    // Match found
    else {
        // No children so this is a leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // One child to the left
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        // One child to the right
        else if (node->right != nullptr && node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        // Two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseId);
        }
    }
    return node;
}

/**
 * Display the course information to the console (courseId, title, prerequisites)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseId << ", " << course.courseTitle;
    if (!course.prerequisites.empty()) {
        cout << endl << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites.at(i);
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl;
    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // Initialize the CSV Parser using the given path
    ifstream file(csvPath);
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << csvPath << endl;
        return;
    }

    string line;
    
    // Read each line from the file
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        
        // Parse the line using stringstream
        stringstream ss(line);
        string token;
        vector<string> tokens;
        
        // Split the line by commas
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        
        // Ensure we have at least courseId and title
        if (tokens.size() >= 2) {
            Course course;
            course.courseId = tokens[0];
            course.courseTitle = tokens[1];
            
            // Add prerequisites if they exist
            for (size_t i = 2; i < tokens.size(); ++i) {
                if (!tokens[i].empty()) {
                    course.prerequisites.push_back(tokens[i]);
                }
            }
            
            // Insert course into BST
            bst->Insert(course);
        }
    }
    
    file.close();
    cout << "Courses loaded successfully." << endl;
}

/**
 * Convert string to uppercase for case-insensitive comparison
 */
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst = new BinarySearchTree();

    Course course;
    string csvPath, courseKey;

    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner." << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Complete the method call to load the courses
            cout << "Enter the file name: ";
            cin >> csvPath;
            
            // Initialize a timer variable before loading courses
            ticks = clock();

            // Complete the method call to load the courses
            loadCourses(csvPath, bst);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // Current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            // Call the method to print courses in order
            bst->InOrder();
            cout << endl;
            break;

        case 3:
            cout << "What course do you want to know about? ";
            cin >> courseKey;
            
            // Convert input to uppercase for case-insensitive search
            courseKey = toUpperCase(courseKey);

            course = bst->Search(courseKey);

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }
            cout << endl;
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}

