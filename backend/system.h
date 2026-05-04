// ============================================================
// Scholar Track - Backend Module
// system.h — System class declaration (manages all operations)
// ============================================================

#ifndef SYSTEM_H
#define SYSTEM_H

#include "student.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

// Enum for user roles
enum Role { ADMIN, USER_ROLE };

// System class manages the entire student management system
// Provides login, CRUD, search, and report functionality
class System {
private:
    vector<Student> students;       // In-memory student records
    string dataFile;                // File path for persistent storage
    bool loggedIn;                  // Login status
    Role currentRole;               // Current user role
    string currentUser;             // Current username

    // File I/O helpers
    void loadFromFile();
    void saveToFile();

    // Find student index by ID (-1 if not found)
    int findStudentIndex(int id) const;

    // Display table header
    void displayTableHeader() const;

    // Display separator line
    void displaySeparator() const;

public:
    // Constructor
    System();

    // ---- Login Module ----
    bool login();
    void logout();
    bool isLoggedIn() const;
    bool isAdmin() const;

    // ---- Student Management Module ----
    void addStudent();
    void updateStudent();
    void deleteStudent();
    void viewAllStudents();

    // ---- Search Module ----
    void searchMenu();
    void searchById();
    void searchByName();
    void searchByCourse();

    // ---- Report Module ----
    void generateReport();

    // ---- Menu System ----
    void showMainMenu();
    void run();
};

#endif // SYSTEM_H
