#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class course;

class student {
    string student_id, name;
    float cgpa;
    int year, completed_credits;
    unordered_set<string> completed_courses;
    unordered_set<string> enrolled_courses;
public:
    student() : student_id("XX"), name("YY"), cgpa(0), year(0), completed_credits(0) {}
    student(string student_id, string name, int year, float cgpa, vector<string> courses) : student_id(student_id), name(name), cgpa(cgpa), year(year), completed_credits(0) {
        for (int i = 0; i < courses.size(); i++)
            completed_courses.insert(courses[i]);
    }
    bool findcourse(const string& course_code) {
        return (completed_courses.find(course_code) != completed_courses.end()) || (enrolled_courses.find(course_code) != enrolled_courses.end());
    }
    friend class course;
};

class course {
    string course_code, course_name;
    int credits, capacity;
    char time_slot;
    unordered_set<string> prerequisites;
    vector<string> enrolled_students; 
    queue<string> waitlist;
public:
    course() : credits(0), capacity(0), time_slot(' ') {}
    course(string course_code, string course_name, int credits, int capacity, char time_slot, vector<string> prereq) : course_code(course_code), course_name(course_name), credits(credits), capacity(capacity), time_slot(time_slot) {
        for (int i = 0; i < prereq.size(); i++)
            prerequisites.insert(prereq[i]);
    }
    void enroll(student& s, unordered_map<string, course>& course_mp) {
        for (auto pre : prerequisites) {
            if (s.completed_courses.find(pre) == s.completed_courses.end()) {
                cout << "Prerequisite course " << pre << " hasn't been completed!" << endl;
                return;
            }
        }
        for (auto curr : s.enrolled_courses) {
            if (course_mp[curr].time_slot == time_slot) {
                cout << "Time slot clash with registered course " << curr << "!" << endl;
                return; 
            }
        }
        if ((s.cgpa < 8 && s.completed_credits + credits > 66) || (s.cgpa >= 8 && s.completed_credits + credits > 72)) {
            cout << "Maximum credit limit has been reached!" << endl;
            return;
        }
        if (enrolled_students.size() == capacity) {
            waitlist.push(s.student_id);
            cout << "Currently in waitlist for registration" << endl;
            return;
        }
        enrolled_students.push_back(s.student_id); 
        s.completed_credits += credits;
        s.enrolled_courses.insert(course_code);
    }
    void drop(student& s, unordered_map<string, student>& std_mp, unordered_map<string, course>& course_mp) {
        if (s.enrolled_courses.find(course_code) == s.enrolled_courses.end()) {
            cout << "Course hasn't been registered!" << endl;
            return;
        }
        auto it = find(enrolled_students.begin(), enrolled_students.end(), s.student_id);
        if (it != enrolled_students.end()) {
            enrolled_students.erase(it);
        }
        s.enrolled_courses.erase(course_code);
        s.completed_credits -= credits;
        cout << "Course has been dropped!" << endl; 
        while (!waitlist.empty()) {
            student &next_student = std_mp[waitlist.front()];
            waitlist.pop();
            bool has_conflict = false;
            for (auto enrolled_course : next_student.enrolled_courses) {
                if (course_mp[enrolled_course].time_slot == time_slot) {
                    has_conflict = true;
                    break;
                }
            }
            if ((next_student.cgpa < 8 && next_student.completed_credits + credits > 66) || (next_student.cgpa >= 8 && next_student.completed_credits + credits > 72))
                has_conflict = true;
            if (!has_conflict) {
                enrolled_students.push_back(next_student.student_id);
                next_student.completed_credits += credits;
                next_student.enrolled_courses.insert(course_code);
                break; 
            }
        }
    }
    void printcourses() {
        cout << "Course Code : " << course_code << endl;
        cout << "Course Name : " << course_name << endl;
        cout << "Credits : " << credits << endl;
        cout << "Capacity : " << capacity << " students" << endl;
        cout << "Time Slot : " << time_slot << endl;
        cout << "Prerequisites : ";
        for (const auto& pre : prerequisites)
            cout << pre << " ";
        cout << endl;
    }
    friend class student;
};
class Administration {
    unordered_map<string, student> std_mp;
    unordered_map<string, course> course_mp;
public:
    void addStudent() {
        string id, name;
        int year, n;
        float cgpa;
        cin >> id >> name >> year >> cgpa >> n;
        vector<string> courses(n);
        for (int j = 0; j < n; j++)
            cin >> courses[j];
        std_mp[id] = student(id, name, year, cgpa, courses);
        cout << "Student " << id << " has been added!" << endl;
    }
    void addCourse() {
        string id, name;
        int credits, capacity, n;
        char slot;
        cin >> id >> name >> credits >> capacity >> slot >> n;
        vector<string> prereq(n);
        bool valid = true;
        for (int j = 0; j < n; j++) {
            cin >> prereq[j];
            if (course_mp.find(prereq[j]) == course_mp.end()) {
                valid = false;
                cout << "Prerequisite course " << prereq[j] << " doesn't exist!" << endl; 
            }
        }
        if (valid) {
            course_mp[id] = course(id, name, credits, capacity, slot, prereq);
            cout << "Course " << id << " has been added!" << endl;
        }
    }
    void enroll() {
        string student_id, course_id;
        cin >> student_id >> course_id;
        if (std_mp.find(student_id) == std_mp.end() || course_mp.find(course_id) == course_mp.end())
            cout << "Invalid ID!" << endl;
        else if (std_mp[student_id].findcourse(course_id))
            cout << "Student already enrolled or completed this course!" << endl;
        else
            course_mp[course_id].enroll(std_mp[student_id], course_mp);
    }
    void drop() {
        string student_id, course_id;
        cin >> student_id >> course_id;
        if (std_mp.find(student_id) != std_mp.end() && course_mp.find(course_id) != course_mp.end())
            course_mp[course_id].drop(std_mp[student_id], std_mp, course_mp);
        else
            cout << "Invalid ID!" << endl;
    }
    void printCourseDetails() {
        string course_id;
        cin >> course_id;
        if (course_mp.find(course_id) == course_mp.end())
            cout << "Invalid Course!" << endl;
        else
            course_mp[course_id].printcourses();
    }
};
int main() {
    Administration administration;
    string command;
    int queries;
    cin >> queries;
    while (queries--) {
        cin >> command;
        if (command == "add_student")
            administration.addStudent();
        else if (command == "add_course")
            administration.addCourse();
        else if (command == "enroll")
            administration.enroll();
        else if (command == "drop")
            administration.drop();
        else if (command == "ShowCourseDetails")
            administration.printCourseDetails();
    }
}
