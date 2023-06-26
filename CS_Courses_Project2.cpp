
#include <iostream>
#include <String>
#include <fstream>
#include <sstream>



// Define course object to hold course data
struct Course {
    std::string courseNumber;
    std::string courseName;
    std::string prereqCourseNumber;
    std::string prereqCourseNumber_2;
};

//Define a node to contain a course and provide a pointer to the next node
struct Node {
    Course course;
    Node* left;
    Node* right;

    //Node constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //Node constructor with a course
    Node(Course aCourse):
        Node() {
            this->course = aCourse;
    }
  
};

// The Binary Search Tree class definition
class BST {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    //Constructor 
    BST();
    //Destructor 
    ~BST();
    //Destructor helper function
    void DestroyTree(Node* node);
    //Insert a course into a node
    void Insert(Course course);
    //Print courses in order
    void InOrder();
    //Search for a single course
    Course Search(std::string courseNumber);
};


BST::BST() {
    root = nullptr;
}

BST::~BST() {
    DestroyTree(root);
}

void BST::DestroyTree(Node* node) {
    if (node) {
        DestroyTree(node->left);
        DestroyTree(node->right);
        delete node;
    }
}

//Reference to internal private method 
void BST::InOrder() {
    this->inOrder(root);
}
//Print courses in chronological order using a recursive funciton call
void BST::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        std::cout << node->course.courseNumber << ", " << node->course.courseName << std::endl;
        inOrder(node->right);
    }
}
//Used to insert a new course object into a node 
void BST::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        this->addNode(root, course);
    }
}
//Recursive funtion to search the binary search tree for the next place to insert a new node
void BST::addNode(Node* node, Course course) {
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
}
//reads data from the provided .csv file, loads a single line into a course structure, 
//inserts course structure into a node with the Insert() method
void loadCourses(BST* bst) {
    Course course;

    //using fstream from the std library, open the .csv file 
    std::ifstream input_file;
    input_file.open("course_list.csv");

    //read .csv file line by line if the file is open
    if (input_file.is_open()) {
        std::string file_row;
        while (std::getline(input_file, file_row)) {

            //declare a string for each piece of course data on one line
            std::string course_number;
            std::string course_name;
            std::string pre_course_number;
            std::string pre_course_number_2;

            //Use stringstream to read the string data read from the .csv file
            std::stringstream inputstring(file_row);

            // Use getline to retirve course data with the ',' as a delimeter as a distinguisher
            std::getline(inputstring, course_number, ',');
            std::getline(inputstring, course_name, ',');
            std::getline(inputstring, pre_course_number, ',');
            std::getline(inputstring, pre_course_number_2, ',');
            
            //assign the course data structure with the data read from .csv file 
            course.courseNumber = course_number;
            course.courseName = course_name;
            course.prereqCourseNumber = pre_course_number;
            course.prereqCourseNumber_2 = pre_course_number_2;

            //Insert the newly created course data strcute into a node
            bst->Insert(course);
        }
    }
    //close the file to reduce memory leak
    input_file.close();
}
//Search for a specific course
Course BST::Search(std::string courseNumber) {
    Node* currentNode = root;
    while (currentNode != nullptr) {
        if (currentNode->course.courseNumber == courseNumber) {
            return currentNode->course;
        }
        if (currentNode->course.courseNumber > courseNumber) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
    Course course;
    return course;
}

//Print the the course number, name, and prerequisite(s) of a specific course after the Search() fucntion finds the course input by the user
void printCourse(Course course) {
    std::cout << course.courseNumber << ", ";
    std::cout << course.courseName << " " << std::endl;
    std::cout << "Prerequisites: ";
    
    std::cout << course.prereqCourseNumber << " ";
    std::cout << course.prereqCourseNumber_2;
    std::cout << std::endl;
 
};
//SImple funciton to print the user menu, reduces main() funciton clutter
void displayMenu() {
    std::cout << "-------------------------------------------------------" << std::endl;
    std::cout << "                Menu" << std::endl;
    std::cout << "  1. Load courses into Data Structure." << std::endl;
    std::cout << "  2. Print course list." << std::endl;
    std::cout << "  3. Display individual course data." << std::endl;
    std::cout << "  9. Exit program" << std::endl;
    std::cout << "-------------------------------------------------------" << std::endl;
}
//INput validation to enure user input is not invlaid character or number
int inputValidation(int choice) {
    while ((choice != 1) && (choice != 2) && (choice != 3) && (choice != 9)) {
        std::cout << choice << " is not a valid option." << std::endl;
        std::cin >> choice;
        while (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Only Integers '1, 2, 3, 9' are valid options" << std::endl;
            std::cin >> choice;
        }
    }
    return choice;
}


int main()
{
    // Create a binary search tree pointer that will hold courses
    BST* bst;
    //Create a BST in heap memory
    bst = new BST();
    //Create a course data strcutre in stack memory
    Course course;
    std::string searchNumber = "";
    std::cout << "Welcome to the course planner." << std::endl;
    std::cout << std::endl;
    int userChoice;
    // Run program as long as user input != 9, chose do while() loop since it will always run once and reduces assigning userChoice before running
    //since it isn't required to allocate that memory since we are always entering the loop once 
    do {
        //Shows menu options to user
        displayMenu();
        std::cin >> userChoice;

        //Input validation
        userChoice = inputValidation(userChoice);

        // Program options based on user input
        switch (userChoice) {

        case 1:
            //load the courses into a course structure, create a node, add to BST
            loadCourses(bst);
            std::cout << "Loaded all courses into a Binary Search Tree." << std::endl;
            std::cout << std::endl;
            break;

            //Print course schedule in chronological order using recursive inOrder() funciton to traverse tree
        case 2:
            
            std::cout << "Here is a sample schedule in chronological order:" << std::endl;
            std::cout << std::endl;
            bst->InOrder();
            std::cout << std::endl;
            break;

            //Searches for course based on user input, prints course data
            //NOTE: I was unable to figure out how to force an input string to all lowercase or uppercase to provide this sort
            //of input validation. It only works if it mathces the .csv file format in all caps. I tried to figure this out over a few days and 
            //was unable to get it to work - Daulton
        case 3:
            std::cout << "Please input a course you would like to learn more about in all caps, (EX. 'CSCI301'). ";
            std::cin >> searchNumber;
            course = bst->Search(searchNumber);
            std::cout << std::endl;
            printCourse(course);
            std::cout << std::endl;
            break;

            //Exits program
        case 9:
            std::cout << "Thank you for using the course planner!" << std::endl;
            break;
        }
    } while (userChoice != 9);
    
    return 0;
}



