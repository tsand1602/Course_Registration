# Course Registration System (Python + MySQL)

A Python console application to manage a university course registration process, including student enrollment, course management, prerequisite checking, time slot clash detection, waitlisting, and more. Data is stored persistently using a MySQL database.

## Features

- **Student Management:**  
  Add students with details such as ID, name, year, CGPA, and completed courses.

- **Course Management:**  
  Add courses with code, name, credits, capacity, prerequisites, and time slot.

- **Enrollment:**  
  Students can enroll in courses if they meet prerequisites, available slots, and credit limits. Handles time slot clashes.

- **Waitlisting:**  
  Maintains a waitlist when a course reaches its capacity. Automatically enrolls from the waitlist when spots open.

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
   - Replace `<username>` with your MySQL username (often `root`).  
   - Enter your password when prompted.
   - If MySQL is not in your PATH (especially on Windows), use the full path to `mysql.exe`:
     ```
     "C:\Program Files\MySQL\MySQL Server 8.0\bin\mysql.exe" -u root -p < schema.sql
     ```

4. **Configure Database Connection**

   - Edit `course_registration.py` to set your MySQL username and password if different from the defaults:
     ```python
     self.db = mysql.connector.connect(
         host="localhost",
         user="YOUR_MYSQL_USERNAME",
         password="YOUR_MYSQL_PASSWORD",
         database="course_registration"
     )
     ```

## Usage

Run the application:
```bash
py course_registration.py
```

The program is interactive and will guide you through all input fields for each command.

### Command List & Prompts

- `add_student`  
  Prompts for:
  - Student ID
  - Student Name
  - Year of Study
  - CGPA
  - Number of completed courses, then asks for each course code

- `add_course`  
  Prompts for:
  - Course code
  - Course name
  - Number of credits
  - Capacity
  - Time slot (A/B/C/etc.)
  - Number of prerequisites, then asks for each prerequisite course code

- `enroll`  
  Prompts for:
  - Student ID to enroll
  - Course code to enroll in

- `drop`  
  Prompts for:
  - Student ID
  - Course code to drop

- `show_course`  
  Prompts for:
  - Course code to display details

### Example Session

```
Enter number of commands to execute: 5

Enter command #1 (add_student, add_course, enroll, drop, show_course): add_student
Enter student ID: S01
Enter student name: Alice
Enter student year: 2
Enter student CGPA: 8.2
Enter number of completed courses: 2
Enter completed course code #1: CS101
Enter completed course code #2: MA101

Student S01 has been added!
----------------------------------------

Enter command #2 (add_student, add_course, enroll, drop, show_course): add_course
Enter course code: CS201
Enter course name: DataStructures
Enter number of credits: 4
Enter course capacity: 2
Enter course time slot: A
Enter number of prerequisites: 1
Enter prerequisite course code #1: CS101

Course CS201 has been added!
----------------------------------------

Enter command #3 (add_student, add_course, enroll, drop, show_course): enroll
Enter student ID to enroll: S01
Enter course code to enroll in: CS201

Student S01 has been enrolled!
----------------------------------------

Enter command #4 (add_student, add_course, enroll, drop, show_course): show_course
Enter course code to display details: CS201

Course Code: CS201
Course Name: DataStructures
Credits: 4
Capacity: 2
Time Slot: A
Prerequisites: ['CS101']
----------------------------------------

Enter command #5 (add_student, add_course, enroll, drop, show_course): drop
Enter student ID to drop from course: S01
Enter course code to drop: CS201

Course has been dropped!
----------------------------------------

Course Registration done!!
```

## Database Schema

See [`schema.sql`](schema.sql) for table definitions for students, courses, enrollments, prerequisites, completed_courses, and waitlists.

## Notes

- All data is persisted in the MySQL database.
- The program uses clear, separated console output for improved readability.
- All constraints (prerequisites, credit limits, time slot clashes) are enforced at enrollment.
