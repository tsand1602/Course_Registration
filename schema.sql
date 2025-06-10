CREATE DATABASE IF NOT EXISTS course_registration;
USE course_registration;

-- Table: students
CREATE TABLE IF NOT EXISTS students (
    student_id VARCHAR(20) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    year INT NOT NULL,
    cgpa FLOAT NOT NULL,
    completed_credits INT DEFAULT 0
);

-- Table: courses
CREATE TABLE IF NOT EXISTS courses (
    course_code VARCHAR(20) PRIMARY KEY,
    course_name VARCHAR(100) NOT NULL,
    credits INT NOT NULL,
    capacity INT NOT NULL,
    time_slot VARCHAR(10) NOT NULL
);

-- Table: completed_courses
CREATE TABLE IF NOT EXISTS completed_courses (
    student_id VARCHAR(20),
    course_code VARCHAR(20),
    PRIMARY KEY (student_id, course_code),
    FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE,
    FOREIGN KEY (course_code) REFERENCES courses(course_code) ON DELETE CASCADE
);

-- Table: enrollments
CREATE TABLE IF NOT EXISTS enrollments (
    student_id VARCHAR(20),
    course_code VARCHAR(20),
    PRIMARY KEY (student_id, course_code),
    FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE,
    FOREIGN KEY (course_code) REFERENCES courses(course_code) ON DELETE CASCADE
);

-- Table: prerequisites
CREATE TABLE IF NOT EXISTS prerequisites (
    course_code VARCHAR(20),
    prerequisite_code VARCHAR(20),
    PRIMARY KEY (course_code, prerequisite_code),
    FOREIGN KEY (course_code) REFERENCES courses(course_code) ON DELETE CASCADE,
    FOREIGN KEY (prerequisite_code) REFERENCES courses(course_code) ON DELETE CASCADE
);

-- Table: waitlists
CREATE TABLE IF NOT EXISTS waitlists (
    id INT AUTO_INCREMENT PRIMARY KEY,
    course_code VARCHAR(20),
    student_id VARCHAR(20),
    FOREIGN KEY (course_code) REFERENCES courses(course_code) ON DELETE CASCADE,
    FOREIGN KEY (student_id) REFERENCES students(student_id) ON DELETE CASCADE
);