// MIT103 Assessment 2 - Personal Finance Tracker

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <queue>
#include <regex> // For regex validation
#include <algorithm> // For std::sort

using namespace std;

//////////////////////////////////////////////////////
// PART 1: PROGRAM SETUP AND USER INTERFACE
//////////////////////////////////////////////////////

// Function to display the main menu
void showMenu() {
    cout << "\n========== Personal Finance Tracker ==========\n";
    cout << "1. Add Transaction\n";
    cout << "2. Delete Transaction\n";
    cout << "3. Search Transaction by Category\n";
    cout << "4. Display All Transactions\n";
    cout << "5. Sort Transactions by Amount\n";
    cout << "6. Display Recent Transactions\n"; // Changed menu option
    cout << "7. Exit\n";
    cout << "Choose an option: ";
}

//////////////////////////////////////////////////////
// PART 2: DATA HANDLING
//////////////////////////////////////////////////////

// Class to represent a financial transaction
class Transaction {
private:
    string date;
    string category;
    string description;
    float amount;

public:
    // Constructor to initialize a transaction
    Transaction(string d, string c, string desc, float a)
        : date(d), category(c), description(desc), amount(a) {}

    // Accessor methods
    string getDate() const { return date; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    float getAmount() const { return amount; }

    // Method to print transaction details in a formatted way
    void display() const {
        cout << setw(12) << date
             << setw(12) << category
             << setw(20) << description
             << setw(10) << fixed << setprecision(2) << amount << endl;
    }
};

// Vector to store multiple transactions (dynamic array) - PRIMARY DATA STORE
vector<Transaction> transactions;

// Map to store transactions by category for fast searching
unordered_map<string, vector<Transaction>> categoryMap;

// Queue to store recent transactions
queue<Transaction> recentTransactions;

/////////////////////////////////////////////////////////
//// Implementing File Handling for Data Persistence ////
/////////////////////////////////////////////////////////

// Forward declarations for functions that rebuild auxiliary data structures
void rebuildCategoryMap();
void rebuildRecentTransactionsQueue();

// Function to save transactions to a file
void saveTransactionsToFile() {
    ofstream outFile("transactions.txt");

    if (!outFile) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    for (const auto& t : transactions) {
        outFile << t.getDate() << "," << t.getCategory() << "," << t.getDescription() << "," << t.getAmount() << "\n";
    }

    outFile.close();
    cout << "Transactions saved successfully.\n";
}

// Function to load transactions from a file
void loadTransactionsFromFile() {
    ifstream inFile("transactions.txt");

    if (!inFile) {
        cerr << "Error opening file for reading (file might not exist, creating a new one).\n";
        // It's okay if the file doesn't exist, it means no transactions yet.
        return;
    }

    transactions.clear(); // Clear existing transactions before loading
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string date_str, category_str, description_str, amount_str;

        if (getline(ss, date_str, ',') &&
            getline(ss, category_str, ',') &&
            getline(ss, description_str, ',') &&
            getline(ss, amount_str)) {
            
            try {
                float amount_val = stof(amount_str); // Convert string to float
                transactions.emplace_back(date_str, category_str, description_str, amount_val);
            } catch (const invalid_argument& e) {
                cerr << "Warning: Invalid amount '" << amount_str << "' in file. Skipping line.\n";
            } catch (const out_of_range& e) {
                cerr << "Warning: Amount '" << amount_str << "' out of range. Skipping line.\n";
            }
        } else {
            cerr << "Warning: Malformed line in file: " << line << ". Skipping.\n";
        }
    }

    inFile.close();
    cout << "Transactions loaded successfully.\n";

    // Rebuild auxiliary data structures after loading
    rebuildCategoryMap();
    rebuildRecentTransactionsQueue();
}

//////////////////////////////////////////////////////
// PART 3: SORTING AND SEARCHING
//////////////////////////////////////////////////////

// Sort transactions by amount using Bubble Sort
void sortTransactions() {
    // Using std::sort for efficiency and simplicity,
    // as Bubble Sort is generally not recommended for practical applications.
    // If Bubble Sort is strictly required for assessment, replace std::sort with the loop.
    // For large datasets, std::sort is significantly faster.
    std::sort(transactions.begin(), transactions.end(), [](const Transaction& a, const Transaction& b) {
        return a.getAmount() < b.getAmount();
    });
    cout << "Transactions sorted by amount.\n";
}

