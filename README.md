# Course Registration System (Python + MySQL)

A Python console application to manage a university course registration process, including student enrollment, course management, prerequisite checking, time slot clash detection, waitlisting, and more. Data is stored persistently using a MySQL database.

## Features

- **Student Management:**  
  Add students with ID, name, year, CGPA, and completed courses.
- **Course Management:**  
  Add courses with code, name, credits, capacity, prerequisites, and time slot.
- **Enrollment:**  
  Enroll students in courses if they meet prerequisites, available slots, and credit limits. Handles time slot clashes.
- **Waitlisting:**  
  Maintains a waitlist when a course is full. Automatically enrolls from the waitlist when spots open.
- **Dropping Courses:**  
  Students can drop courses. Waitlisted students are considered for enrollment upon vacancy.
- **Course Details:**  
  Display complete details for any course, including prerequisites.

## Setup

### Requirements

- Python 3.7 or higher
- MySQL Server
- [`mysql-connector-python`](https://pypi.org/project/mysql-connector-python/)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/tsand1602/Course_Registration.git
   cd Course_Registration
   ```

2. **Install Python dependencies**
   ```bash
   pip install mysql-connector-python
   ```

3. **Create the MySQL database and tables**

   - Ensure your MySQL server is running.
   - Use the provided `schema.sql`:
     ```bash
     mysql -u <username> -p < schema.sql
     ```
   - Replace `<username>` with your MySQL username (e.g., `root`).
   - Enter your password when prompted.
   - If MySQL is not in your PATH (especially on Windows), use the full path to `mysql.exe`:
     ```
     "C:\Program Files\MySQL\MySQL Server 8.0\bin\mysql.exe" -u root -p < schema.sql
     ```

4. **Configure Database Connection**

   - Edit `course_registration.py` to set your MySQL username and password if different from the defaults:
     ```python
     self.db = mysql.connector.connect(
         host = "localhost",
         user = "YOUR_MYSQL_USERNAME",
         password = "YOUR_MYSQL_PASSWORD",
         database = "course_registration"
     )
     ```

## Usage

Run the application:
```bash
python course_registration.py
```

The program is interactive and will guide you through all input fields for each command.

### Example Session

Below is a realistic example with **10 commands** to demonstrate registering courses (with prerequisites), adding students, enrolling, dropping, and printing course details.

```
Enter number of commands to execute: 10

Enter command #1 (add_student, add_course, enroll, drop, show_course): add_course
Enter course code: MA101
Enter course name: Calculus
Enter number of credits: 4
Enter course capacity: 3
Enter course time slot: A
Enter number of prerequisites: 0

Course MA101 has been added!
----------------------------------------

Enter command #2 (add_student, add_course, enroll, drop, show_course): add_course
Enter course code: CS101
Enter course name: Programming
Enter number of credits: 5
Enter course capacity: 2
Enter course time slot: B
Enter number of prerequisites: 0

Course CS101 has been added!
----------------------------------------

Enter command #3 (add_student, add_course, enroll, drop, show_course): add_course
Enter course code: CS201
Enter course name: Data Structures
Enter number of credits: 4
Enter course capacity: 2
Enter course time slot: C
Enter number of prerequisites: 1
Enter prerequisite course code #1: CS101

Course CS201 has been added!
----------------------------------------

Enter command #4 (add_student, add_course, enroll, drop, show_course): add_student
Enter student ID: S01
Enter student name: Alice
Enter student year: 2
Enter student CGPA: 8.2
Enter number of completed courses: 2
Enter completed course code #1: CS101
Enter completed course code #2: MA101

Student S01 has been added!
----------------------------------------

Enter command #5 (add_student, add_course, enroll, drop, show_course): add_student
Enter student ID: S02
Enter student name: Bob
Enter student year: 1
Enter student CGPA: 7.5
Enter number of completed courses: 1
Enter completed course code #1: MA101

Student S02 has been added!
----------------------------------------

Enter command #6 (add_student, add_course, enroll, drop, show_course): enroll
Enter student ID to enroll: S01
Enter course code to enroll in: CS201

Student S01 has been enrolled!
----------------------------------------

Enter command #7 (add_student, add_course, enroll, drop, show_course): enroll
Enter student ID to enroll: S02
Enter course code to enroll in: CS101

Student S02 has been enrolled!
----------------------------------------

Enter command #8 (add_student, add_course, enroll, drop, show_course): enroll
Enter student ID to enroll: S02
Enter course code to enroll in: CS201

Prerequisite course CS101 hasn't been completed!
----------------------------------------

Enter command #9 (add_student, add_course, enroll, drop, show_course): drop
Enter student ID to drop from course: S01
Enter course code to drop: CS201

Course has been dropped!
----------------------------------------

Enter command #10 (add_student, add_course, enroll, drop, show_course): show_course
Enter course code to display details: CS201

Course Code: CS201
Course Name: DataStructures
Credits: 4
Capacity: 2
Time Slot: C
Prerequisites: ['CS101']
----------------------------------------

Course Registration done!!
```

## Database Schema

See [`schema.sql`](schema.sql) for table definitions for students, courses, enrollments, prerequisites, completed_courses, and waitlists.

## Notes

- All data is persisted in the MySQL database.
- The program uses clear, separated console output for improved readability.
- All constraints (prerequisites, credit limits, time slot clashes) are enforced at enrollment.
