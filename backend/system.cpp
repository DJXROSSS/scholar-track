// ============================================================
// Scholar Track - Backend Module
// system.cpp — System class implementation
// ============================================================

#include "system.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <map>

using namespace std;

// ============================================================
// Constructor — initializes the system and loads data from file
// ============================================================
System::System() : dataFile("students.dat"), loggedIn(false), currentRole(USER_ROLE), currentUser("") {
    loadFromFile();
}

// ============================================================
// FILE I/O HELPERS
// ============================================================

// Load student records from the data file into memory
void System::loadFromFile() {
    students.clear();
    ifstream fin(dataFile);
    if (!fin.is_open()) return; // File doesn't exist yet — no data

    string line;
    while (getline(fin, line)) {
        if (!line.empty()) {
            students.push_back(Student::deserialize(line));
        }
    }
    fin.close();
}

// Save all in-memory student records to the data file
void System::saveToFile() {
    ofstream fout(dataFile, ios::trunc);
    if (!fout.is_open()) {
        cerr << "  [ERROR] Unable to open data file for writing." << endl;
        return;
    }
    for (const auto& s : students) {
        fout << s.serialize() << "\n";
    }
    fout.close();
}

// Find a student's index by ID; returns -1 if not found
int System::findStudentIndex(int id) const {
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].getStudentId() == id) return i;
    }
    return -1;
}

// ============================================================
// DISPLAY HELPERS
// ============================================================

void System::displayTableHeader() const {
    displaySeparator();
    cout << "  | " << left << setw(8) << "ID"
         << " | " << left << setw(20) << "Name"
         << " | " << left << setw(4) << "Age"
         << " | " << left << setw(20) << "Course"
         << " | " << left << setw(6) << "Grade"
         << " | " << left << setw(14) << "Contact"
         << " |" << endl;
    displaySeparator();
}

void System::displaySeparator() const {
    cout << "  +----------+----------------------+------+----------------------+--------+----------------+" << endl;
}

// ============================================================
// LOGIN MODULE
// ============================================================

// Authenticate user with username/password and role selection
bool System::login() {
    // Hardcoded credentials for simulation
    // Admin: admin/admin123, User: user/user123
    string username, password;
    int roleChoice;

    cout << "\n  ========================================" << endl;
    cout << "         SCHOLAR TRACK - LOGIN" << endl;
    cout << "  ========================================" << endl;

    cout << "  Username: ";
    cin >> username;
    cout << "  Password: ";
    cin >> password;

    cout << "\n  Select Role:" << endl;
    cout << "  1. Admin" << endl;
    cout << "  2. User" << endl;
    cout << "  Choice: ";
    cin >> roleChoice;

    // Validate credentials
    if (roleChoice == 1) {
        if (username == "admin" && password == "admin123") {
            loggedIn = true;
            currentRole = ADMIN;
            currentUser = username;
            cout << "\n  [SUCCESS] Logged in as Admin." << endl;
            return true;
        }
    } else if (roleChoice == 2) {
        if (username == "user" && password == "user123") {
            loggedIn = true;
            currentRole = USER_ROLE;
            currentUser = username;
            cout << "\n  [SUCCESS] Logged in as User." << endl;
            return true;
        }
    }

    cout << "\n  [ERROR] Invalid credentials or role." << endl;
    return false;
}

void System::logout() {
    loggedIn = false;
    currentUser = "";
    cout << "\n  [INFO] Logged out successfully." << endl;
}

bool System::isLoggedIn() const { return loggedIn; }
bool System::isAdmin() const { return currentRole == ADMIN; }

// ============================================================
// STUDENT MANAGEMENT MODULE
// ============================================================

