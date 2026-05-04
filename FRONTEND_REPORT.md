# Scholar Track — Frontend Module Report

> A complete, beginner-friendly explanation of every file, function, and component in the frontend.

---

## Table of Contents

1. [How the Frontend Works (Big Picture)](#1-how-the-frontend-works-big-picture)
2. [File Overview](#2-file-overview)
3. [index.html — The Login Page](#3-indexhtml--the-login-page)
4. [dashboard.html — The Main Application](#4-dashboardhtml--the-main-application)
5. [styles.css — The Design System](#5-stylescss--the-design-system)
6. [app.js — The Brain of the Frontend](#6-appjs--the-brain-of-the-frontend)
7. [How Data is Stored (localStorage)](#7-how-data-is-stored-localstorage)
8. [How the Pages Connect](#8-how-the-pages-connect)

---

## 1. How the Frontend Works (Big Picture)

The frontend is what the user **sees and interacts with** in their browser. Think of it as the "face" of the application.

It is built with three technologies:

| Technology | What it does | Analogy |
|------------|-------------|---------|
| **HTML** | Defines the structure (headings, buttons, tables, forms) | The skeleton of a building |
| **CSS** | Defines the look (colors, spacing, fonts, shadows) | The paint, wallpaper, and decoration |
| **JavaScript** | Defines the behavior (what happens when you click, type, search) | The electricity and plumbing |

Since there is **no real server or database** connected, the frontend uses the browser's built-in storage (`localStorage`) to save and retrieve student data. This means all data lives inside your browser.

---

## 2. File Overview

```
/frontend
├── index.html       ← Login page (the first thing you see)
├── dashboard.html   ← Main app with all 4 modules (Dashboard, Students, Search, Reports)
├── styles.css       ← All visual styling for both pages
└── app.js           ← All logic for the dashboard page
```

---

## 3. index.html — The Login Page

**What it is:** The first page users see. It asks for a username, password, and role before allowing access.

### Structure Breakdown

```
┌──────────────────────────────────────┐
│               🎓 Logo                │
│          "Scholar Track"             │
│    "Student Management System"       │
│                                      │
│   ┌──────────────────────────┐       │
│   │ USERNAME: [____________] │       │
│   │ PASSWORD: [____________] │       │
│   │ ROLE: [Admin] [User]     │       │
│   │ [ Sign In → ]            │       │
│   └──────────────────────────┘       │
│                                      │
│   Demo credentials shown below       │
└──────────────────────────────────────┘
```

### Key HTML Elements

| Element | ID | Purpose |
|---------|-----|---------|
| Username input | `username` | Where user types their username |
| Password input | `password` | Where user types their password |
| Admin toggle button | `roleAdmin` | Selects the Admin role |
| User toggle button | `roleUser` | Selects the User role |
| Login form | `loginForm` | Wraps all inputs; handles the submit event |
| Toast container | `toastContainer` | Shows pop-up messages (success/error) |

### Inline JavaScript (inside `<script>` tag)

Since the login page is simple, its JavaScript is written directly inside the HTML file instead of a separate file.

#### `selectRole(role)`
- **What it does:** Switches between Admin and User role.
- **How it works:** When you click "Admin" or "User", this function highlights the selected button and stores your choice in a variable called `selectedRole`.

#### `showToast(message, type)`
- **What it does:** Shows a small pop-up notification at the top-right corner.
- **Parameters:**
  - `message` — The text to display (e.g., "Login successful!")
  - `type` — The style: `"success"`, `"error"`, or `"info"`
- **How it works:** Creates a `<div>` element, adds it to the toast container, and automatically removes it after 3 seconds.

#### Form Submit Handler (`loginForm` event listener)
- **What it does:** Runs when the user clicks "Sign In".
- **Step-by-step:**
  1. **Prevents page refresh** — `e.preventDefault()` stops the browser from reloading.
  2. **Reads input values** — Gets the username and password from the form fields.
  3. **Validates** — Checks if fields are empty. If so, shows an error by adding the `.error` class.
  4. **Checks credentials** — Compares against hardcoded accounts:
     - Admin: username `admin`, password `admin123`
     - User: username `user`, password `user123`
  5. **On success** — Saves session to `localStorage` and redirects to `dashboard.html`.
  6. **On failure** — Shows an error toast.

#### Auto-redirect Check
- At the bottom of the script, it checks if a user is already logged in (by reading `localStorage`). If yes, it skips the login page and goes straight to the dashboard.

### Login Credentials

| Role | Username | Password |
|------|----------|----------|
| Admin | `admin` | `admin123` |
| User | `user` | `user123` |

---

## 4. dashboard.html — The Main Application

**What it is:** A Single Page Application (SPA) that contains ALL pages of the app. Instead of loading separate HTML files, it shows/hides sections using JavaScript.

### Layout Structure

```
┌─────────────┬──────────────────────────────────────┐
│             │                                      │
│  SIDEBAR    │         MAIN CONTENT AREA            │
│             │                                      │
│  - Dashboard│   (Shows one page at a time:         │
│  - Students │    Dashboard / Students / Search /    │
│  - Search   │    Reports)                          │
│  - Reports  │                                      │
│             │                                      │
│  - Logout   │                                      │
│             │                                      │
└─────────────┴──────────────────────────────────────┘
```

### Component Breakdown

#### A. Sidebar (Navigation Menu)
The left panel that stays visible at all times.

| Element | Purpose |
|---------|---------|
| Brand section | Shows "Scholar Track" title and user badge (ADMIN/USER) |
| Nav buttons | Four buttons: Dashboard, Students, Search, Reports. Each has `data-page` attribute. |
| Logout button | Clears session and redirects to login page |

#### B. Dashboard Page (`page-dashboard`)
The home screen showing an overview of all student data.

| Component | ID | What it shows |
|-----------|----|--------------|
| Stat Card 1 | `statTotal` | Total number of students |
| Stat Card 2 | `statCourses` | Number of unique courses |
| Stat Card 3 | `statAvgGrade` | Average grade across all students |
| Stat Card 4 | `statTopGrade` | Highest grade among all students |
| Recent Table | `recentStudentsTable` | Last 5 students added (newest first) |

#### C. Students Page (`page-students`)
Where you add, update, delete, and view students. Uses **tabs** to switch between operations.

| Tab | ID | What it does |
|-----|----|-------------|
| ➕ Add | `tab-add` | Form with 6 fields to add a new student |
| ✏️ Update | `tab-update` | Enter a student ID, load their data, edit and save |
| 🗑️ Delete | `tab-delete` | Enter a student ID and delete them (with confirmation) |
| 📋 View All | `tab-view` | Table showing every student in the system |

**Add Student Form Fields:**

| Field | Input ID | Validation Rules |
|-------|----------|-----------------|
| Student ID | `addId` | Must be positive, must not already exist |
| Full Name | `addName` | At least 2 characters |
| Age | `addAge` | Between 16 and 60 |
| Course | `addCourse` | Cannot be empty |
| Grade | `addGrade` | Between 0.0 and 10.0 |
| Contact | `addContact` | Exactly 10 digits |

#### D. Search Page (`page-search`)
A search bar with a dropdown to filter by ID, Name, or Course.

| Element | ID | Purpose |
|---------|-----|---------|
| Filter dropdown | `searchType` | Choose: By ID, By Name, or By Course |
| Search input | `searchInput` | Type the search term here |
| Results table | `searchResultsTable` | Shows matching students |
| Empty state | `searchEmpty` | Shows "Search for Students" when no results |

#### E. Reports Page (`page-reports`)
Displays analytics and statistics in three tables.

| Table | ID | Content |
|-------|----|---------|
| Course Distribution | `courseReportTable` | Each course, student count, and average grade |
| Top Performers | `topPerformersTable` | Top 5 students by grade with rank medals |
| Full Report | `fullReportTable` | Complete table of all students |

#### F. Delete Confirmation Modal
A popup that appears when you try to delete a student, asking "Are you sure?".

| Element | ID | Purpose |
|---------|-----|---------|
| Overlay | `deleteModal` | Dark semi-transparent background |
| Message | `deleteModalText` | Shows which student will be deleted |
| Cancel button | — | Closes the modal without deleting |
| Confirm button | `confirmDeleteBtn` | Actually deletes the student |

---

## 5. styles.css — The Design System

**What it is:** A single CSS file that controls the visual appearance of the entire frontend. It uses a minimal grayscale color palette.

### Color Palette

| Variable | Hex Code | Usage |
|----------|----------|-------|
| `--gray-50` | `#F8F9FA` | Page background, lightest surfaces |
| `--gray-100` | `#E9ECEF` | Card backgrounds, hover states |
| `--gray-200` | `#DEE2E6` | Subtle borders |
| `--gray-300` | `#CED4DA` | Scrollbar thumbs |
| `--gray-400` | `#ADB5BD` | Muted/placeholder text |
| `--gray-500` | `#6C757D` | Secondary text |
| `--gray-600` | `#495057` | Emphasis text |
| `--gray-700` | `#343A40` | Strong text, dark buttons |
| `--gray-800` | `#212529` | Primary text, darkest elements |

### Key CSS Sections

| Section | What it styles |
|---------|---------------|
| Reset & Base | Removes default browser spacing, sets base font |
| Glass card (`.glass`) | White card with subtle border and shadow |
| Login wrapper | Centers the login card vertically and horizontally |
| Form elements | Input fields, labels, select dropdowns, error states |
| Buttons | Primary (dark), accent (gray), ghost (transparent border), danger |
| Role toggle | Two side-by-side buttons for Admin/User selection |
| Sidebar | Fixed left panel with navigation links |
| Stats grid | Responsive grid of stat cards on the dashboard |
| Tables | Styled headers, row hover effects, grade badges |
| Tabs | Underlined tab buttons for switching views |
| Search bar | Input + dropdown + buttons in a row |
| Toast notifications | Pop-up messages that slide in and fade out |
| Modal | Centered popup with dark overlay |
| Animations | Fade-in, slide-up, and slide-in-right effects |
| Responsive rules | Adjusts layout for tablets and phones |

### How Responsive Design Works

```
Desktop (> 768px):     Sidebar visible, 4-column stat grid
Tablet (≤ 768px):      Sidebar hidden (toggle button), 2-column grid
Phone (≤ 480px):       Single column grid
```

---

## 6. app.js — The Brain of the Frontend

**What it is:** A single JavaScript file containing ALL the logic for the dashboard. It's organized into 10 numbered sections.

### Section 1: Session Management

These functions handle who is logged in.

| Function | What it does |
|----------|-------------|
| `getSession()` | Reads the logged-in user data from `localStorage`. Returns an object like `{ username: "admin", role: "admin", loggedIn: true }` or `null` if no one is logged in. |
| `requireAuth()` | Checks if the user is logged in. If not, it redirects them back to the login page. This runs when the dashboard loads to block unauthorized access. |
| `logout()` | Removes the session from `localStorage` and sends the user back to the login page. |

### Section 2: Mock Data

Since there's no real database, this section creates fake student data.

| Item | What it is |
|------|-----------|
| `SEED_DATA` | An array of 10 student objects. Each has: `student_id`, `name`, `age`, `course`, `grade`, `contact`. This data is loaded into `localStorage` the very first time someone visits the app. |
| `getStudents()` | Reads all students from `localStorage`. If none exist yet, it seeds the default 10 students. |
| `saveStudents(students)` | Saves the entire students array back to `localStorage`. Called after every add, update, or delete. |
| `findStudent(id)` | Searches the students array for one with a matching `student_id`. Returns the student object or `null`. |

### Section 3: Navigation

| Function | What it does |
|----------|-------------|
| `showPage(page)` | Switches which page is visible. It: (1) hides all pages, (2) shows the target page, (3) updates sidebar highlighting, (4) refreshes data on the shown page, (5) closes mobile sidebar. |
| `toggleSidebar()` | Opens or closes the sidebar on mobile screens. |

### Section 4: Tab Switching

| Function | What it does |
|----------|-------------|
| `switchTab(tab)` | Switches between Add/Update/Delete/View All tabs on the Students page. Highlights the active tab button and shows the corresponding content panel. |

### Section 5: Student Management (CRUD)

CRUD stands for **C**reate, **R**ead, **U**pdate, **D**elete — the four basic operations.

#### `addStudent(e)` — Create
1. Checks if user is Admin (only admins can add).
2. Reads all 6 form fields.
3. Validates every field using `validateStudentInput()`.
4. Checks if the ID already exists (no duplicates allowed).
5. Adds the new student to the array and saves to `localStorage`.
6. Shows a success toast and clears the form.

#### `loadStudentForUpdate()` — Read (for editing)
1. Takes the ID from the update search field.
2. Finds the student in the data.
3. If found, fills the update form with the student's current values.
4. If not found, shows an error toast.

#### `updateStudent(e)` — Update
1. Checks if user is Admin.
2. Reads the updated values from the form.
3. Validates all fields.
4. Finds the student in the array and replaces their data.
5. Saves to `localStorage` and shows a success toast.

#### `deleteStudent()` — Delete (step 1: ask confirmation)
1. Checks if user is Admin.
2. Finds the student by ID.
3. Opens the confirmation modal showing the student's name.
4. Stores the ID in `pendingDeleteId` for later.

#### `confirmDelete()` — Delete (step 2: actually delete)
1. Filters out the student with `pendingDeleteId` from the array.
2. Saves the updated array.
3. Shows a success toast and closes the modal.

#### `closeModal()`
- Closes the delete confirmation modal without deleting.

### Section 6: Search Module

| Function | What it does |
|----------|-------------|
| `searchStudents()` | Reads the search type (ID, Name, or Course) and the search term. Filters the students array based on the criteria. For Name and Course, it does **partial matching** (e.g., searching "comp" will find "Computer Science"). Displays results in a table, or shows "No Results Found". |
| `clearSearch()` | Clears the search input and resets the results area to its default state. |

### Section 7: Reports Module

| Function | What it does |
|----------|-------------|
| `refreshReports()` | Builds three reports: (1) **Course distribution** — groups students by course, counts them, calculates average grade. (2) **Top 5 performers** — sorts all students by grade (highest first), takes the top 5, and adds medal emojis. (3) **Full report** — lists every student in a table. |

### Section 8: Dashboard

| Function | What it does |
|----------|-------------|
| `refreshDashboard()` | Calculates and displays: total students, number of unique courses, average grade, and highest grade in the stat cards. Also fills the "Recent Students" table with the last 5 entries. |
| `refreshStudentTable()` | Fills the "View All" tab with every student, or shows an empty state if no data exists. |

### Section 9: UI Helpers

| Function | What it does |
|----------|-------------|
| `studentRow(s)` | Takes a student object and returns an HTML `<tr>` table row string. Used everywhere tables are displayed. |
| `gradeBadge(grade)` | Takes a grade number and returns a colored badge. Grade ≥ 8 = `grade-high`, ≥ 6 = `grade-mid`, < 6 = `grade-low`. |
| `showToast(message, type)` | Creates a pop-up notification. Same as the login page version. |
| `validateStudentInput(...)` | Checks all 6 student fields for correctness. Returns `true` if all valid, `false` (with error toast) if any fail. Validation rules: ID > 0, name ≥ 2 chars, age 16–60, course not empty, grade 0–10, contact is exactly 10 digits. |

### Section 10: Initialization

| Function | What it does |
|----------|-------------|
| `DOMContentLoaded` listener | Runs once when the page finishes loading. It: (1) checks authentication, (2) sets the user badge and welcome message, (3) hides admin-only tabs if user is not admin, (4) connects the delete confirmation button, (5) adds Enter key support for search, (6) loads the dashboard. |
| `switchTabDirect(tab)` | Same as `switchTab` but without needing a click event. Used during initialization to set the default tab for non-admin users. |

---

## 7. How Data is Stored (localStorage)

`localStorage` is a built-in browser feature that lets websites save small amounts of data that persists even after closing the browser.

### Keys Used

| Key | What it stores | Example |
|-----|---------------|---------|
| `scholartrack_user` | Current logged-in user info | `{"username":"admin","role":"admin","loggedIn":true}` |
| `scholartrack_students` | All student records | `[{"student_id":1001,"name":"Aarav Sharma",...}, ...]` |

### How CRUD Works with localStorage

```
ADD:    getStudents() → push new student → saveStudents()
UPDATE: getStudents() → find by ID → replace data → saveStudents()
DELETE: getStudents() → filter out by ID → saveStudents()
READ:   getStudents() → display in tables
```

> **Important:** This data is specific to your browser. If you open the app in a different browser or clear your browser data, the students will reset to the default 10 records.

---

## 8. How the Pages Connect

```
index.html                          dashboard.html
┌──────────┐    successful login    ┌──────────────────────┐
│          │ ───────────────────► │                      │
│  LOGIN   │    (saves session     │  DASHBOARD APP       │
│  PAGE    │     to localStorage)  │  (reads session      │
│          │                       │   from localStorage) │
│          │ ◄─────────────────── │                      │
└──────────┘    logout / no auth   └──────────────────────┘
```

**Flow:**
1. User opens `index.html` → sees login form.
2. User enters credentials → JS validates → saves session to `localStorage` → redirects to `dashboard.html`.
3. `dashboard.html` loads → `app.js` runs → checks `localStorage` for session → if not found, sends back to login.
4. If session exists, loads dashboard data and shows the app.
5. User clicks "Logout" → `localStorage` session is removed → redirected to `index.html`.

---

*End of Frontend Report*
