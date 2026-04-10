#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <regex>
using namespace std;

// Utility functions
class DateUtils {
public:
    static string getCurrentDate() {
        time_t now = time(0);
        tm* localtm = localtime(&now);
        stringstream ss;
        ss << 1900 + localtm->tm_year << "-" 
           << setw(2) << setfill('0') << (1 + localtm->tm_mon) << "-" 
           << setw(2) << setfill('0') << localtm->tm_mday;
        return ss.str();
    }

    static int daysBetween(string startDate, string endDate) {
        // Simple date difference calculation (assuming YYYY-MM-DD format)
        return stoi(endDate.substr(8,2)) - stoi(startDate.substr(8,2));
    }

    static bool isDatePassed(string date) {
        return date < getCurrentDate();
    }

    static string addDays(string startDate, int days) {
        // Simple date addition
        int day = stoi(startDate.substr(8,2)) + days;
        int month = stoi(startDate.substr(5,2));
        int year = stoi(startDate.substr(0,4));
        
        // Handle month boundaries (simplified)
        if (day > 30) {
            day -= 30;
            month++;
        }
        if (month > 12) {
            month = 1;
            year++;
        }
        
        stringstream ss;
        ss << year << "-" 
           << setw(2) << setfill('0') << month << "-" 
           << setw(2) << setfill('0') << day;
        return ss.str();
    }
};
class IDGenerator {
private:
    static int bookCounter;
    static int userCounter;
public:
    static string generateBookID() {
        return "BK" + to_string(++bookCounter);
    }
    
    static string generateUserID() {
        return "USR" + to_string(++userCounter);
    }
};
int IDGenerator::bookCounter = 1000;
int IDGenerator::userCounter = 1000;

// Validation functions
class Validator {
public:
    static bool isValidPhoneNumber(const string& phone) {
        // Check if phone number has 10 digits and starts with 98
        regex pattern("^98[0-9]{8}$");
        return regex_match(phone, pattern);
    }
    
    static bool isValidPrice(double price) {
        return price >= 0;
    }
    
    static bool isValidYear(int year) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;

    return year >= 1800 && year <= currentYear;
}
    static bool isValidCopies(int copies) {
        return copies > 0;
    }
};
// Enums and Constants
enum class UserRole {
    ADMIN,
    LIBRARIAN,
    MEMBER
};

enum class BookStatus {
    AVAILABLE,
    BORROWED,
    RESERVED,
    OVERDUE
};

const double FINE_PER_DAY = 10.0; // RS per day
const int MAX_BORROW_DAYS = 14;
const int MAX_BOOKS_PER_USER = 5;
// Book Class
class Book {
public:
    string bookID;
    string title;
    string author;
    string isbn;
    string category;
    int publicationYear;
    int totalCopies;
    int availableCopies;
    double price; // in RS
    Book(string t, string a, string i, string cat, int year, int copies, double p) {
        bookID = IDGenerator::generateBookID();
        title = t;
        author = a;
        isbn = i;
        category = cat;
        publicationYear = year;
        totalCopies = copies;
        availableCopies = copies;
        price = p;
    }

     void display() const {
        cout << "=========================================" << endl;
        cout << "ID: " << bookID << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << isbn << endl;
        cout << "Category: " << category << endl;
        cout << "Year: " << publicationYear << endl;
        cout << "Available: " << availableCopies << "/" << totalCopies << endl;
        cout << "Price: RS " << fixed << setprecision(2) << price << endl;
        cout << "=========================================" << endl;
    }
    bool isAvailable() const {
        return availableCopies > 0;
    }
};
class User {
public:
    string userID;
    string username;
    string password;
    string email;
    string phone;
    UserRole role;
    double totalFine; // in RS
    bool isActive;
    User(string uname, string pwd, string em, string ph, UserRole r) {
        userID = IDGenerator::generateUserID();
        username = uname;
        password = pwd;
        email = em;
        phone = ph;
        role = r;
        totalFine = 0.0;
        isActive = true;
    }
    bool authenticate(string pwd) const {
        return password == pwd && isActive;
    }
    
    string getRoleString() const {
        switch(role) {
            case UserRole::ADMIN: return "Admin";
            case UserRole::LIBRARIAN: return "Librarian";
            case UserRole::MEMBER: return "Member";
            default: return "Unknown";
        }
    }
    
    void display() const {
        cout << "=========================================" << endl;
        cout << "UserID: " << userID << endl;
        cout << "Username: " << username << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Role: " << getRoleString() << endl;
        cout << "Total Fine: RS " << fixed << setprecision(2) << totalFine << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "=========================================" << endl;
    }
};
// Transaction Class
class Transaction {
public:
    string transactionID;
    string userID;
    string bookID;
    string borrowDate;
    string dueDate;
    string returnDate;
    double fineAmount; // in RS
    bool isReturned;
    
    Transaction(string uid, string bid) {
        static int transCounter = 1;
        transactionID = "TR" + to_string(transCounter++);
        userID = uid;
        bookID = bid;
        borrowDate = DateUtils::getCurrentDate();
        dueDate = DateUtils::addDays(borrowDate, MAX_BORROW_DAYS);
        returnDate = "";
        fineAmount = 0.0;
        isReturned = false;
    }
    void calculateFine() {
        if (!isReturned && DateUtils::isDatePassed(dueDate)) {
            int daysOverdue = DateUtils::daysBetween(dueDate, DateUtils::getCurrentDate());
            fineAmount = daysOverdue * FINE_PER_DAY;
        }
    }
    
    void returnBook() {
        returnDate = DateUtils::getCurrentDate();
        calculateFine();
        isReturned = true;
    }
    
    void display() const {
        cout << "=========================================" << endl;
        cout << "TransactionID: " << transactionID << endl;
        cout << "UserID: " << userID << endl;
        cout << "BookID: " << bookID << endl;
        cout << "Borrow Date: " << borrowDate << endl;
        cout << "Due Date: " << dueDate << endl;
        cout << "Return Date: " << (returnDate.empty() ? "Not Returned" : returnDate) << endl;
        cout << "Fine: RS " << fixed << setprecision(2) << fineAmount << endl;
        cout << "Status: " << (isReturned ? "Returned" : "Borrowed") << endl;
        cout << "=========================================" << endl;
    }
};
// Search Criteria Structure
struct SearchCriteria {
    string title;
    string author;
    int publicationYear;
    string category;
    bool availableOnly;
    
    SearchCriteria() {
        title = "";
        author = "";
        publicationYear = 0;
        category = "";
        availableOnly = false;
    }
};
