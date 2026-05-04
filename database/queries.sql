-- ============================================================
-- Scholar Track - Database Module
-- SQL Reference Queries for MySQL
-- ============================================================
-- NOTE: These queries are for REFERENCE ONLY.
-- The database schema (Students table) is assumed to already exist.
-- No direct connection between frontend/backend and database.
-- ============================================================

-- Table Schema (for reference, DO NOT run — already created):
-- CREATE TABLE Students (
--     student_id INT PRIMARY KEY,
--     name VARCHAR(100),
--     age INT,
--     course VARCHAR(100),
--     grade FLOAT,
--     contact VARCHAR(50)
-- );

-- ============================================================
-- 1. INSERT STUDENT
-- ============================================================
-- Add a new student record
INSERT INTO Students (student_id, name, age, course, grade, contact)
VALUES (1001, 'John Doe', 20, 'Computer Science', 8.5, '9876543210');

-- Insert multiple students at once
INSERT INTO Students (student_id, name, age, course, grade, contact) VALUES
(1002, 'Jane Smith', 21, 'Mathematics', 9.0, '9876543211'),
(1003, 'Bob Wilson', 19, 'Physics', 7.8, '9876543212'),
(1004, 'Alice Brown', 22, 'Chemistry', 8.2, '9876543213'),
(1005, 'Charlie Davis', 20, 'Computer Science', 9.5, '9876543214');

-- ============================================================
-- 2. UPDATE STUDENT
-- ============================================================
-- Update a student's course
UPDATE Students
SET course = 'Data Science'
WHERE student_id = 1001;

-- Update a student's grade
UPDATE Students
SET grade = 9.2
WHERE student_id = 1003;

-- Update multiple fields for a student
UPDATE Students
SET name = 'John A. Doe', age = 21, contact = '9999999999'
WHERE student_id = 1001;

-- ============================================================
-- 3. DELETE STUDENT
-- ============================================================
-- Delete a student by ID
DELETE FROM Students
WHERE student_id = 1005;

-- Delete students by course (bulk delete)
DELETE FROM Students
WHERE course = 'Physics';

-- ============================================================
-- 4. SELECT ALL STUDENTS
-- ============================================================
-- Retrieve all student records
SELECT * FROM Students;

-- Retrieve all students ordered by name
SELECT * FROM Students
ORDER BY name ASC;

-- Retrieve all students ordered by grade (descending)
SELECT * FROM Students
ORDER BY grade DESC;

-- Retrieve student count
SELECT COUNT(*) AS total_students FROM Students;

-- ============================================================
-- 5. SEARCH QUERIES
-- ============================================================
-- Search by student ID
SELECT * FROM Students
WHERE student_id = 1001;

-- Search by name (exact match)
SELECT * FROM Students
WHERE name = 'John Doe';

-- Search by name (partial match / wildcard)
SELECT * FROM Students
WHERE name LIKE '%John%';

-- Search by course
SELECT * FROM Students
WHERE course = 'Computer Science';

-- Search by course (partial match)
SELECT * FROM Students
WHERE course LIKE '%Science%';

-- ============================================================
-- 6. REPORT QUERIES (Aggregation & Analytics)
-- ============================================================
-- Average grade per course
SELECT course, AVG(grade) AS avg_grade
FROM Students
GROUP BY course;

-- Count of students per course
SELECT course, COUNT(*) AS student_count
FROM Students
GROUP BY course;

-- Students with grade above a threshold
SELECT * FROM Students
WHERE grade >= 8.0
ORDER BY grade DESC;

-- Youngest and oldest student
SELECT MIN(age) AS youngest, MAX(age) AS oldest
FROM Students;

-- Top performing students
SELECT * FROM Students
ORDER BY grade DESC
LIMIT 5;

-- ============================================================
-- END OF QUERIES
-- ============================================================
