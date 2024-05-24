#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;

class Department{
public:
    int deptId;
    string deptName;
    Department* next;

    Department(int deptId, string deptName){
        this->deptId = deptId;
        this->deptName = deptName;
        this->next = nullptr;
    }
};

class Student{
public:
    int id;
    string name;
    int age;
    int deptId;
    string regNumber;
    string email; 
    float cgpa;
    string grade;
    Student* next;

    Student(int id, string name, int age, int deptId, string regNumber, string email, float cgpa, string grade) {
        this->id = id;
        this->name = name;
        this->age = age;
        this->deptId = deptId;
        this->regNumber = regNumber;
        this->email = email;
        this->cgpa = cgpa;
        this->grade = grade;
        this->next = nullptr;
    }
};

class StudentManagementSystem{
private:
    Student* studentHead;
    Department* departmentHead;
    queue<Student*> pendingStudentQueue;

public:
    StudentManagementSystem(){
        studentHead = nullptr;
        departmentHead = nullptr;
    }

    ~StudentManagementSystem(){
        Student* currentStudent = studentHead;
        while (currentStudent != nullptr){
            Student* tempStudent = currentStudent;
            currentStudent = currentStudent->next;
            delete tempStudent;
        }

        Department* currentDepartment = departmentHead;
        while (currentDepartment != nullptr){
            Department* tempDepartment = currentDepartment;
            currentDepartment = currentDepartment->next;
            delete tempDepartment;
        }
    }

    void addDepartment(int deptId, string deptName){
        Department* newDepartment = new Department(deptId, deptName);
        if (departmentHead == nullptr){
            departmentHead = newDepartment;
        } else {
            Department* temp = departmentHead;
            while (temp->next != nullptr){
                temp = temp->next;
            }
            temp->next = newDepartment;
        }
        cout << "Department added successfully." << endl;
        writeDepartmentsToFile("departments.txt");
    }


    void addStudent(int id, string name, int age, int deptId, string regNumber, string email, float cgpa, string grade) {
        if (!departmentExists(deptId)){
            cout << "Department ID does not exist." << endl;
            return;
        }
        Student* newStudent = new Student(id, name, age, deptId, regNumber, email, cgpa, grade);
        pendingStudentQueue.push(newStudent);
        cout << "Student added to pending queue." << endl;
    }

    void deleteStudent(int id){
        if (studentHead == nullptr){
            cout << "No students to delete." << endl;
            return;
        }
        if (studentHead->id == id){
            Student* temp = studentHead;
            studentHead = studentHead->next;
            delete temp;
            cout << "Student deleted successfully." << endl;
            writeStudentsToFile("students.txt");
            return;
        }
        Student* temp = studentHead;
        while(temp->next != nullptr && temp->next->id != id){
            temp = temp->next;
        }
        if (temp->next == nullptr){
            cout << "Student not found." << endl;
        }else{
            Student* toDelete = temp->next;
            temp->next = toDelete->next;
            delete toDelete;
            cout << "Student deleted successfully." << endl;
            writeStudentsToFile("students.txt");
        }
    }

    void displayStudents(){
        if(studentHead == nullptr){
            cout << "No students to display." << endl;
            return;
        }
        Student* temp = studentHead;
        while(temp != nullptr){
            cout << "ID: " << temp->id << ", Name: " << temp->name << ", Age: " << temp->age<< ", Department ID: " << temp->deptId << endl;
            temp = temp->next;
        }
    }

