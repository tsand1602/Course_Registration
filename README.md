# Course Registration System

A simple C++ console application to manage a university course registration process, including student enrollment, course management, prerequisite checking, time slot clash detection, waitlisting, and credit limits.

## Features

- **Student Management:**  
  Add students with details such as ID, name, year, CGPA, and completed courses.

- **Course Management:**  
  Add courses with course code, name, credits, capacity, prerequisites, and time slot.

- **Enrollment:**  
  Students can enroll in courses if they meet prerequisites, available slots, and credit limits. Handles time slot clashes.

- **Waitlisting:**  
  Maintains a waitlist when a course reaches its capacity. Automatically enrolls from waitlist when spots open.

- **Dropping Courses:**  
  Students can drop courses. Waitlisted students are considered for enrollment upon vacancy.

- **Course Details:**  
  Display details for any course, including code, name, credits, capacity, time slot, and prerequisites.

## Usage

### Commands

- `add_student`  
  Add a new student.  
  Input format:  
  ```
  <student_id> <name> <year> <cgpa> <number_of_completed_courses>
  <completed_course_1> <completed_course_2> ... <completed_course_n>
  ```

- `add_course`  
  Add a new course.  
  Input format:  
  ```
  <course_code> <course_name> <credits> <capacity> <time_slot> <number_of_prerequisites>
  <prereq_1> <prereq_2> ... <prereq_n>
  ```

- `enroll`  
  Enroll a student in a course.  
  Input format:  
  ```
  <student_id> <course_code>
  ```

- `drop`  
  Drop a student from a course.  
  Input format:  
  ```
  <student_id> <course_code>
  ```

- `ShowCourseDetails`  
  Display details of a course.  
  Input format:  
  ```
  <course_code>
  ```

### Example

```
5
add_student
S01 Alice 2 8.2 2
CS101 MA101
add_course
CS201 DataStructures 4 2 A 1
CS101
enroll
S01 CS201
ShowCourseDetails
CS201
drop
S01 CS201
```

## How It Works

- The program reads the number of queries, then executes each command.
- All data is stored in memory using C++ STL containers (map, set, queue, vector).
- Constraints like prerequisites, time slot clashes, and credit limits are enforced during enrollment.
- Waitlisted students are automatically enrolled if space becomes available.

## Requirements

- C++11 or higher
- Standard input/output (console)

## Compilation

```bash
g++ -std=c++11 Course_registration.cpp -o course_registration
```

## Running

```bash
./course_registration
```