// Add a new student record (Admin only)
void System::addStudent() {
    if (!isAdmin()) {
        cout << "\n  [ACCESS DENIED] Only Admins can add students." << endl;
        return;
    }

    int id, age;
    string name, course, contact;
    float grade;

    cout << "\n  ---- Add New Student ----" << endl;

    // Get and validate Student ID
    cout << "  Enter Student ID: ";
    cin >> id;
    if (!Student::isValidId(id)) {
        cout << "  [ERROR] Student ID must be a positive integer." << endl;
        return;
    }
    if (findStudentIndex(id) != -1) {
        cout << "  [ERROR] A student with ID " << id << " already exists." << endl;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Get Name
    cout << "  Enter Name: ";
    getline(cin, name);
    if (name.empty()) {
        cout << "  [ERROR] Name cannot be empty." << endl;
        return;
    }

    // Get and validate Age
    cout << "  Enter Age: ";
    cin >> age;
    if (!Student::isValidAge(age)) {
        cout << "  [ERROR] Age must be between 16 and 60." << endl;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Get Course
    cout << "  Enter Course: ";
    getline(cin, course);
    if (course.empty()) {
        cout << "  [ERROR] Course cannot be empty." << endl;
        return;
    }

    // Get and validate Grade
    cout << "  Enter Grade (0.0 - 10.0): ";
    cin >> grade;
    if (!Student::isValidGrade(grade)) {
        cout << "  [ERROR] Grade must be between 0.0 and 10.0." << endl;
        return;
    }

    // Get and validate Contact
    cout << "  Enter Contact (10 digits): ";
    cin >> contact;
    if (!Student::isValidContact(contact)) {
        cout << "  [ERROR] Contact must be exactly 10 digits." << endl;
        return;
    }

    // Create and store the student
    Student newStudent(id, name, age, course, grade, contact);
    students.push_back(newStudent);
    saveToFile();

    cout << "\n  [SUCCESS] Student added successfully!" << endl;
    newStudent.display();
}

// Update an existing student record (Admin only)
void System::updateStudent() {
    if (!isAdmin()) {
        cout << "\n  [ACCESS DENIED] Only Admins can update students." << endl;
        return;
    }

    int id;
    cout << "\n  ---- Update Student ----" << endl;
    cout << "  Enter Student ID to update: ";
    cin >> id;

    int index = findStudentIndex(id);
    if (index == -1) {
        cout << "  [ERROR] Student with ID " << id << " not found." << endl;
        return;
    }

    cout << "\n  Current record:" << endl;
    students[index].display();

    int choice;
    cout << "\n  What would you like to update?" << endl;
    cout << "  1. Name" << endl;
    cout << "  2. Age" << endl;
    cout << "  3. Course" << endl;
    cout << "  4. Grade" << endl;
    cout << "  5. Contact" << endl;
    cout << "  6. All fields" << endl;
    cout << "  Choice: ";
    cin >> choice;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
        case 1: {
            string name;
            cout << "  New Name: ";
            getline(cin, name);
            if (!name.empty()) students[index].setName(name);
            break;
        }
        case 2: {
            int age;
            cout << "  New Age: ";
            cin >> age;
            if (Student::isValidAge(age)) students[index].setAge(age);
            else cout << "  [ERROR] Invalid age." << endl;
            break;
        }
        case 3: {
            string course;
            cout << "  New Course: ";
            getline(cin, course);
            if (!course.empty()) students[index].setCourse(course);
            break;
        }
        case 4: {
            float grade;
            cout << "  New Grade: ";
            cin >> grade;
            if (Student::isValidGrade(grade)) students[index].setGrade(grade);
            else cout << "  [ERROR] Invalid grade." << endl;
            break;
        }
        case 5: {
            string contact;
            cout << "  New Contact: ";
            cin >> contact;
            if (Student::isValidContact(contact)) students[index].setContact(contact);
            else cout << "  [ERROR] Invalid contact." << endl;
            break;
        }
        case 6: {
            string name, course, contact;
            int age;
            float grade;

            cout << "  New Name: ";
            getline(cin, name);
            cout << "  New Age: ";
            cin >> age;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  New Course: ";
            getline(cin, course);
            cout << "  New Grade: ";
            cin >> grade;
            cout << "  New Contact: ";
            cin >> contact;

            if (!name.empty()) students[index].setName(name);
            if (Student::isValidAge(age)) students[index].setAge(age);
            if (!course.empty()) students[index].setCourse(course);
            if (Student::isValidGrade(grade)) students[index].setGrade(grade);
            if (Student::isValidContact(contact)) students[index].setContact(contact);
            break;
        }
        default:
            cout << "  [ERROR] Invalid choice." << endl;
            return;
    }

    saveToFile();
    cout << "\n  [SUCCESS] Student updated successfully!" << endl;
    students[index].display();
}

// Delete a student record (Admin only)
void System::deleteStudent() {
    if (!isAdmin()) {
        cout << "\n  [ACCESS DENIED] Only Admins can delete students." << endl;
        return;
    }

    int id;
    cout << "\n  ---- Delete Student ----" << endl;
    cout << "  Enter Student ID to delete: ";
    cin >> id;

    int index = findStudentIndex(id);
    if (index == -1) {
        cout << "  [ERROR] Student with ID " << id << " not found." << endl;
        return;
    }

    cout << "\n  Student to be deleted:" << endl;
    students[index].display();

    char confirm;
    cout << "\n  Are you sure? (y/n): ";
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        students.erase(students.begin() + index);
        saveToFile();
        cout << "  [SUCCESS] Student deleted successfully." << endl;
    } else {
        cout << "  [INFO] Deletion cancelled." << endl;
    }
}

