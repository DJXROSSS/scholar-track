// ============================================================
// Scholar Track - Backend Module
// student.cpp — Student class implementation
// ============================================================

#include "student.h"
#include <sstream>
#include <regex>

// ---- Constructors ----

Student::Student()
    : studentId(0), name(""), age(0), course(""), grade(0.0f), contact("") {}

Student::Student(int id, const string& name, int age, const string& course, float grade, const string& contact)
    : studentId(id), name(name), age(age), course(course), grade(grade), contact(contact) {}

// ---- Getters ----

int Student::getStudentId() const { return studentId; }
string Student::getName() const { return name; }
int Student::getAge() const { return age; }
string Student::getCourse() const { return course; }
float Student::getGrade() const { return grade; }
string Student::getContact() const { return contact; }

// ---- Setters ----

void Student::setStudentId(int id) { studentId = id; }
void Student::setName(const string& name) { this->name = name; }
void Student::setAge(int age) { this->age = age; }
void Student::setCourse(const string& course) { this->course = course; }
void Student::setGrade(float grade) { this->grade = grade; }
void Student::setContact(const string& contact) { this->contact = contact; }

// ---- Display Methods ----

// Display detailed student info (card format)
void Student::display() const {
    cout << "\n  +------------------------------------+" << endl;
    cout << "  | Student ID : " << left << setw(22) << studentId << "|" << endl;
    cout << "  | Name       : " << left << setw(22) << name << "|" << endl;
    cout << "  | Age        : " << left << setw(22) << age << "|" << endl;
    cout << "  | Course     : " << left << setw(22) << course << "|" << endl;
    cout << "  | Grade      : " << left << setw(22) << fixed << setprecision(1) << grade << "|" << endl;
    cout << "  | Contact    : " << left << setw(22) << contact << "|" << endl;
    cout << "  +------------------------------------+" << endl;
}

// Display as a single table row
void Student::displayRow() const {
    cout << "  | " << left << setw(8) << studentId
         << " | " << left << setw(20) << name
         << " | " << left << setw(4) << age
         << " | " << left << setw(20) << course
         << " | " << left << setw(6) << fixed << setprecision(1) << grade
         << " | " << left << setw(14) << contact
         << " |" << endl;
}

// ---- Serialization ----

// Convert student to pipe-delimited string for file storage
string Student::serialize() const {
    stringstream ss;
    ss << studentId << "|" << name << "|" << age << "|" << course << "|"
       << fixed << setprecision(1) << grade << "|" << contact;
    return ss.str();
}

// Parse a pipe-delimited string into a Student object
Student Student::deserialize(const string& line) {
    Student s;
    stringstream ss(line);
    string token;

    // Parse student_id
    if (getline(ss, token, '|')) s.setStudentId(stoi(token));
    // Parse name
    if (getline(ss, token, '|')) s.setName(token);
    // Parse age
    if (getline(ss, token, '|')) s.setAge(stoi(token));
    // Parse course
    if (getline(ss, token, '|')) s.setCourse(token);
    // Parse grade
    if (getline(ss, token, '|')) s.setGrade(stof(token));
    // Parse contact
    if (getline(ss, token, '|')) s.setContact(token);

    return s;
}

// ---- Validation Helpers ----

// Student ID must be a positive integer
bool Student::isValidId(int id) {
    return id > 0;
}

// Age must be between 16 and 60
bool Student::isValidAge(int age) {
    return age >= 16 && age <= 60;
}

// Grade must be between 0.0 and 10.0
bool Student::isValidGrade(float grade) {
    return grade >= 0.0f && grade <= 10.0f;
}

// Contact must be exactly 10 digits
bool Student::isValidContact(const string& contact) {
    if (contact.length() != 10) return false;
    for (char c : contact) {
        if (!isdigit(c)) return false;
    }
    return true;
}
