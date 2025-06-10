import mysql.connector

class Student:
    def __init__(self, student_id, name, year, cgpa, completed_courses, completed_credits=0):
        self.student_id = student_id
        self.name = name
        self.year = year
        self.cgpa = cgpa
        self.completed_credits = completed_credits
        self.completed_courses = set(completed_courses)
        self.enrolled_courses = set()

    def find_course(self, course_code):
        return course_code in self.completed_courses or course_code in self.enrolled_courses

class Course:
    def __init__(self, code, name, credits, capacity, time_slot, prerequisites):
        self.code = code
        self.name = name
        self.credits = credits
        self.capacity = capacity
        self.time_slot = time_slot
        self.prerequisites = set(prerequisites)
        self.enrolled_students = []
        self.waitlist = []

class Administration:
    def __init__(self):
        self.db = mysql.connector.connect(
            host="localhost",
            user="YOUR_MYSQL_USERNAME",
            password="YOUR_MYSQL_PASSWORD",  # Set your MySQL root password
            database="course_registration"
        )
        self.cursor = self.db.cursor(buffered=True)

    def addStudent(self):
        id = input("Enter student ID: ").strip()
        name = input("Enter student name: ").strip()
        year = int(input("Enter student year: "))
        cgpa = float(input("Enter student CGPA: "))
        n = int(input("Enter number of completed courses: "))
        completed = []
        for i in range(n):
            completed.append(input(f"Enter completed course code #{i+1}: ").strip())
        print()
        try:
            self.cursor.execute(
                "INSERT INTO students (student_id, name, year, cgpa, completed_credits) VALUES (%s, %s, %s, %s, 0)",
                (id, name, year, cgpa)
            )
            for course in completed:
                try:
                    self.cursor.execute(
                        "INSERT INTO completed_courses (student_id, course_code) VALUES (%s, %s)", (id, course)
                    )
                except mysql.connector.errors.IntegrityError:
                    pass
            self.db.commit()
            print(f"Student {id} has been added!")
        except mysql.connector.errors.IntegrityError as e:
            if "Duplicate entry" in str(e) and "students.PRIMARY" in str(e):
                print("Student already registered!")
            else:
                print("Integrity error:", e)
        print("-" * 40)

    def addCourse(self):
        id = input("Enter course code: ").strip()
        name = input("Enter course name: ").strip()
        credits = int(input("Enter number of credits: "))
        capacity = int(input("Enter course capacity: "))
        slot = input("Enter course time slot: ").strip()
        n = int(input("Enter number of prerequisites: "))
        prereq = []
        valid = True
        for i in range(n):
            p = input(f"Enter prerequisite course code #{i+1}: ").strip()
            prereq.append(p)
            self.cursor.execute("SELECT * FROM courses WHERE course_code=%s", (p,))
            if self.cursor.fetchone() is None:
                print(f"Prerequisite course {p} doesn't exist!")
                valid = False
        print()
        if valid:
            try:
                self.cursor.execute(
                    "INSERT INTO courses (course_code, course_name, credits, capacity, time_slot) VALUES (%s, %s, %s, %s, %s)",
                    (id, name, credits, capacity, slot)
                )
                for p in prereq:
                    try:
                        self.cursor.execute(
                            "INSERT INTO prerequisites (course_code, prerequisite_code) VALUES (%s, %s)", (id, p)
                        )
                    except mysql.connector.errors.IntegrityError:
                        pass
                self.db.commit()
                print(f"Course {id} has been added!")
            except mysql.connector.errors.IntegrityError as e:
                if "Duplicate entry" in str(e) and "courses.PRIMARY" in str(e):
                    print("Course already registered!")
                else:
                    print("Integrity error:", e)
        print("-" * 40)

    def enroll(self):
        student_id = input("Enter student ID to enroll: ").strip()
        course_id = input("Enter course code to enroll in: ").strip()
        print()
        self.cursor.execute("SELECT * FROM students WHERE student_id=%s", (student_id,))
        student = self.cursor.fetchone()
        self.cursor.execute("SELECT * FROM courses WHERE course_code=%s", (course_id,))
        course = self.cursor.fetchone()
        if not student or not course:
            print("Invalid ID!")
            print("-" * 40)
            return

        self.cursor.execute("SELECT * FROM completed_courses WHERE student_id=%s AND course_code=%s", (student_id, course_id))
        if self.cursor.fetchone():
            print("Course already registered!")
            print("-" * 40)
            return
        self.cursor.execute("SELECT * FROM enrollments WHERE student_id=%s AND course_code=%s", (student_id, course_id))
        if self.cursor.fetchone():
            print("Course already registered!")
            print("-" * 40)
            return

        self.cursor.execute("SELECT prerequisite_code FROM prerequisites WHERE course_code=%s", (course_id,))
        prereqs = [row[0] for row in self.cursor.fetchall()]
        for p in prereqs:
            self.cursor.execute("SELECT * FROM completed_courses WHERE student_id=%s AND course_code=%s", (student_id, p))
            if self.cursor.fetchone() is None:
                print(f"Prerequisite course {p} hasn't been completed!")
                print("-" * 40)
                return

        self.cursor.execute("SELECT time_slot FROM courses WHERE course_code=%s", (course_id,))
        new_slot = self.cursor.fetchone()[0]
        self.cursor.execute("SELECT c.time_slot FROM enrollments e JOIN courses c ON e.course_code = c.course_code WHERE e.student_id=%s", (student_id,))
        for (slot,) in self.cursor.fetchall():
            if slot == new_slot:
                print(f"Time slot clash with registered course!")
                print("-" * 40)
                return

        self.cursor.execute("SELECT completed_credits, cgpa FROM students WHERE student_id=%s", (student_id,))
        credits, cgpa = self.cursor.fetchone()
        self.cursor.execute("SELECT credits FROM courses WHERE course_code=%s", (course_id,))
        new_credits = self.cursor.fetchone()[0]
        limit = 66 if cgpa < 8 else 72
        if credits + new_credits > limit:
            print("Maximum credit limit has been reached!")
            print("-" * 40)
            return

        self.cursor.execute("SELECT COUNT(*) FROM enrollments WHERE course_code=%s", (course_id,))
        enrolled_count = self.cursor.fetchone()[0]
        self.cursor.execute("SELECT capacity FROM courses WHERE course_code=%s", (course_id,))
        capacity = self.cursor.fetchone()[0]
        if enrolled_count >= capacity:
            try:
                self.cursor.execute("INSERT INTO waitlists (course_code, student_id) VALUES (%s, %s)", (course_id, student_id))
                print("Currently in waitlist for registration")
            except mysql.connector.errors.IntegrityError:
                print("Already in waitlist for this course!")
        else:
            try:
                self.cursor.execute("INSERT INTO enrollments (student_id, course_code) VALUES (%s, %s)", (student_id, course_id))
                self.cursor.execute("UPDATE students SET completed_credits = completed_credits + %s WHERE student_id=%s", (new_credits, student_id))
                print(f"Student {student_id} has been enrolled!")
            except mysql.connector.errors.IntegrityError:
                print("Course already registered!")
        self.db.commit()
        print("-" * 40)

    def drop(self):
        student_id = input("Enter student ID to drop from course: ").strip()
        course_id = input("Enter course code to drop: ").strip()
        print()
        self.cursor.execute("SELECT * FROM enrollments WHERE student_id=%s AND course_code=%s", (student_id, course_id))
        if not self.cursor.fetchone():
            print("Course hasn't been registered!")
            print("-" * 40)
            return
        self.cursor.execute("DELETE FROM enrollments WHERE student_id=%s AND course_code=%s", (student_id, course_id))
        self.cursor.execute("SELECT credits FROM courses WHERE course_code=%s", (course_id,))
        credits = self.cursor.fetchone()
        if credits:
            self.cursor.execute("UPDATE students SET completed_credits = completed_credits - %s WHERE student_id=%s", (credits[0], student_id))
        self.db.commit()
        print("Course has been dropped!")

        self.cursor.execute("SELECT student_id FROM waitlists WHERE course_code=%s ORDER BY id", (course_id,))
        for (wait_student_id,) in self.cursor.fetchall():
            self.cursor.execute("SELECT time_slot FROM courses WHERE course_code=%s", (course_id,))
            time_slot = self.cursor.fetchone()[0]
            self.cursor.execute("SELECT c.time_slot FROM enrollments e JOIN courses c ON e.course_code = c.course_code WHERE e.student_id=%s", (wait_student_id,))
            has_conflict = False
            for (slot,) in self.cursor.fetchall():
                if slot == time_slot:
                    has_conflict = True
                    break
            self.cursor.execute("SELECT completed_credits, cgpa FROM students WHERE student_id=%s", (wait_student_id,))
            cred, gpa = self.cursor.fetchone()
            self.cursor.execute("SELECT credits FROM courses WHERE course_code=%s", (course_id,))
            newcred = self.cursor.fetchone()[0]
            if (gpa < 8 and cred + newcred > 66) or (gpa >= 8 and cred + newcred > 72):
                has_conflict = True
            if not has_conflict:
                try:
                    self.cursor.execute("INSERT INTO enrollments (student_id, course_code) VALUES (%s, %s)", (wait_student_id, course_id))
                    self.cursor.execute("UPDATE students SET completed_credits = completed_credits + %s WHERE student_id=%s", (newcred, wait_student_id))
                    self.cursor.execute("DELETE FROM waitlists WHERE student_id=%s AND course_code=%s", (wait_student_id, course_id))
                    self.db.commit()
                    print(f"Student {wait_student_id} has been enrolled!")
                except mysql.connector.errors.IntegrityError:
                    pass
                break
        print("-" * 40)

    def printCourseDetails(self):
        cid = input("Enter course code to display details: ").strip()
        print()
        self.cursor.execute("SELECT * FROM courses WHERE course_code=%s", (cid,))
        course = self.cursor.fetchone()
        if course:
            print("Course Code:", course[0])
            print("Course Name:", course[1])
            print("Credits:", course[2])
            print("Capacity:", course[3])
            print("Time Slot:", course[4])
            self.cursor.execute("SELECT prerequisite_code FROM prerequisites WHERE course_code=%s", (cid,))
            prereqs = [p[0] for p in self.cursor.fetchall()]
            print("Prerequisites:", prereqs if prereqs else "None!")
        else:
            print("Invalid Course!")
        print("-" * 40)

def main():
    admin = Administration()
    queries = int(input("Enter number of commands to execute: "))
    cmd_count = 1
    while queries > 0:
        command = input(f"\nEnter command #{cmd_count} (add_student, add_course, enroll, drop, show_course): ").strip()
        if command == "add_student":
            admin.addStudent()
        elif command == "add_course":
            admin.addCourse()
        elif command == "enroll":
            admin.enroll()
        elif command == "drop":
            admin.drop()
        elif command == "show_course":
            admin.printCourseDetails()
        else:
            print("Unknown command!")
            print("-" * 40)
        queries -= 1
        cmd_count += 1
    print("Course Registration done!!")

if __name__ == "__main__":
    main()