// View all student records (accessible to all roles)
void System::viewAllStudents() {
    if (students.empty()) {
        cout << "\n  [INFO] No student records found." << endl;
        return;
    }

    cout << "\n  ---- All Students (" << students.size() << " records) ----" << endl;
    displayTableHeader();
    for (const auto& s : students) {
        s.displayRow();
    }
    displaySeparator();
}

// ============================================================
// SEARCH MODULE
// ============================================================

void System::searchMenu() {
    int choice;
    cout << "\n  ---- Search Students ----" << endl;
    cout << "  1. Search by ID" << endl;
    cout << "  2. Search by Name" << endl;
    cout << "  3. Search by Course" << endl;
    cout << "  0. Back" << endl;
    cout << "  Choice: ";
    cin >> choice;

    switch (choice) {
        case 1: searchById(); break;
        case 2: searchByName(); break;
        case 3: searchByCourse(); break;
        case 0: break;
        default: cout << "  [ERROR] Invalid choice." << endl;
    }
}

void System::searchById() {
    int id;
    cout << "  Enter Student ID: ";
    cin >> id;

    int index = findStudentIndex(id);
    if (index != -1) {
        cout << "\n  [FOUND] Student record:" << endl;
        students[index].display();
    } else {
        cout << "  [NOT FOUND] No student with ID " << id << "." << endl;
    }
}

void System::searchByName() {
    string name;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Enter Name (or partial): ";
    getline(cin, name);

    // Convert search term to lowercase for case-insensitive matching
    string lowerName = name;
    transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    bool found = false;
    cout << endl;
    displayTableHeader();
    for (const auto& s : students) {
        string sName = s.getName();
        transform(sName.begin(), sName.end(), sName.begin(), ::tolower);
        if (sName.find(lowerName) != string::npos) {
            s.displayRow();
            found = true;
        }
    }
    displaySeparator();

    if (!found) {
        cout << "  [NOT FOUND] No students matching '" << name << "'." << endl;
    }
}

void System::searchByCourse() {
    string course;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Enter Course (or partial): ";
    getline(cin, course);

    string lowerCourse = course;
    transform(lowerCourse.begin(), lowerCourse.end(), lowerCourse.begin(), ::tolower);

    bool found = false;
    cout << endl;
    displayTableHeader();
    for (const auto& s : students) {
        string sCourse = s.getCourse();
        transform(sCourse.begin(), sCourse.end(), sCourse.begin(), ::tolower);
        if (sCourse.find(lowerCourse) != string::npos) {
            s.displayRow();
            found = true;
        }
    }
    displaySeparator();

    if (!found) {
        cout << "  [NOT FOUND] No students in course '" << course << "'." << endl;
    }
}

// ============================================================
// REPORT MODULE
// ============================================================

