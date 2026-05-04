// ============================================================
// Scholar Track - Frontend Module
// app.js — Core application logic (modular)
// ============================================================
// This file manages:
//   1. Session / Auth
//   2. Mock Data (localStorage-based CRUD)
//   3. Navigation
//   4. Student Management (Add, Update, Delete, View)
//   5. Search Module
//   6. Report Module
//   7. UI Helpers (Toast, Modal, Grade badges)
// ============================================================

"use strict";

// ============================================================
// 1. SESSION MANAGEMENT
// ============================================================

/** Get current session from localStorage */
function getSession() {
  return JSON.parse(localStorage.getItem("scholartrack_user") || "null");
}

/** Redirect to login if not authenticated */
function requireAuth() {
  const session = getSession();
  if (!session || !session.loggedIn) {
    window.location.href = "index.html";
    return null;
  }
  return session;
}

/** Logout and redirect */
function logout() {
  localStorage.removeItem("scholartrack_user");
  window.location.href = "index.html";
}

// ============================================================
// 2. MOCK DATA — localStorage-backed student store
// ============================================================

// Seed data matching the MySQL schema fields exactly:
// student_id, name, age, course, grade, contact
const SEED_DATA = [
  { student_id: 1001, name: "Aarav Sharma",    age: 20, course: "Computer Science", grade: 9.2, contact: "9876543210" },
  { student_id: 1002, name: "Priya Patel",     age: 21, course: "Mathematics",      grade: 8.7, contact: "9876543211" },
  { student_id: 1003, name: "Rohan Gupta",     age: 19, course: "Physics",          grade: 7.5, contact: "9876543212" },
  { student_id: 1004, name: "Sneha Reddy",     age: 22, course: "Chemistry",        grade: 8.9, contact: "9876543213" },
  { student_id: 1005, name: "Vikram Singh",    age: 20, course: "Computer Science", grade: 9.5, contact: "9876543214" },
  { student_id: 1006, name: "Ananya Iyer",     age: 21, course: "Mathematics",      grade: 9.0, contact: "9876543215" },
  { student_id: 1007, name: "Karan Mehta",     age: 23, course: "Physics",          grade: 6.8, contact: "9876543216" },
  { student_id: 1008, name: "Divya Nair",      age: 20, course: "Data Science",     grade: 8.3, contact: "9876543217" },
  { student_id: 1009, name: "Arjun Verma",     age: 19, course: "Computer Science", grade: 7.9, contact: "9876543218" },
  { student_id: 1010, name: "Meera Joshi",     age: 22, course: "Data Science",     grade: 9.1, contact: "9876543219" },
];

/** Get all students from localStorage */
function getStudents() {
  const data = localStorage.getItem("scholartrack_students");
  if (!data) {
    // Seed with initial mock data on first load
    localStorage.setItem("scholartrack_students", JSON.stringify(SEED_DATA));
    return [...SEED_DATA];
  }
  return JSON.parse(data);
}

/** Save students array to localStorage */
function saveStudents(students) {
  localStorage.setItem("scholartrack_students", JSON.stringify(students));
}

/** Find student by ID */
function findStudent(id) {
  return getStudents().find((s) => s.student_id === id) || null;
}

// ============================================================
// 3. NAVIGATION — SPA-style page switching
// ============================================================

/** Show a page section and update nav state */
function showPage(page) {
  // Hide all pages
  document.querySelectorAll(".page-section").forEach((el) => el.classList.add("hidden"));
  // Show target page
  const target = document.getElementById(`page-${page}`);
  if (target) target.classList.remove("hidden");

  // Update nav active state
  document.querySelectorAll(".nav-item[data-page]").forEach((btn) => {
    btn.classList.toggle("active", btn.dataset.page === page);
  });

  // Refresh data on page show
  if (page === "dashboard") refreshDashboard();
  if (page === "students") refreshStudentTable();
  if (page === "reports") refreshReports();

  // Close mobile sidebar
  document.getElementById("sidebar").classList.remove("open");
}

/** Toggle mobile sidebar */
function toggleSidebar() {
  document.getElementById("sidebar").classList.toggle("open");
}

