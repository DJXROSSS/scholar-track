# Scholar Track — Backend Module Report

> A complete, beginner-friendly explanation of every file, class, function, and component in the C++ backend.

---

## Table of Contents

1. [How the Backend Works (Big Picture)](#1-how-the-backend-works-big-picture)
2. [File Overview](#2-file-overview)
3. [What are Header (.h) and Source (.cpp) Files?](#3-what-are-header-h-and-source-cpp-files)
4. [student.h — Student Class Declaration](#4-studenth--student-class-declaration)
5. [student.cpp — Student Class Implementation](#5-studentcpp--student-class-implementation)
6. [system.h — System Class Declaration](#6-systemh--system-class-declaration)
7. [system.cpp — System Class Implementation](#7-systemcpp--system-class-implementation)
8. [main.cpp — The Entry Point](#8-maincpp--the-entry-point)
9. [How Data is Stored (File Handling)](#9-how-data-is-stored-file-handling)
10. [How to Compile and Run](#10-how-to-compile-and-run)
11. [Program Flow (Step by Step)](#11-program-flow-step-by-step)

---

## 1. How the Backend Works (Big Picture)

The backend is a **console application** — it runs in a terminal/command prompt window, not in a browser. It uses text menus and keyboard input instead of buttons and forms.

It does the same things as the frontend:
- Login with username/password
- Add, update, delete, view students
- Search by ID, name, or course
- Generate reports

But instead of `localStorage`, it saves data to a **text file** on your computer called `students.dat`.

### Why OOP (Object-Oriented Programming)?

The backend uses two **classes**:

| Class | What it represents | Analogy |
|-------|-------------------|---------|
| `Student` | A single student record | A single index card with one student's info |
| `System` | The entire management system | The filing cabinet + the person managing it |

A **class** is like a blueprint. It defines what data something holds and what actions it can perform. An **object** is an actual instance of that blueprint.

---

## 2. File Overview

```
/backend
├── main.cpp       ← Entry point (starts the program)
├── student.h      ← Student class blueprint (what a Student looks like)
├── student.cpp    ← Student class logic (how a Student works)
├── system.h       ← System class blueprint (what the System looks like)
└── system.cpp     ← System class logic (how the System works)
```

---

## 3. What are Header (.h) and Source (.cpp) Files?

In C++, code is split into two types of files:

| File Type | Extension | Purpose | Analogy |
|-----------|-----------|---------|---------|
| **Header** | `.h` | Declares what exists (function names, variables) | A restaurant menu (lists what's available) |
| **Source** | `.cpp` | Defines how things work (actual code logic) | The kitchen (where the food is actually made) |

**Why split?** It keeps code organized. Other files can read the header to know what's available without needing to see all the implementation details.

The `#include "student.h"` line in other files means "read the Student header and make its declarations available here."

---

## 4. student.h — Student Class Declaration

**What it is:** The blueprint for a Student object. It lists everything a Student has (data) and can do (functions), but doesn't say *how* things are done.

### Private Members (Data)

These are the student's properties. They are **private**, meaning only the Student class itself can directly read or change them. This is called **encapsulation** — it protects data from being accidentally modified.

| Member | Type | What it stores | Database equivalent |
|--------|------|---------------|-------------------|
| `studentId` | `int` | Unique student number | `student_id INT PRIMARY KEY` |
| `name` | `string` | Full name | `name VARCHAR` |
| `age` | `int` | Age in years | `age INT` |
| `course` | `string` | Course name | `course VARCHAR` |
| `grade` | `float` | Grade (0.0 to 10.0) | `grade FLOAT` |
| `contact` | `string` | 10-digit phone number | `contact VARCHAR` |

### Public Members (Functions)

These are declared in the header and implemented in `student.cpp`.

#### Constructors
- `Student()` — Creates an empty student with default values (ID = 0, name = "", etc.).
- `Student(id, name, age, course, grade, contact)` — Creates a student with all fields filled in.

#### Getters (read data)
Functions like `getStudentId()`, `getName()`, `getAge()`, etc. Since the data is private, these functions provide **read-only access** to the outside world.

#### Setters (write data)
Functions like `setStudentId(id)`, `setName(name)`, `setAge(age)`, etc. These allow controlled modification of private data.

#### Display Functions
- `display()` — Shows student info in a detailed card format.
- `displayRow()` — Shows student info as a single table row.

#### Serialization
- `serialize()` — Converts the student to a text string for saving to a file.
- `deserialize(line)` — Converts a text string back into a Student object. This is `static`, meaning you call it on the class itself (`Student::deserialize(...)`) rather than on an object.

#### Validation Helpers
- `isValidId(id)` — Checks if ID is positive.
- `isValidAge(age)` — Checks if age is between 16 and 60.
- `isValidGrade(grade)` — Checks if grade is between 0.0 and 10.0.
- `isValidContact(contact)` — Checks if contact is exactly 10 digits.

These are `static` functions — they don't need a Student object to work. You call them like `Student::isValidAge(25)`.

---

## 5. student.cpp — Student Class Implementation

**What it is:** The actual code for every function declared in `student.h`.

### Constructors

```cpp
Student::Student()
    : studentId(0), name(""), age(0), course(""), grade(0.0f), contact("") {}
```
The `: ...` syntax is called an **initializer list**. It sets all member variables to their initial values when a Student is created.

### Getters & Setters

These are straightforward one-liners. Example:
```cpp
int Student::getStudentId() const { return studentId; }    // getter
void Student::setStudentId(int id) { studentId = id; }     // setter
```
The `const` keyword on getters means "this function promises not to change any data."

### `display()` — Card Format

Prints the student info inside a bordered box:
```
  +------------------------------------+
  | Student ID : 1001                  |
  | Name       : Aarav Sharma         |
  | Age        : 20                    |
  | Course     : Computer Science     |
  | Grade      : 9.2                   |
  | Contact    : 9876543210           |
  +------------------------------------+
```
Uses `setw()` (set width) and `left` to align text neatly in columns.

### `displayRow()` — Table Row

Prints one row in a formatted table:
```
  | 1001     | Aarav Sharma         | 20   | Computer Science     | 9.2    | 9876543210     |
```

### `serialize()` — Convert to Text

Converts a Student into a single line of text using `|` (pipe) as a separator:
```
1001|Aarav Sharma|20|Computer Science|9.2|9876543210
```
This format is used to save students to the `students.dat` file.

### `deserialize(line)` — Convert from Text

Does the reverse — takes a pipe-separated string and builds a Student object from it. Uses `getline(ss, token, '|')` to split the string at each `|` character.

### Validation Functions

| Function | Rule | Example |
|----------|------|---------|
| `isValidId(id)` | `id > 0` | `isValidId(1001)` → `true` |
| `isValidAge(age)` | `16 ≤ age ≤ 60` | `isValidAge(15)` → `false` |
| `isValidGrade(grade)` | `0.0 ≤ grade ≤ 10.0` | `isValidGrade(8.5)` → `true` |
| `isValidContact(contact)` | Exactly 10 digits | `isValidContact("abc")` → `false` |

---

## 6. system.h — System Class Declaration

**What it is:** The blueprint for the System class, which manages the entire application.

### Private Members

| Member | Type | What it stores |
|--------|------|---------------|
| `students` | `vector<Student>` | A resizable list of all Student objects in memory |
| `dataFile` | `string` | The filename used for storage (`"students.dat"`) |
| `loggedIn` | `bool` | Whether someone is currently logged in |
| `currentRole` | `Role` enum | Either `ADMIN` or `USER_ROLE` |
| `currentUser` | `string` | The username of the logged-in person |

### `Role` Enum

```cpp
enum Role { ADMIN, USER_ROLE };
```
An **enum** is a custom type with named values. Instead of using confusing numbers like `0` and `1`, we use meaningful names `ADMIN` and `USER_ROLE`.

### Private Helper Functions

| Function | Purpose |
|----------|---------|
| `loadFromFile()` | Reads `students.dat` and fills the `students` vector |
| `saveToFile()` | Writes all students from the vector back to `students.dat` |
| `findStudentIndex(id)` | Returns the position of a student in the vector (or -1 if not found) |
| `displayTableHeader()` | Prints the header row of a table |
| `displaySeparator()` | Prints a horizontal line of dashes |

### Public Functions (grouped by module)

| Module | Functions |
|--------|----------|
| **Login** | `login()`, `logout()`, `isLoggedIn()`, `isAdmin()` |
| **Student Management** | `addStudent()`, `updateStudent()`, `deleteStudent()`, `viewAllStudents()` |
| **Search** | `searchMenu()`, `searchById()`, `searchByName()`, `searchByCourse()` |
| **Reports** | `generateReport()` |
| **Menu** | `showMainMenu()`, `run()` |

---

## 7. system.cpp — System Class Implementation

**What it is:** The largest file in the backend. Contains all the actual logic for every System function.

### Constructor

```cpp
System::System() : dataFile("students.dat"), loggedIn(false), currentRole(USER_ROLE), currentUser("") {
    loadFromFile();
}
```
When a System object is created, it:
1. Sets the data file name to `students.dat`.
2. Sets login state to `false`.
3. Calls `loadFromFile()` to load any previously saved students.

---

### File I/O Functions

#### `loadFromFile()`
1. Opens `students.dat` for reading.
2. Reads each line one by one.
3. Converts each line to a Student object using `Student::deserialize()`.
4. Adds each Student to the `students` vector.
5. If the file doesn't exist, it does nothing (empty database).

#### `saveToFile()`
1. Opens `students.dat` for writing (overwrites existing content).
2. Converts each Student to a text line using `serialize()`.
3. Writes each line to the file.

#### `findStudentIndex(id)`
Loops through the `students` vector comparing each student's ID. Returns the index (position) if found, or `-1` if no match.

---

### Login Module

#### `login()`

**Step-by-step:**
1. Displays a login banner.
2. Asks for username and password.
3. Asks user to select role (1 = Admin, 2 = User).
4. Checks credentials:
   - Admin: `admin` / `admin123`
   - User: `user` / `user123`
5. If correct: sets `loggedIn = true`, stores role and username, prints success.
6. If wrong: prints error message, returns `false`.

#### `logout()`
Sets `loggedIn = false` and clears `currentUser`.

#### `isLoggedIn()` / `isAdmin()`
Simple checks that return `true` or `false`.

---

### Student Management Module

#### `addStudent()`

1. **Access check** — Only admins can add. If not admin, prints "ACCESS DENIED".
2. **Get Student ID** — Reads from keyboard, validates (must be positive, must not already exist).
3. **Get Name** — Uses `getline()` to read full names with spaces.
4. **Get Age** — Validates between 16 and 60.
5. **Get Course** — Uses `getline()` for course names with spaces.
6. **Get Grade** — Validates between 0.0 and 10.0.
7. **Get Contact** — Validates exactly 10 digits.
8. **Save** — Creates a Student object, adds to vector, calls `saveToFile()`.
9. **Confirm** — Prints success and displays the new student's card.

> **Note on `cin.ignore()`**: After reading a number with `cin >>`, there's a leftover newline character in the input buffer. `cin.ignore(numeric_limits<streamsize>::max(), '\n')` clears it out so the next `getline()` works correctly. This is a common C++ pattern.

#### `updateStudent()`

1. **Access check** — Admin only.
2. **Find student** — Asks for ID, looks it up.
3. **Show current data** — Displays the student's current record.
4. **Choose what to update** — Menu with options: Name, Age, Course, Grade, Contact, or All.
5. **Read new values** — Validates each one.
6. **Save** — Updates the student in the vector and saves to file.

#### `deleteStudent()`

1. **Access check** — Admin only.
2. **Find student** — Asks for ID, looks it up.
3. **Show student** — Displays who will be deleted.
4. **Confirm** — Asks "Are you sure? (y/n)".
5. **Delete** — Uses `students.erase()` to remove the student from the vector.
6. **Save** — Writes updated data to file.

#### `viewAllStudents()`

1. Checks if the list is empty.
2. Prints the table header.
3. Loops through all students, printing each as a table row.

---

### Search Module

#### `searchMenu()`
Shows a sub-menu with 3 search options and calls the appropriate function.

#### `searchById()`
1. Asks for a Student ID.
2. Uses `findStudentIndex()` to locate it.
3. Displays the student card if found, or "NOT FOUND" message.

#### `searchByName()`
1. Asks for a name (or partial name).
2. Converts search term to **lowercase** for case-insensitive matching.
3. Loops through all students, converting each name to lowercase.
4. Uses `.find()` to check if the search term appears anywhere in the name.
5. Displays all matches in a table.

#### `searchByCourse()`
Same logic as `searchByName()` but checks the `course` field instead of `name`.

---

### Report Module

#### `generateReport()`

Generates a comprehensive report with 4 sections:

**Section 1: Full Student List**
- Displays every student in a formatted table.

**Section 2: Summary Statistics**
- Total students (size of the vector).
- Average grade (sum of all grades ÷ count).
- Age range (youngest and oldest).

**Section 3: Course-wise Distribution**
- Uses two `map` containers:
  - `courseCount` — how many students per course.
  - `courseGradeSum` — total grade per course (for computing average).
- Displays a table with Course, Count, and Average Grade.

> A `map` is like a dictionary — you look up a key (course name) and get a value (count or grade sum).

**Section 4: Top 5 Performers**
- Copies the students vector.
- Sorts the copy by grade (highest first) using a **lambda function**:
  ```cpp
  sort(sorted.begin(), sorted.end(), [](const Student& a, const Student& b) {
      return a.getGrade() > b.getGrade();
  });
  ```
- Displays the top 5 (or fewer if less than 5 students exist).

---

### Menu System

#### `showMainMenu()`
Prints the menu options. Shows options 1–3 for all users, and options 4–6 (Add, Update, Delete) only for admins.

```
  ========================================
       SCHOLAR TRACK - MAIN MENU
  ========================================
  Logged in as: admin (Admin)
  ----------------------------------------
  1. View All Students
  2. Search Students
  3. Generate Report
  4. Add Student          ← Admin only
  5. Update Student       ← Admin only
  6. Delete Student       ← Admin only
  0. Logout & Exit
  ========================================
```

#### `run()` — The Main Loop

This is the heart of the program. It controls the entire flow:

1. **Print welcome banner.**
2. **Login loop** — Allows up to 3 login attempts. If all fail, the program exits.
3. **Main menu loop** — Repeatedly shows the menu and processes user choices:
   - `1` → `viewAllStudents()`
   - `2` → `searchMenu()`
   - `3` → `generateReport()`
   - `4` → `addStudent()` (admin check)
   - `5` → `updateStudent()` (admin check)
   - `6` → `deleteStudent()` (admin check)
   - `0` → `logout()` and exit loop
4. **Prints goodbye message.**

The `do { ... } while (choice != 0)` structure means the menu keeps appearing after each action until the user chooses `0`.

**Input error handling:**
```cpp
if (cin.fail()) {
    cin.clear();                                        // reset error flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
}
```
If the user types letters instead of a number, this prevents the program from crashing.

---

## 8. main.cpp — The Entry Point

```cpp
#include "system.h"

int main() {
    System scholarTrack;   // Create the system (loads data from file)
    scholarTrack.run();    // Start the program
    return 0;              // Exit with success code
}
```

This is the **simplest file** in the project. Every C++ program starts at `main()`. It creates one `System` object and calls its `run()` method, which handles everything else.

---

## 9. How Data is Stored (File Handling)

### The Data File: `students.dat`

This file is created automatically in the same folder where you run the program. Each line represents one student, with fields separated by `|` (pipe character):

```
1001|Aarav Sharma|20|Computer Science|9.2|9876543210
1002|Priya Patel|21|Mathematics|8.7|9876543211
1003|Rohan Gupta|19|Physics|7.5|9876543212
```

### How CRUD Maps to File Operations

| Operation | What happens in memory | What happens on disk |
|-----------|----------------------|---------------------|
| **Load** | `loadFromFile()` reads file → fills `vector` | File is read line by line |
| **Add** | New Student pushed to `vector` | Entire file is rewritten with all students |
| **Update** | Student at index is modified in `vector` | Entire file is rewritten |
| **Delete** | Student is erased from `vector` | Entire file is rewritten |

> The file is always rewritten completely (not edited in-place) because text files don't support editing individual lines easily.

---

## 10. How to Compile and Run

### Using g++ (GCC) — Windows/Mac/Linux

```bash
cd backend
g++ -std=c++11 -o scholar_track main.cpp student.cpp system.cpp
./scholar_track
```

**What this means:**
- `g++` — The C++ compiler
- `-std=c++11` — Use C++11 standard (needed for lambdas and `auto`)
- `-o scholar_track` — Name the output program `scholar_track`
- `main.cpp student.cpp system.cpp` — The three source files to compile
- `./scholar_track` — Run the compiled program

### Using MSVC (Visual Studio) — Windows

```bash
cl /EHsc main.cpp student.cpp system.cpp /Fe:scholar_track.exe
scholar_track.exe
```

### What You'll See

```
  ******************************************
  *                                        *
  *         SCHOLAR TRACK v1.0             *
  *     Student Management System          *
  *                                        *
  ******************************************

  ========================================
         SCHOLAR TRACK - LOGIN
  ========================================
  Username: admin
  Password: admin123

  Select Role:
  1. Admin
  2. User
  Choice: 1

  [SUCCESS] Logged in as Admin.
```

---

## 11. Program Flow (Step by Step)

```
main()
  │
  ├── System() constructor
  │     └── loadFromFile()     ← Load saved data
  │
  └── run()
        │
        ├── Print welcome banner
        │
        ├── Login loop (max 3 attempts)
        │     └── login()
        │           ├── Read username, password, role
        │           └── Validate credentials
        │
        └── Main menu loop
              │
              ├── showMainMenu()
              ├── Read user choice
              │
              ├── 1: viewAllStudents()
              │
              ├── 2: searchMenu()
              │     ├── 1: searchById()
              │     ├── 2: searchByName()
              │     └── 3: searchByCourse()
              │
              ├── 3: generateReport()
              │
              ├── 4: addStudent()        [Admin]
              │     ├── Read 6 fields
              │     ├── Validate each
              │     ├── Add to vector
              │     └── saveToFile()
              │
              ├── 5: updateStudent()     [Admin]
              │     ├── Find by ID
              │     ├── Read new values
              │     └── saveToFile()
              │
              ├── 6: deleteStudent()     [Admin]
              │     ├── Find by ID
              │     ├── Confirm (y/n)
              │     ├── Erase from vector
              │     └── saveToFile()
              │
              └── 0: logout() → exit loop
```

---

### Key C++ Concepts Used

| Concept | Where it's used | What it means |
|---------|----------------|--------------|
| **Class** | `Student`, `System` | A blueprint for objects with data and functions |
| **Encapsulation** | `private:` members | Hiding data inside a class for protection |
| **Constructor** | `Student()`, `System()` | Special function that runs when an object is created |
| **Static functions** | `Student::isValidAge()` | Functions that belong to the class, not to any specific object |
| **Vector** | `vector<Student>` | A resizable list/array from the C++ Standard Library |
| **Map** | `map<string, int>` | A key-value dictionary |
| **Enum** | `enum Role` | A custom type with named constants |
| **Lambda** | `[](...){}` in sort | A small unnamed function written inline |
| **File I/O** | `ifstream`, `ofstream` | Reading from and writing to files |
| **String streams** | `stringstream` | Treating a string as a stream for parsing |
| **`const`** | `getStudentId() const` | Promise that a function won't change data |

---

*End of Backend Report*