    bool departmentExists(int deptId){
        Department* temp = departmentHead;
        while(temp != nullptr){
            if (temp->deptId == deptId){
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    void showStudentsByDepartment(const string& departmentName){
    ifstream file("students.txt");
    if (!file) {
        cerr << "Error opening student file." << endl;
        return;
    }

    bool found = false;
    string line;
    while (getline(file, line)){
        stringstream ss(line);
        string token;
        int id, age, deptId;
        string name, regNumber, email, grade;
        float cgpa;

        if(getline(ss, token, ',')){
            id = stoi(token);
            getline(ss, name, ',');
            getline(ss, token, ',');
            age = stoi(token);
            getline(ss, token, ',');
            deptId = stoi(token);
            getline(ss, regNumber, ',');
            getline(ss, email, ',');
            getline(ss, token, ',');
            cgpa = stof(token);
            getline(ss, grade, ',');

            if(departmentExists(deptId)){
                Department* department = departmentHead;
                while (department != nullptr) {
                    if (department->deptId == deptId && department->deptName == departmentName){
                        cout << "ID: " << id << ", Name: " << name << ", Age: " << age
                             << ", Department ID: " << deptId << endl;
                        found = true;
                        break;
                    }
                    department = department->next;
                }
            }
        }
    }

    file.close();
    if (!found){
        cout << "No students found in the department \"" << departmentName << "\"." << endl;
    }
}


    void readDepartmentsFromFile(const string& filename){
        ifstream file(filename);
        if(!file){
            cerr << "Error opening department file." << endl;
            return;
        }
        string line;
        while(getline(file, line)){
            stringstream ss(line);
            string token;
            int deptId;
            string deptName;
            if(getline(ss, token, ',')){
                deptId = stoi(token);
                if (getline(ss, deptName, ',')){
                    addDepartment(deptId, deptName);
                }
            }
        }
        file.close();
        cout << "Departments loaded from file." << endl;
    }
     
    void readStudentsFromFile(const string& filename){
    ifstream file(filename);
    if(!file){
        cerr << "Error opening student file." << endl;
        return;
    }
    string line;
    while (getline(file, line)){
        stringstream ss(line);
        string token;
        int id, age, deptId;
        string name, regNumber, email;
        float cgpa;
        string grade;
        if(getline(ss, token, ',')){
            id = stoi(token);
            if(getline(ss, name, ',')){
                if(getline(ss, token, ',')){
                    age = stoi(token);
                    if(getline(ss, token, ',')){
                        deptId = stoi(token);
                        if(getline(ss, regNumber, ',')){
                            if(getline(ss, email, ',')) {
                                if(getline(ss, token, ',')){
                                    cgpa = stof(token);
                                    if(getline(ss, token, ',')){
                                        grade = token[0];
                                        addStudent(id, name, age, deptId, regNumber, email, cgpa, grade);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    file.close();
    cout << "Students loaded from file." << endl;
}

    void writeDepartmentsToFile(const string& filename){
        ofstream file(filename);
        if (!file){
            cerr << "Error opening department file for writing." << endl;
            return;
        }
        Department* temp = departmentHead;
        while(temp != nullptr){
            file << temp->deptId << "," << temp->deptName << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Departments saved to file." << endl;
    }

    void writeStudentsToFile(const string& filename){
    ofstream file(filename);
    if (!file){
        cerr << "Error opening student file for writing." << endl;
        return;
    }
    Student* temp = studentHead;
    while (temp != nullptr){
        file << temp->id << "," << temp->name << "," << temp->age << "," << temp->deptId << ","<< temp->regNumber << "," << temp->email << "," << temp->cgpa << "," << temp->grade << endl;
        temp = temp->next;
    }
    file.close();
    cout << "Students saved to file." << endl;
}

    Student* searchStudentById(int id){
    ifstream file("students.txt");
    if (!file){
        cerr << "Error opening student file." << endl;
        return nullptr;
    }

    string line;
    while(getline(file, line)){
        stringstream ss(line);
        string token;
        int studentId;
        string name;
        int age;
        int deptId;
        string regNumber;
        string email;
        float cgpa;
        string grade;

        if(getline(ss, token, ',')){
            studentId = stoi(token);
            if(studentId == id){
                getline(ss, name, ',');
                getline(ss, token, ',');
                age = stoi(token);
                getline(ss, token, ',');
                deptId = stoi(token);
                getline(ss, regNumber, ',');
                getline(ss, email, ',');
                getline(ss, token, ',');
                cgpa = stof(token);
                getline(ss, grade, ',');
                
                file.close();
                return new Student(studentId, name, age, deptId, regNumber, email, cgpa, grade);
            }
        }
    }
    
    file.close(); 
    return nullptr; 
}


void searchStudentsByName(const string& name) {
    ifstream file("students.txt");
    if (!file) {
        cerr << "Error opening student file." << endl;
        return;
    }

    bool found = false;
    string line;
    while(getline(file, line)){
        stringstream ss(line);
        string token;
        int id;
        string studentName;
        int age;
        int deptId;
        string regNumber;
        string email;
        float cgpa;
        string grade;

        if (getline(ss, token, ',')){
            id = stoi(token);
            getline(ss, studentName, ',');
            if(studentName.find(name) != string::npos){
                getline(ss, token, ',');
                age = stoi(token);
                getline(ss, token, ',');
                deptId = stoi(token);
                getline(ss, regNumber, ',');
                getline(ss, email, ',');
                getline(ss, token, ',');
                cgpa = stof(token);
                getline(ss, grade, ',');
                
                cout << "ID: " << id << ", Name: " << studentName << ", Age: " << age
                     << ", Department ID: " << deptId << endl;
                found = true;
            }
        }
    }
    
    file.close();
    if (!found){
        cout << "No students found with the name containing \"" << name << "\"." << endl;
    }
}

    void sortStudentsById(){
        if(studentHead == nullptr || studentHead->next == nullptr){
            return;
        }
        bool swapped;
        do{
            swapped = false;
            Student* temp = studentHead;
            while(temp->next != nullptr){
                if (temp->id > temp->next->id){
                    swap(temp->id, temp->next->id);
                    swap(temp->name, temp->next->name);
                    swap(temp->age, temp->next->age);
                    swap(temp->deptId, temp->next->deptId);
                    swapped = true;
                }
                temp = temp->next;
            }
        }while(swapped);
        cout << "Students sorted by ID." << endl;
    }

    void processPendingStudents(){
        while(!pendingStudentQueue.empty()){
            Student* newStudent = pendingStudentQueue.front();
            pendingStudentQueue.pop();
            if(studentHead == nullptr){
                studentHead = newStudent;
            }else{
                Student* temp = studentHead;
                while(temp->next != nullptr){
                    temp = temp->next;
                }
                temp->next = newStudent;
            }
            cout << "Student added successfully." << endl;
            writeStudentsToFile("students.txt");
            sortStudentsById();
            writeStudentsToFile("students.txt");
        }
    }

     void depthFirstSearchDepartments(){
        if(departmentHead == nullptr){
            cout << "No departments to traverse." << endl;
            return;
        }

        stack<Department*> departmentStack;
        Department* current = departmentHead;
        departmentStack.push(current);

        cout << "Depth-First Search on Departments:" << endl;
        while(!departmentStack.empty()){
            current = departmentStack.top();
            departmentStack.pop();
            cout << "Department ID: " << current->deptId << ", Department Name: " << current->deptName << endl;
            if(current->next != nullptr){
                departmentStack.push(current->next);
            }
        }
    }
};
StudentManagementSystem sms;

int main(){
     
    sms.readDepartmentsFromFile("departments.txt");
    sms.readStudentsFromFile("students.txt");

    int choice, id, age, deptId;
    string name, deptName, regNumber, email;
    float cgpa;
    string grade;

    while (true){
        cout << "\nStudent Management System"<<endl;
        cout << "1. Add Department"<<endl;
        cout << "2. Add Student"<<endl;
        cout << "3. Delete Student"<<endl;
        cout << "4. Display Students"<<endl;
        cout << "5. Search Student by ID"<<endl;
        cout << "6. Search Students by Name"<<endl;
        cout << "7.All Departments"<<endl;
        cout <<"8.Show all student of each department"<<endl;
        cout << "9. Exit"<<endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice){
            case 1:
            cout << "Enter department ID: ";
            cin >> deptId;
            cout << "Enter department name: ";
            cin.ignore();
            getline(cin, deptName);
            sms.addDepartment(deptId, deptName);
            sms.writeDepartmentsToFile("departments.txt");
            break;
            case 2:
                cout << "Enter student ID: ";
                cin >> id;
                cout << "Enter student name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter student age: ";
                cin >> age;
                cout << "Enter department ID: ";
                cin >> deptId;
                cout << "Enter registration number: ";
                cin >> regNumber;
                cout << "Enter email address: ";
                cin >> email;
                cout << "Enter CGPA: ";
                cin >> cgpa;
                cout << "Enter grade: ";
                cin >> grade;
                sms.addStudent(id, name, age, deptId, regNumber, email, cgpa, grade);
                sms.writeStudentsToFile("students.txt");
                sms.processPendingStudents();
                break;
            case 3:
            cout << "Enter student ID to delete: ";
            cin >> id;
            sms.deleteStudent(id);
            break;
        case 4:
            sms.displayStudents();
            break;
        case 5:
            cout << "Enter student ID to search: ";
            cin >> id;
            {
                Student* student = sms.searchStudentById(id);
                if (student){
                    cout << "ID: " << student->id << ", Name: " << student->name << ", Age: " << student->age<< ", Department ID: " << student->deptId << endl;
                }else{
                    cout << "Student not found." << endl;
                }
            }
            break;
        case 6:
            cout << "Enter student name to search: ";
            cin.ignore();
            getline(cin, name);
            sms.searchStudentsByName(name);
            break;
        case 7:
            sms.depthFirstSearchDepartments();
            break;
        case 8:
            cout << "Enter department name to show students: ";
            cin >> deptName;
            sms.showStudentsByDepartment(deptName);
            break;
        case 9:
        return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}