// ============================================================
// 4. TAB SWITCHING (Student Management)
// ============================================================

function switchTab(tab) {
  // Update tab buttons
  document.querySelectorAll(".tab-btn").forEach((btn) => btn.classList.remove("active"));
  event.currentTarget.classList.add("active");

  // Show/hide tab content
  document.querySelectorAll(".tab-content").forEach((el) => el.classList.remove("active"));
  const target = document.getElementById(`tab-${tab}`);
  if (target) target.classList.add("active");

  if (tab === "view") refreshStudentTable();
}

// ============================================================
// 5. STUDENT MANAGEMENT — CRUD Operations
// ============================================================

/** Add a new student */
function addStudent(e) {
  e.preventDefault();
  const session = getSession();
  if (session.role !== "admin") {
    showToast("Access denied. Admin only.", "error");
    return false;
  }

  const id = parseInt(document.getElementById("addId").value);
  const name = document.getElementById("addName").value.trim();
  const age = parseInt(document.getElementById("addAge").value);
  const course = document.getElementById("addCourse").value.trim();
  const grade = parseFloat(document.getElementById("addGrade").value);
  const contact = document.getElementById("addContact").value.trim();

  // Validation
  if (!validateStudentInput(id, name, age, course, grade, contact)) return false;

  // Check duplicate ID
  if (findStudent(id)) {
    showToast(`Student ID ${id} already exists.`, "error");
    return false;
  }

  const students = getStudents();
  students.push({ student_id: id, name, age, course, grade, contact });
  saveStudents(students);

  showToast(`Student "${name}" added successfully!`, "success");
  document.getElementById("addStudentForm").reset();
  return false;
}

/** Load student data into update form */
function loadStudentForUpdate() {
  const id = parseInt(document.getElementById("updateSearchId").value);
  if (!id || id <= 0) {
    showToast("Enter a valid Student ID.", "error");
    return;
  }

  const student = findStudent(id);
  if (!student) {
    showToast(`Student ID ${id} not found.`, "error");
    document.getElementById("updateStudentForm").classList.add("hidden");
    return;
  }

  // Populate form
  document.getElementById("updName").value = student.name;
  document.getElementById("updAge").value = student.age;
  document.getElementById("updCourse").value = student.course;
  document.getElementById("updGrade").value = student.grade;
  document.getElementById("updContact").value = student.contact;
  document.getElementById("updateStudentForm").classList.remove("hidden");
}

/** Update student record */
function updateStudent(e) {
  e.preventDefault();
  const session = getSession();
  if (session.role !== "admin") {
    showToast("Access denied. Admin only.", "error");
    return false;
  }

  const id = parseInt(document.getElementById("updateSearchId").value);
  const name = document.getElementById("updName").value.trim();
  const age = parseInt(document.getElementById("updAge").value);
  const course = document.getElementById("updCourse").value.trim();
  const grade = parseFloat(document.getElementById("updGrade").value);
  const contact = document.getElementById("updContact").value.trim();

  if (!validateStudentInput(id, name, age, course, grade, contact)) return false;

  const students = getStudents();
  const index = students.findIndex((s) => s.student_id === id);
  if (index === -1) {
    showToast("Student not found.", "error");
    return false;
  }

  students[index] = { student_id: id, name, age, course, grade, contact };
  saveStudents(students);
  showToast(`Student "${name}" updated successfully!`, "success");
  return false;
}

/** Delete student (with confirmation modal) */
let pendingDeleteId = null;

function deleteStudent() {
  const session = getSession();
  if (session.role !== "admin") {
    showToast("Access denied. Admin only.", "error");
    return;
  }

  const id = parseInt(document.getElementById("deleteId").value);
  if (!id || id <= 0) {
    showToast("Enter a valid Student ID.", "error");
    return;
  }

  const student = findStudent(id);
  if (!student) {
    showToast(`Student ID ${id} not found.`, "error");
    return;
  }

  // Show confirmation modal
  pendingDeleteId = id;
  document.getElementById("deleteModalText").textContent =
    `Are you sure you want to delete "${student.name}" (ID: ${id})?`;
  document.getElementById("deleteModal").classList.add("active");
}

