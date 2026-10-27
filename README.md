# Scholar Track — Student Management System

A 3-tier Student Management System with **Frontend** (HTML/CSS/JS), **Backend** (C++), and **Database** (MySQL queries) implemented as **independent modules**. No direct integration exists between the layers — they are logically consistent but operate separately.

---

## Project Structure

```
/scholar-track
│
├── /frontend                  # Web-based UI (HTML, CSS, JS)
│   ├── index.html             # Login page
│   ├── dashboard.html         # Dashboard + all management pages (SPA)
│   ├── styles.css             # Complete design system & styling
│   └── app.js                 # Core application logic (modular)
│
├── /backend                   # Console-based C++ application
│   ├── main.cpp               # Entry point
│   ├── student.h              # Student class declaration
│   ├── student.cpp            # Student class implementation
│   ├── system.h               # System class declaration
│   └── system.cpp             # System class implementation
│
├── /database                  # MySQL reference queries
│   └── queries.sql            # INSERT, UPDATE, DELETE, SELECT, SEARCH queries
│
└── README.md                  # This file
```

---

## Modules

### 1. Login Module
- **Frontend**: Glassmorphism login card with username/password fields and role toggle (Admin/User). Sessions stored in `localStorage`.
- **Backend**: Console-based authentication with 3 login attempts. Hardcoded credentials: `admin/admin123` (Admin), `user/user123` (User).
- **Database**: N/A (authentication is application-level).

### 2. Student Management Module
- **Frontend**: Tabbed interface with Add, Update, Delete, and View All tabs. Full client-side validation. CRUD operations use `localStorage`.
- **Backend**: Menu-driven CRUD with file-based persistence (`students.dat`). Validates all inputs (ID > 0, age 16–60, grade 0–10, 10-digit contact).
- **Database**: `INSERT`, `UPDATE`, `DELETE`, `SELECT` queries provided.

### 3. Search Module
- **Frontend**: Search bar with dropdown filter (by ID, Name, or Course). Results displayed in a styled table with grade badges.
- **Backend**: Console search with case-insensitive partial matching for name and course.
- **Database**: `SELECT ... WHERE` queries with exact and `LIKE` pattern matching.

### 4. Report Module
- **Frontend**: Course-wise distribution table, Top 5 performers with rank medals, and full student report.
- **Backend**: Generates comprehensive report with summary statistics (total, average grade, age range), course distribution, and top performers.
- **Database**: Aggregation queries (`AVG`, `COUNT`, `GROUP BY`, `ORDER BY`, `LIMIT`).

---

## How to Run

### Frontend
1. Open `frontend/index.html` in any modern web browser.
2. Login with credentials:
   - **Admin**: `admin` / `admin123`
   - **User**: `user` / `user123`
3. Navigate using the sidebar.

### Backend (C++)
Compile and run with any C++ compiler (C++11 or later):

```bash
# Using g++
cd backend
g++ -std=c++11 -o scholar_track main.cpp student.cpp system.cpp
./scholar_track

# Using MSVC (Windows)
cl /EHsc main.cpp student.cpp system.cpp /Fe:scholar_track.exe
scholar_track.exe
```

### Database
- Open `database/queries.sql` in MySQL Workbench or any MySQL client.
- Run queries individually against your existing `Students` table.

---
