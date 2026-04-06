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