function confirmDelete() {
  if (pendingDeleteId === null) return;
  let students = getStudents();
  const name = students.find((s) => s.student_id === pendingDeleteId)?.name;
  students = students.filter((s) => s.student_id !== pendingDeleteId);
  saveStudents(students);
  showToast(`Student "${name}" deleted.`, "success");
  pendingDeleteId = null;
  closeModal();
  document.getElementById("deleteId").value = "";
}

function closeModal() {
  document.getElementById("deleteModal").classList.remove("active");
  pendingDeleteId = null;
}

// ============================================================
// 6. SEARCH MODULE
// ============================================================

function searchStudents() {
  const type = document.getElementById("searchType").value;
  const query = document.getElementById("searchInput").value.trim().toLowerCase();

  if (!query) {
    showToast("Enter a search term.", "error");
    return;
  }

  const students = getStudents();
  let results = [];

  switch (type) {
    case "id":
      const id = parseInt(query);
      results = students.filter((s) => s.student_id === id);
      break;
    case "name":
      results = students.filter((s) => s.name.toLowerCase().includes(query));
      break;
    case "course":
      results = students.filter((s) => s.course.toLowerCase().includes(query));
      break;
  }

  const tbody = document.getElementById("searchResultsTable");
  const empty = document.getElementById("searchEmpty");

  if (results.length === 0) {
    tbody.innerHTML = "";
    empty.innerHTML = `<div class="icon">🚫</div><h3>No Results Found</h3><p>No students match "${query}".</p>`;
    empty.classList.remove("hidden");
  } else {
    empty.classList.add("hidden");
    tbody.innerHTML = results.map((s) => studentRow(s)).join("");
  }
}

function clearSearch() {
  document.getElementById("searchInput").value = "";
  document.getElementById("searchResultsTable").innerHTML = "";
  document.getElementById("searchEmpty").classList.remove("hidden");
  document.getElementById("searchEmpty").innerHTML = `
    <div class="icon">🔍</div><h3>Search for Students</h3>
    <p>Use the search bar above to find student records.</p>`;
}

// ============================================================
// 7. REPORTS MODULE
// ============================================================

function refreshReports() {
  const students = getStudents();

  // Course distribution
  const courseMap = {};
  students.forEach((s) => {
    if (!courseMap[s.course]) courseMap[s.course] = { count: 0, totalGrade: 0 };
    courseMap[s.course].count++;
    courseMap[s.course].totalGrade += s.grade;
  });

  document.getElementById("courseReportTable").innerHTML = Object.entries(courseMap)
    .map(
      ([course, data]) => `
    <tr>
      <td>${course}</td>
      <td>${data.count}</td>
      <td>${gradeBadge((data.totalGrade / data.count).toFixed(1))}</td>
    </tr>`
    )
    .join("");

  // Top performers
  const sorted = [...students].sort((a, b) => b.grade - a.grade).slice(0, 5);
  document.getElementById("topPerformersTable").innerHTML = sorted
    .map(
      (s, i) => `
    <tr>
      <td>${["🥇", "🥈", "🥉", "4️⃣", "5️⃣"][i]}</td>
      <td>${s.student_id}</td>
      <td>${s.name}</td>
      <td>${s.course}</td>
      <td>${gradeBadge(s.grade)}</td>
    </tr>`
    )
    .join("");

  // Full report
  document.getElementById("fullReportTable").innerHTML = students
    .map((s) => studentRow(s))
    .join("");
}

// ============================================================
// 8. DASHBOARD
// ============================================================

function refreshDashboard() {
  const students = getStudents();

  // Stats
  document.getElementById("statTotal").textContent = students.length;

  const courses = [...new Set(students.map((s) => s.course))];
  document.getElementById("statCourses").textContent = courses.length;

  const avgGrade =
    students.length > 0
      ? (students.reduce((sum, s) => sum + s.grade, 0) / students.length).toFixed(1)
      : "0.0";
  document.getElementById("statAvgGrade").textContent = avgGrade;

  const topGrade =
    students.length > 0 ? Math.max(...students.map((s) => s.grade)).toFixed(1) : "0.0";
  document.getElementById("statTopGrade").textContent = topGrade;

  // Recent students (last 5)
  const recent = students.slice(-5).reverse();
  document.getElementById("recentStudentsTable").innerHTML = recent
    .map((s) => studentRow(s))
    .join("");
}