// Rebuilds the categoryMap from the current 'transactions' vector
void rebuildCategoryMap() {
    categoryMap.clear(); // Clear existing map
    for (const auto& t : transactions) {
        categoryMap[t.getCategory()].push_back(t);
    }
}

// Search transactions by category using the categoryMap (hash table)
void searchTransactionByCategory() {
    string category;
    cout << "Enter category to search: ";
    cin >> category;

    cout << "\nSearch Results for Category '" << category << "':\n";
    cout << setw(12) << "Date" << setw(12) << "Category"
         << setw(20) << "Description" << setw(10) << "Amount" << endl;

    bool found = false;
    if (categoryMap.count(category)) { // Check if category exists in map
        for (const auto& t : categoryMap[category]) {
            t.display();
            found = true;
        }
    }

    if (!found) {
        cout << "No transactions found for category: " << category << endl;
    }
}

// Rebuilds the recentTransactions queue from the current 'transactions' vector
void rebuildRecentTransactionsQueue() {
    // Clear the current queue
    while (!recentTransactions.empty()) {
        recentTransactions.pop();
    }

    // Add up to the last 5 transactions from the 'transactions' vector
    int count = 0;
    // Iterate from the end of the transactions vector to get most recent
    for (auto it = transactions.rbegin(); it != transactions.rend() && count < 5; ++it) {
        // Push to a temporary stack or list first to reverse order for queue,
        // or just accept reverse order in queue, then display in temp queue.
        // For simplicity, directly add to queue and display by popping temp queue later.
        recentTransactions.push(*it);
        count++;
    }
    // Note: The queue will hold the most recent items pushed last.
    // When displaying, you'll need to iterate a copy to preserve the original queue.
}

// Function to add a recent transaction to the queue (called when adding a new transaction)
void addRecentTransaction(const Transaction& t) {
    if (recentTransactions.size() >= 5) { // Limit to last 5 transactions
        recentTransactions.pop(); // Remove the oldest transaction
    }
    recentTransactions.push(t); // Add the new transaction
}

// Function to display recent transactions from the queue
void displayRecentTransactions() {
    if (recentTransactions.empty()) {
        cout << "No recent transactions.\n";
        return;
    }

    cout << "\nRecent Transactions (Last 5):\n";
    cout << setw(12) << "Date" << setw(12) << "Category"
         << setw(20) << "Description" << setw(10) << "Amount" << endl;

    queue<Transaction> tempQueue = recentTransactions; // Copy of queue for display
    vector<Transaction> displayOrder; // To display in chronological order (most recent last)

    while (!tempQueue.empty()) {
        displayOrder.push_back(tempQueue.front());
        tempQueue.pop();
    }

    // Display in reverse order to show most recent first, then older
    for (auto it = displayOrder.rbegin(); it != displayOrder.rend(); ++it) {
        it->display();
    }
}

/////////////////////////////////////////////////////////
//// SHA-256 Hashing Implementation (Simple Mock) //////
/////////////////////////////////////////////////////////

// Function to hash a password (using a simple alternative hashing method for demonstration)
string hashPassword(const string &password) {
    stringstream ss;
    for (char c : password) {
        ss << hex << (int)c;
    }
    return ss.str();
}

// Store user credentials (username -> hashed password)
unordered_map<string, string> usersDB;
unordered_map<string, string> rolesDB; // Map usernames to roles (admin or user)

// Function to register users
void registerUser() {
    string username, password, role;
    cout << "Enter username: ";
    cin >> username;

    // Check if username already exists
    if (usersDB.count(username)) {
        cout << "Username already exists. Please choose a different one.\n";
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    string hashedPassword = hashPassword(password);

    cout << "Enter role (admin/user): ";
    cin >> role;
    
    // Basic validation for role
    while (role != "admin" && role != "user") {
        cout << "Invalid role. Please enter 'admin' or 'user': ";
        cin >> role;
    }

    usersDB[username] = hashedPassword;
    rolesDB[username] = role;

    cout << "User registered successfully.\n";
}

// Function to authenticate user
bool authenticateUser(string &currentUser) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;

    cout << "Enter password: ";
    cin >> password;

    string hashedPassword = hashPassword(password);

    if (usersDB.count(username) && usersDB[username] == hashedPassword) {
        currentUser = username;
        cout << "Authentication successful.\n";
        return true;
    } else {
        cout << "Authentication failed. Invalid username or password.\n";
        return false;
    }
}

