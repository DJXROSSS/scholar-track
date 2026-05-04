// ============================================================
// Scholar Track - Backend Module
// student.h — Student class declaration
// ============================================================

#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

// Student class represents a single student record
// Fields match the MySQL schema: student_id, name, age, course, grade, contact
class Student {
private:
    int studentId;
    string name;
    int age;
    string course;
    float grade;
    string contact;

public:
    // Constructors
    Student();
    Student(int id, const string& name, int age, const string& course, float grade, const string& contact);

    // Getters
    int getStudentId() const;
    string getName() const;
    int getAge() const;
    string getCourse() const;
    float getGrade() const;
    string getContact() const;

    // Setters
    void setStudentId(int id);
    void setName(const string& name);
    void setAge(int age);
    void setCourse(const string& course);
    void setGrade(float grade);
    void setContact(const string& contact);

    // Display student information
    void display() const;

    // Display as a table row
    void displayRow() const;

    // Serialize to file-friendly string (pipe-delimited)
    string serialize() const;

    // Deserialize from pipe-delimited string
    static Student deserialize(const string& line);

    // Input validation helpers
    static bool isValidId(int id);
    static bool isValidAge(int age);
    static bool isValidGrade(float grade);
    static bool isValidContact(const string& contact);
};

#endif // STUDENT_H
