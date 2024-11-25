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
    string student_id;
    string name;
    int year;
    unordered_set<string> completed_courses;
    unordered_set<string> enrolled_courses;
    float cgpa;
    int completed_credits = 0;
public:
    student() : year(0) {}
    student(string id_inp, string name_inp, int y_inp, float cg, vector<string> courses) {
        student_id = id_inp;
        name = name_inp;
        year = y_inp;
        cgpa = cg;

        for (int i = 0; i < courses.size(); i++)
            completed_courses.insert(courses[i]);
    }
    bool findcourse(const string& course_code) {
        return (completed_courses.find(course_code) != completed_courses.end()) || (enrolled_courses.find(course_code) != enrolled_courses.end());
    }
    friend class course;
};

class course {
    string course_code;
    string course_name;
    int credits;
    int capacity;
    unordered_set<string> prerequisites;
    char time_slot;
    vector<string> enrolled_students; 
    queue<string> waitlist;
public:
    course() : credits(0), capacity(0), time_slot(' ') {}
    course(string cc_inp, string name_inp, int credit_inp, int capacity_inp, char slot_inp, vector<string> prereq) {
        course_code = cc_inp;
        course_name = name_inp;
        credits = credit_inp;
        capacity = capacity_inp;
        time_slot = slot_inp;

        for (int i = 0; i < prereq.size(); i++)
            prerequisites.insert(prereq[i]);
    }
    void enroll(student& s, unordered_map<string, course>& course_mp) {
        for (auto pre : prerequisites) {
            if (s.completed_courses.find(pre) == s.completed_courses.end())
                return; 
        }
        for (auto curr : s.enrolled_courses) {
            if (course_mp[curr].time_slot == time_slot)
                return; 
        }
        if (s.cgpa < 8 && s.completed_credits + credits > 66)
            return;
        if (s.cgpa >= 8 && s.completed_credits + credits > 72)
            return;
        if (enrolled_students.size() == capacity) {
            waitlist.push(s.student_id);
            return; 
        }
        enrolled_students.push_back(s.student_id); 
        s.completed_credits += credits;
        s.enrolled_courses.insert(course_code);
    }
    void drop(student& s, unordered_map<string, student>& std_mp, unordered_map<string, course>& course_mp) {
        if (s.enrolled_courses.find(course_code) == s.enrolled_courses.end())
            return;

        auto it = find(enrolled_students.begin(), enrolled_students.end(), s.student_id);
        if (it != enrolled_students.end()) {
            enrolled_students.erase(it);
        }
        s.enrolled_courses.erase(course_code);
        s.completed_credits -= credits;
        
        while (!waitlist.empty()) {
            string next_student_id = waitlist.front();
            waitlist.pop();
            bool has_conflict = false;
            for (auto enrolled_course : std_mp[next_student_id].enrolled_courses) {
                if (course_mp[enrolled_course].time_slot == time_slot) {
                    has_conflict = true;
                    break;
                }
            }
            if (!has_conflict) {
                enrolled_students.push_back(next_student_id);
                std_mp[next_student_id].enrolled_courses.insert(course_code);
                break; 
            }
        }
    }
    bool findprereqcourse(const string& course_code) {
        return prerequisites.find(course_code) != prerequisites.end();
    }
    void printcourses() {
        cout << "Enrolled students in " << course_code << ":" << endl;
        for (const auto& st : enrolled_students)
            cout << st << " ";
        cout << endl;
    }
    friend class student;
};

int main() {
    unordered_map<string, student> std_mp;
    unordered_map<string, course> course_mp;
    string command;
    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        cin >> command;
        if (command == "add_student") {
            string id_inp, name_inp;
            int y_inp, n;
            float cg_inp;
            cin >> id_inp >> name_inp >> y_inp >> cg_inp >> n;
            vector<string> courses(n);
            for (int j = 0; j < n; j++) {
                cin >> courses[j];
            }
            std_mp[id_inp] = student(id_inp, name_inp, y_inp, cg_inp, courses);
        } else if (command == "add_course") {
            string id_inp, name_inp;
            int credit_inp, capacity_inp, n;
            char slot_inp;
            cin >> id_inp >> name_inp >> credit_inp >> capacity_inp >> slot_inp >> n;
            vector<string> prereq(n);
            bool valid = true;
            for (int j = 0; j < n; j++) {
                cin >> prereq[j];
                if (course_mp.find(prereq[j]) == course_mp.end()) {
                    valid = false;
                }
            }
            if (!valid)
                continue;
            course_mp[id_inp] = course(id_inp, name_inp, credit_inp, capacity_inp, slot_inp, prereq);
        } else if (command == "enroll") {
            string student_id, course_id;
            cin >> student_id >> course_id;
            if (std_mp.find(student_id) == std_mp.end() || course_mp.find(course_id) == course_mp.end())
                continue;
            if (std_mp[student_id].findcourse(course_id))
                continue;
            course_mp[course_id].enroll(std_mp[student_id], course_mp);
        } else if (command == "drop") {
            string student_id, course_id;
            cin >> student_id >> course_id;
            if (std_mp.find(student_id) == std_mp.end() || course_mp.find(course_id) == course_mp.end())
                continue;
            course_mp[course_id].drop(std_mp[student_id], std_mp, course_mp);
        } else {
            string course_id;
            cin >> course_id;
            if (course_mp.find(course_id) == course_mp.end()) {
                cout << "Invalid Course " << course_id << endl;
            } else {
                course_mp[course_id].printcourses();
            }
        }
    }
}