// Function to check user role
string getUserRole(const string &username) {
    if (rolesDB.count(username)) {
        return rolesDB[username];
    }
    return "unknown"; // Should not happen if authenticated
}

//////////////////////////////////////////////////////
// PART 4: MODULARISING CODE WITH FUNCTIONS
//////////////////////////////////////////////////////

// Input validation helper functions
bool isValidDate(const string &date) {
    regex dateRegex(R"(^\d{2}/\d{2}/\d{4}$)");
    return regex_match(date, dateRegex);
}

bool isValidAmount(float amount) {
    return amount > 0; // The amount must be positive
}

// Function to add a new transaction (with input validation)
void addTransaction() {
    string date, category, description;
    float amount;

    cout << "Enter date (DD/MM/YYYY): ";
    cin >> date;
    while (!isValidDate(date)) {
        cout << "Invalid date format. Please use DD/MM/YYYY: ";
        cin >> date;
    }

    cout << "Enter category: ";
    cin >> category;
    while (category.empty()) {
        cout << "Category cannot be empty. Please enter a valid category: ";
        cin >> category;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clears the input buffer before getline
    cout << "Enter description: ";
    getline(cin, description);
    while (description.empty()) {
        cout << "Description cannot be empty. Please enter a valid description: ";
        getline(cin, description);
    }

    cout << "Enter amount: ";
    cin >> amount;
    while (cin.fail() || !isValidAmount(amount)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid amount. Enter a positive numeric value: ";
        cin >> amount;
    }

    // Create the new transaction
    Transaction newTransaction(date, category, description, amount);

    // Add to primary vector
    transactions.push_back(newTransaction);
    cout << "Transaction added successfully!\n";

    // Update auxiliary data structures
    addRecentTransaction(newTransaction); // Add to queue
    categoryMap[newTransaction.getCategory()].push_back(newTransaction); // Add to map
}

// Function to delete a transaction by date
void deleteTransaction() {
    string date_to_delete;
    cout << "Enter date of transaction to delete (DD/MM/YYYY): ";
    cin >> date_to_delete;

    bool found = false;
    // Use an iterator to safely erase elements
    for (auto it = transactions.begin(); it != transactions.end(); ) {
        if (it->getDate() == date_to_delete) {
            it = transactions.erase(it); // erase returns iterator to the next element
            found = true;
            cout << "Transaction on " << date_to_delete << " deleted.\n";
        } else {
            ++it;
        }
    }

    if (!found) {
        cout << "Transaction on " << date_to_delete << " not found.\n";
    } else {
        // Rebuild auxiliary data structures after deletion for consistency
        rebuildCategoryMap();
        rebuildRecentTransactionsQueue(); // Rebuilds from current state of 'transactions'
    }
}

// Function to display all transactions
void displayTransactions() {
    if (transactions.empty()) {
        cout << "No transactions to display.\n";
        return;
    }

    cout << "\nAll Transactions:\n";
    cout << setw(12) << "Date" << setw(12) << "Category"
         << setw(20) << "Description" << setw(10) << "Amount" << endl;

    for (const auto& t : transactions) {
        t.display();
    }
}

//////////////////////////////////////////////////////
// PART 6: DEBUGGING AND ERROR HANDLING (Test Function)
//////////////////////////////////////////////////////

// Function to run test cases and simulate operations
void runTests() {
    cout << "\n=== Running Test Cases ===\n";
    transactions.clear(); // Start with a clean slate
    categoryMap.clear();
    while(!recentTransactions.empty()) recentTransactions.pop();

    // Add sample transactions for testing
    // Using the addTransaction logic to populate all structures
    cout << "\nAdding test transactions...\n";
    // Simulate user input for addTransaction
    // To avoid actual cin input for tests, we'll manually add and then rebuild
    transactions.emplace_back("01/05/2025", "Food", "Lunch at cafe", 12.50);
    transactions.emplace_back("03/05/2025", "Transport", "Bus fare", 3.00);
    transactions.emplace_back("05/05/2025", "Rent", "Monthly rent", 500.00);
    transactions.emplace_back("02/05/2025", "Food", "Dinner", 25.75);
    transactions.emplace_back("04/05/2025", "Utilities", "Electricity bill", 75.00);
    transactions.emplace_back("06/05/2025", "Entertainment", "Movie ticket", 15.00);
    
    rebuildCategoryMap();
    rebuildRecentTransactionsQueue();

    // Test 1: Display all transactions
    cout << "\nTest 1: Display Transactions\n";
    displayTransactions();

    // Test 2: Search by category ('Food')
    cout << "\nTest 2: Search for 'Food'\n";
    // Directly call search function with a category, or modify to simulate input
    // Simulating input for direct call
    cout << "Simulating search for 'Food'...\n";
    if (categoryMap.count("Food")) {
        cout << "\nSearch Results for Category 'Food':\n";
        cout << setw(12) << "Date" << setw(12) << "Category"
             << setw(20) << "Description" << setw(10) << "Amount" << endl;
        for (const auto& t : categoryMap["Food"]) {
            t.display();
        }
    } else {
        cout << "No transactions found for category: Food\n";
    }

    // Test 3: Sort transactions by amount
    cout << "\nTest 3: Sort by Amount\n";
    sortTransactions();
    displayTransactions();

    // Test 4: Delete transaction by date ('03/05/2025')
    cout << "\nTest 4: Delete Transaction on '03/05/2025'\n";
    // Simulate deletion
    string test_date_to_delete = "03/05/2025";
    bool deleted_in_test = false;
    for (auto it = transactions.begin(); it != transactions.end(); ) {
        if (it->getDate() == test_date_to_delete) {
            it = transactions.erase(it);
            deleted_in_test = true;
        } else {
            ++it;
        }
    }
    if (deleted_in_test) {
        cout << "Transaction on '" << test_date_to_delete << "' deleted.\n";
        rebuildCategoryMap();
        rebuildRecentTransactionsQueue();
    } else {
        cout << "Transaction on '" << test_date_to_delete << "' not found.\n";
    }
    displayTransactions();

    // Test 5: Display Recent Transactions
    cout << "\nTest 5: Display Recent Transactions\n";
    displayRecentTransactions();
    
    cout << "\n=== Test Cases Finished ===\n";
}

// Main function: program entry point
int main() {
    int choice;
    string currentUser;

    // Initialize with a default user for demonstration if needed, or enforce registration/login
    // For this demonstration, we'll register one user at start.
    cout << "Welcome to the Personal Finance Tracker!\n";
    registerUser(); 

    bool authenticated = false;
    while (!authenticated) {
        authenticated = authenticateUser(currentUser);
        if (authenticated) {
            cout << "Welcome " << currentUser << "!\n";
        }
    }

    // Load transactions from file at startup
    loadTransactionsFromFile();

    // Main menu loop
    do {
        showMenu();
        cin >> choice;

        while (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
            cin >> choice;
        }

        switch (choice) {
            case 1:
                addTransaction(); // Add a new transaction (updates vector, map, queue)
                break;
            case 2: 
                deleteTransaction(); // Delete a transaction (updates vector, rebuilds map/queue)
                break;
            case 3: 
                searchTransactionByCategory(); // Search using the category map
                break;
            case 4: 
                displayTransactions(); // Display all from the main vector
                break;
            case 5: 
                sortTransactions(); // Sorts the main vector
                displayTransactions(); // Display the sorted vector
                break;
            case 6:
                displayRecentTransactions(); // Display recent from the queue
                break;
            case 7: 
                cout << "Exiting program. Goodbye!\n"; 
                break;
            case 8: // Hidden option for running tests
                runTests();
                break;
            default: 
                cout << "Invalid option. Try again.\n";
        }
    } while (choice != 7);

    // Save transactions to file before exiting
    saveTransactionsToFile();

    return 0;
}
