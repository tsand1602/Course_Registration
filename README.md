# Course Registration System

A simple C++ console application to manage a university course registration process, including student enrollment, course management, prerequisite checking, time slot clash detection, waitlisting, and more.

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

### Command List with Prompts

- `addStudent`  
  Add a new student. The program will prompt for:
  - Student ID
  - Student Name
  - Year of Study
  - CGPA
  - Number of completed courses, then asks for each course code

- `addCourse`  
  Add a new course. The program will prompt for:
  - Course ID
  - Course Name
  - Number of Credits
  - Course Capacity
  - Time Slot (A/B/C/etc.)
  - Number of prerequisite courses, then asks for each prerequisite course code

- `enroll`  
  Enroll a student in a course. The program will prompt for:
  - Student ID for Enrollment
  - Course ID to Enroll In

- `drop`  
  Drop a student from a course. The program will prompt for:
  - Student ID for Dropping Course
  - Course ID to Drop

- `ShowCourseDetails`  
  Display details of a course. The program will prompt for:
  - Course ID to View Details

### Interactive Input with Helper Prompts

The program guides you for every input field. When executing a command, you will be prompted for each value. For example:

#### Example Session

```
Enter the number of queries to be made: 5

Enter the type of query to be made (addStudent, addCourse, enroll, drop, ShowCourseDetails): addStudent
Enter Student ID: S01
Enter Student Name: Alice
Enter Year of Study: 2
Enter CGPA: 8.2
Enter Number of Completed Courses: 2
Enter Course 1: CS101
Enter Course 2: MA101
Student S01 has been added!

Enter the type of query to be made (addStudent, addCourse, enroll, drop, ShowCourseDetails): addCourse
Enter Course ID: CS201
Enter Course Name: DataStructures
Enter Number of Credits: 4
Enter Course Capacity: 2
Enter Time Slot (A/B/C/etc.): A
Enter Number of Prerequisite Courses: 1
Enter Prerequisite Course 1: CS101
Course CS201 has been added!

Enter the type of query to be made (addStudent, addCourse, enroll, drop, ShowCourseDetails): enroll
Enter Student ID for Enrollment: S01
Enter Course ID to Enroll In: CS201
Student S01 has been enrolled!

Enter the type of query to be made (addStudent, addCourse, enroll, drop, ShowCourseDetails): ShowCourseDetails
Enter Course ID to View Details: CS201
Course Code : CS201
Course Name : DataStructures
Credits : 4
Capacity : 2 students
Time Slot : A
Prerequisites : CS101 

Enter the type of query to be made (addStudent, addCourse, enroll, drop, ShowCourseDetails): drop
Enter Student ID for Dropping Course: S01
Enter Course ID to Drop: CS201
Course has been dropped!

Registration completed!
```

## How It Works

- The program reads the number of queries, then executes each command.
- All data is stored in memory using C++ STL containers (map, set, queue, vector).
- Constraints like prerequisites, time slot clashes, and credit limits are enforced during enrollment.
- Waitlisted students are automatically enrolled if space becomes available.
- Input is user-friendly: the program provides helper lines for each prompt.

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
