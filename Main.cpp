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