function refreshStudentTable() {
  const students = getStudents();
  const tbody = document.getElementById("allStudentsTable");
  const empty = document.getElementById("viewEmpty");

  if (students.length === 0) {
    tbody.innerHTML = "";
    empty.classList.remove("hidden");
  } else {
    empty.classList.add("hidden");
    tbody.innerHTML = students.map((s) => studentRow(s)).join("");
  }
}

// ============================================================
// 9. UI HELPERS
// ============================================================

/** Build a table row for a student */
function studentRow(s) {
  return `<tr>
    <td>${s.student_id}</td>
    <td>${s.name}</td>
    <td>${s.age}</td>
    <td>${s.course}</td>
    <td>${gradeBadge(s.grade)}</td>
    <td>${s.contact}</td>
  </tr>`;
}

/** Create a colored grade badge */
function gradeBadge(grade) {
  const g = parseFloat(grade);
  let cls = "grade-low";
  if (g >= 8) cls = "grade-high";
  else if (g >= 6) cls = "grade-mid";
  return `<span class="grade-badge ${cls}">${g.toFixed(1)}</span>`;
}

/** Show a toast notification */
function showToast(message, type = "info") {
  const container = document.getElementById("toastContainer");
  const toast = document.createElement("div");
  toast.className = `toast ${type}`;
  const icons = { success: "✅", error: "❌", info: "ℹ️" };
  toast.innerHTML = `<span>${icons[type] || ""}</span> ${message}`;
  container.appendChild(toast);
  setTimeout(() => toast.remove(), 3000);
}

/** Validate student input fields */
function validateStudentInput(id, name, age, course, grade, contact) {
  if (!id || id <= 0) {
    showToast("Student ID must be a positive number.", "error");
    return false;
  }
  if (!name || name.length < 2) {
    showToast("Name must be at least 2 characters.", "error");
    return false;
  }
  if (!age || age < 16 || age > 60) {
    showToast("Age must be between 16 and 60.", "error");
    return false;
  }
  if (!course) {
    showToast("Course is required.", "error");
    return false;
  }
  if (grade === undefined || grade < 0 || grade > 10) {
    showToast("Grade must be between 0 and 10.", "error");
    return false;
  }
  if (!contact || !/^\d{10}$/.test(contact)) {
    showToast("Contact must be exactly 10 digits.", "error");
    return false;
  }
  return true;
}

// ============================================================
// 10. INITIALIZATION
// ============================================================

document.addEventListener("DOMContentLoaded", () => {
  const session = requireAuth();
  if (!session) return;

  // Set user info in sidebar
  const badge = document.getElementById("userBadge");
  if (badge) {
    badge.textContent = session.role === "admin" ? "Admin" : "User";
    badge.className = `user-badge ${session.role}`;
  }
  const welcome = document.getElementById("welcomeUser");
  if (welcome) welcome.textContent = session.username;

  // Hide admin-only tabs for regular users
  if (session.role !== "admin") {
    document.querySelectorAll('.tab-btn').forEach((btn, i) => {
      // Hide Add, Update, Delete tabs (indices 0, 1, 2)
      if (i < 3) btn.style.display = "none";
    });
    // Auto-switch to view tab
    switchTabDirect("view");
  }

  // Setup delete confirmation
  document.getElementById("confirmDeleteBtn").addEventListener("click", confirmDelete);

  // Search on Enter key
  document.getElementById("searchInput").addEventListener("keyup", (e) => {
    if (e.key === "Enter") searchStudents();
  });

  // Load dashboard
  refreshDashboard();
});

/** Direct tab switch without event (used on init) */
function switchTabDirect(tab) {
  document.querySelectorAll(".tab-btn").forEach((btn) => btn.classList.remove("active"));
  document.querySelectorAll(".tab-content").forEach((el) => el.classList.remove("active"));
  const target = document.getElementById(`tab-${tab}`);
  if (target) target.classList.add("active");
}
