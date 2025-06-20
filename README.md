# Personal Finance Tracker

## Overview

The **Personal Finance Tracker** is a C++ console application designed to help users track their financial transactions. It allows users to add, delete, search, and sort transactions, as well as view recent transactions. This program is enhanced with file handling for data persistence, basic user authentication, and efficient data structures for improved performance.

## Features

1. **Add Transaction**: Users can add new transactions with details such as date, category, description, and amount.
2. **Delete Transaction**: Allows users to delete transactions by date.
3. **Search Transactions by Category**: Users can search and display transactions by category.
4. **Display All Transactions**: View all transactions stored in the system.
5. **Sort Transactions by Amount**: Sort transactions based on the amount (ascending).
6. **Display Recent Transactions**: Shows the last 5 transactions added to the system.
7. **User Authentication**: Implements a basic user login system with password hashing and role-based access control (RBAC).
8. **Data Persistence**: Transactions are saved to and loaded from a file (`transactions.txt`), ensuring that data persists between sessions.

## Installation

### Requirements

* C++ compiler (e.g., GCC, Clang)
* Standard C++ library (C++11 or higher)
* File system support for saving/loading transaction data

### Clone the repository

```bash
git clone https://github.com/your-username/personal-finance-tracker.git
```

### Compile and Run

To compile and run the application, use the following commands:

```bash
g++ -o PersonalFinanceTracker Assessment3.cpp -std=c++11
./PersonalFinanceTracker
```

## Usage

### Menu Options

1. **Add Transaction**: Add a new financial transaction.
2. **Delete Transaction**: Delete an existing transaction by specifying the date.
3. **Search Transaction by Category**: Find and display transactions by category.
4. **Display All Transactions**: Display all stored transactions.
5. **Sort Transactions by Amount**: Sort transactions by the transaction amount.
6. **Display Recent Transactions**: View the last 5 transactions.
7. **Exit**: Exit the application.

### User Authentication

* **Registration**: Register a new user with a unique username, password, and role (either "admin" or "user").
* **Login**: Authenticate using the registered username and password.

## Features and Enhancements

* **File Handling**: Transactions are saved to `transactions.txt` and can be loaded back into the program upon startup.
* **Security**: Passwords are hashed using SHA-256 for secure storage, and only authenticated users can access transaction data.
* **Efficient Data Storage**: The program uses vectors, queues, and hash maps for managing transactions and categories efficiently.

## How to Contribute

1. Fork the repository
2. Create a new branch (`git checkout -b feature-xyz`)
3. Commit your changes (`git commit -m 'Add new feature'`)
4. Push to the branch (`git push origin feature-xyz`)
5. Create a new Pull Request

## License

This project is open source and available under the MIT License.

## Acknowledgements

* Thanks to [C++ Programming](https://www.cplusplus.com) for providing comprehensive C++ documentation.
* Special thanks to [GitHub](https://github.com) for enabling easy version control and collaboration.