void System::generateReport() {
    if (students.empty()) {
        cout << "\n  [INFO] No data to generate report." << endl;
        return;
    }

    cout << "\n  ============================================" << endl;
    cout << "          SCHOLAR TRACK - STUDENT REPORT" << endl;
    cout << "  ============================================" << endl;

    // 1. Full student list
    cout << "\n  >> All Students:" << endl;
    displayTableHeader();
    for (const auto& s : students) {
        s.displayRow();
    }
    displaySeparator();

    // 2. Summary statistics
    cout << "\n  >> Summary Statistics:" << endl;
    cout << "  Total Students : " << students.size() << endl;

    float totalGrade = 0;
    int minAge = 999, maxAge = 0;
    for (const auto& s : students) {
        totalGrade += s.getGrade();
        if (s.getAge() < minAge) minAge = s.getAge();
        if (s.getAge() > maxAge) maxAge = s.getAge();
    }
    cout << "  Average Grade  : " << fixed << setprecision(2) << (totalGrade / students.size()) << endl;
    cout << "  Age Range      : " << minAge << " - " << maxAge << endl;

    // 3. Course-wise distribution
    map<string, int> courseCount;
    map<string, float> courseGradeSum;
    for (const auto& s : students) {
        courseCount[s.getCourse()]++;
        courseGradeSum[s.getCourse()] += s.getGrade();
    }

    cout << "\n  >> Course-wise Distribution:" << endl;
    cout << "  +----------------------+-------+-------------+" << endl;
    cout << "  | " << left << setw(20) << "Course"
         << " | " << setw(5) << "Count"
         << " | " << setw(11) << "Avg Grade"
         << " |" << endl;
    cout << "  +----------------------+-------+-------------+" << endl;
    for (const auto& pair : courseCount) {
        float avg = courseGradeSum[pair.first] / pair.second;
        cout << "  | " << left << setw(20) << pair.first
             << " | " << setw(5) << pair.second
             << " | " << setw(11) << fixed << setprecision(2) << avg
             << " |" << endl;
    }
    cout << "  +----------------------+-------+-------------+" << endl;

    // 4. Top performers
    vector<Student> sorted = students;
    sort(sorted.begin(), sorted.end(), [](const Student& a, const Student& b) {
        return a.getGrade() > b.getGrade();
    });

    int topN = min(5, (int)sorted.size());
    cout << "\n  >> Top " << topN << " Performers:" << endl;
    displayTableHeader();
    for (int i = 0; i < topN; i++) {
        sorted[i].displayRow();
    }
    displaySeparator();

    cout << "\n  ============================================" << endl;
    cout << "          END OF REPORT" << endl;
    cout << "  ============================================" << endl;
}

// ============================================================
// MENU SYSTEM
// ============================================================

void System::showMainMenu() {
    cout << "\n  ========================================" << endl;
    cout << "       SCHOLAR TRACK - MAIN MENU" << endl;
    cout << "  ========================================" << endl;
    cout << "  Logged in as: " << currentUser << " (" << (isAdmin() ? "Admin" : "User") << ")" << endl;
    cout << "  ----------------------------------------" << endl;
    cout << "  1. View All Students" << endl;
    cout << "  2. Search Students" << endl;
    cout << "  3. Generate Report" << endl;

    if (isAdmin()) {
        cout << "  4. Add Student" << endl;
        cout << "  5. Update Student" << endl;
        cout << "  6. Delete Student" << endl;
    }

    cout << "  0. Logout & Exit" << endl;
    cout << "  ========================================" << endl;
    cout << "  Choice: ";
}

// Main application loop
void System::run() {
    cout << "\n  ******************************************" << endl;
    cout << "  *                                        *" << endl;
    cout << "  *         SCHOLAR TRACK v1.0             *" << endl;
    cout << "  *     Student Management System          *" << endl;
    cout << "  *                                        *" << endl;
    cout << "  ******************************************" << endl;

    // Login loop — allow 3 attempts
    int attempts = 0;
    while (!loggedIn && attempts < 3) {
        if (!login()) {
            attempts++;
            cout << "  Attempts remaining: " << (3 - attempts) << endl;
        }
    }

    if (!loggedIn) {
        cout << "\n  [ERROR] Too many failed attempts. Exiting." << endl;
        return;
    }

    // Main menu loop
    int choice;
    do {
        showMainMenu();
        cin >> choice;

        // Handle invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [ERROR] Please enter a valid number." << endl;
            continue;
        }

        switch (choice) {
            case 1: viewAllStudents(); break;
            case 2: searchMenu(); break;
            case 3: generateReport(); break;
            case 4:
                if (isAdmin()) addStudent();
                else cout << "  [ACCESS DENIED] Admin only." << endl;
                break;
            case 5:
                if (isAdmin()) updateStudent();
                else cout << "  [ACCESS DENIED] Admin only." << endl;
                break;
            case 6:
                if (isAdmin()) deleteStudent();
                else cout << "  [ACCESS DENIED] Admin only." << endl;
                break;
            case 0:
                logout();
                break;
            default:
                cout << "  [ERROR] Invalid option." << endl;
        }
    } while (choice != 0);

    cout << "\n  Thank you for using Scholar Track!" << endl;
    cout << "  ******************************************\n" << endl;
}
