#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstdlib>
using namespace std;

// ---------------- Utility ----------------
int getIntInputSafe(const string& prompt) {
    int value;
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        try {
            value = stoi(line);
            break;
        } catch (...) {
            cout << "Invalid input. Please enter a number.\n";
        }
    }
    return value;
}

// ---------------- Patient Structure ----------------
struct Patient {
    int id;
    string name;
    int age;
    string gender;
    string diagnosis;
    string admissionDate;
    string dischargeDate;
    string status;
    stack<string> treatmentHistory;
};

// ---------------- Linked List Node ----------------
struct ListNode {
    Patient data;
    ListNode* next;
};

// ---------------- Admitted Patients List ----------------
class PatientList {
private:
    ListNode* head;
    int nextID;
    vector<int> patientIDs;
public:
    PatientList() { head = nullptr; nextID = 1; loadFromFile(); }
    void addPatient();
    void deletePatient();
    void updatePatient();
    void displayPatients();
    void searchPatient();
    void dischargePatient();   // <-- new
    ListNode* findByID(int id);
    void saveToFile();
    void loadFromFile();
};

// ---------------- ER Patient Queue ----------------
struct ERPatient {
    int id;
    string name;
    int priority;
};

struct ERQueueNode {
    ERPatient data;
    ERQueueNode* next;
};

class ERQueue {
private:
    ERQueueNode* front;
    ERQueueNode* rear;
    int nextERID;
public:
    ERQueue() { front = rear = nullptr; nextERID = 1000; loadFromFile(); }
    void enqueue();
    void dequeue();
    void displayQueue();
    void saveToFile();
    void loadFromFile();
};

// ---------------- Global Objects ----------------
PatientList admittedPatients;
ERQueue erQueue;

// ---------------- Menu Functions ----------------
int mainMenu();
int patientMenu();
int treatmentMenu();
int erMenu();

// ---------------- Main Function ----------------
int main() {
    while (true) {
        system("cls");
        switch(mainMenu()) {
            case 1: {
                while (true) {
                    system("cls");
                    int choice = patientMenu();
                    if (choice == 7) break;  // <-- updated for new discharge option
                    switch(choice) {
                        case 1: admittedPatients.addPatient(); break;
                        case 2: admittedPatients.deletePatient(); break;
                        case 3: admittedPatients.updatePatient(); break;
                        case 4: admittedPatients.displayPatients(); break;
                        case 5: admittedPatients.searchPatient(); break;
                        case 6: admittedPatients.dischargePatient(); break; // <-- new
                        default: cout << "Invalid option.\n"; system("pause");
                    }
                }
                break;
            }
            case 2: {
                while (true) {
                    system("cls");
                    int choice = treatmentMenu();
                    if (choice == 4) break;
                    switch(choice) {
                        case 1: {
                            int id;
                            string treatment;
                            ListNode* patient;
                            id = getIntInputSafe("Enter Patient ID: ");
                            patient = admittedPatients.findByID(id);
                            if (!patient) { cout << "Patient not found.\n"; system("pause"); break; }
                            if (patient->data.status == "Discharged") {
                                cout << "Cannot add treatment. Patient already discharged.\n";
                                system("pause"); break;
                            }
                            cout << "Enter treatment description: ";
                            getline(cin, treatment);
                            patient->data.treatmentHistory.push(treatment);
                            admittedPatients.saveToFile();
                            cout << "Treatment added.\n";
                            system("pause");
                            break;
                        }
                        case 2: {
                            int id;
                            ListNode* patient;
                            id = getIntInputSafe("Enter Patient ID: ");
                            patient = admittedPatients.findByID(id);
                            if (!patient || patient->data.treatmentHistory.empty()) {
                                cout << "No treatment to undo.\n"; system("pause"); break;
                            }
                            cout << "Undo treatment: " << patient->data.treatmentHistory.top() << endl;
                            patient->data.treatmentHistory.pop();
                            admittedPatients.saveToFile();
                            system("pause");
                            break;
                        }
                        case 3: {
                            int id;
                            ListNode* patient;
                            id = getIntInputSafe("Enter Patient ID: ");
                            patient = admittedPatients.findByID(id);
                            if (!patient || patient->data.treatmentHistory.empty()) {
                                cout << "No treatment history.\n"; system("pause"); break;
                            }
                            cout << "Treatment history for " << patient->data.name << ":\n";
                            stack<string> tempStack = patient->data.treatmentHistory;
                            while (!tempStack.empty()) {
                                cout << "- " << tempStack.top() << endl;
                                tempStack.pop();
                            }
                            system("pause");
                            break;
                        }
                        default: cout << "Invalid option.\n"; system("pause");
                    }
                }
                break;
            }
            case 3: {
                while (true) {
                    system("cls");
                    int choice = erMenu();
                    if (choice == 4) break;
                    switch(choice) {
                        case 1: erQueue.enqueue(); break;
                        case 2: erQueue.dequeue(); break;
                        case 3: erQueue.displayQueue(); break;
                        default: cout << "Invalid option.\n"; system("pause");
                    }
                }
                break;
            }
            case 4: exit(0);
            default: cout << "Invalid option.\n"; system("pause");
        }
    }
    return 0;
}

// ---------------- Menu Definitions ----------------
int mainMenu() {
    int choice;
    cout << "===== Hospital Patient System =====\n";
    cout << "1. Patient Management\n";
    cout << "2. Treatment Management\n";
    cout << "3. ER Management\n";
    cout << "4. Exit\n";
    choice = getIntInputSafe("Enter choice: ");
    return choice;
}

int patientMenu() {
    int choice;
    cout << "----- Patient Management -----\n";
    cout << "1. Add Patient\n";
    cout << "2. Delete Patient\n";
    cout << "3. Update Patient Info\n";
    cout << "4. Display All Patients\n";
    cout << "5. Search Patient\n";
    cout << "6. Discharge Patient\n";   // <-- added
    cout << "7. Back\n";                // <-- shifted down
    choice = getIntInputSafe("Enter choice: ");
    return choice;
}

int treatmentMenu() {
    int choice;
    cout << "----- Treatment Management -----\n";
    cout << "1. Add Treatment Record\n";
    cout << "2. Undo Last Treatment\n";
    cout << "3. View Treatment History\n";
    cout << "4. Back\n";
    choice = getIntInputSafe("Enter choice: ");
    return choice;
}

int erMenu() {
    int choice;
    cout << "----- ER Management -----\n";
    cout << "1. Add Patient to ER Queue\n";
    cout << "2. Process Next ER Patient\n";
    cout << "3. Display ER Queue\n";
    cout << "4. Back\n";
    choice = getIntInputSafe("Enter choice: ");
    return choice;
}

// ---------------- PatientList Methods ----------------
void PatientList::addPatient() {
    system("cls");
    Patient p;
    p.id = nextID++;
    cout << "Name: "; getline(cin, p.name);
    p.age = getIntInputSafe("Age: ");
    cout << "Gender: "; getline(cin, p.gender);
    cout << "Diagnosis: "; getline(cin, p.diagnosis);
    cout << "Admission Date: "; getline(cin, p.admissionDate);
    p.dischargeDate = "N/A";
    p.status = "Admitted";

    ListNode* newNode = new ListNode();
    newNode->data = p;
    newNode->next = nullptr;

    if (!head) head = newNode;
    else {
        ListNode* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
    patientIDs.push_back(p.id);
    saveToFile();

    cout << "Patient added with ID: " << p.id << endl;
    system("pause");
}

void PatientList::deletePatient() {
    system("cls");
    int id = getIntInputSafe("Enter Patient ID to delete: ");
    ListNode* temp = head;
    ListNode* prev = nullptr;
    while (temp && temp->data.id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (!temp) { cout << "Patient not found.\n"; system("pause"); return; }
    if (!prev) head = temp->next;
    else prev->next = temp->next;
    delete temp;
    saveToFile();
    cout << "Patient deleted.\n";
    system("pause");
}

void PatientList::updatePatient() {
    system("cls");
    int id = getIntInputSafe("Enter Patient ID to update: ");
    ListNode* p = findByID(id);
    if (!p) { cout << "Patient not found.\n"; system("pause"); return; }

    cout << "Current Diagnosis: " << p->data.diagnosis << endl;
    cout << "Enter new Diagnosis: "; getline(cin, p->data.diagnosis);
    p->data.treatmentHistory.push("Diagnosis updated to: " + p->data.diagnosis);
    saveToFile();
    cout << "Diagnosis updated.\n";
    system("pause");
}

void PatientList::dischargePatient() {
    system("cls");
    int id = getIntInputSafe("Enter Patient ID to discharge: ");
    ListNode* p = findByID(id);

    if (!p) {
        cout << "Patient not found.\n";
        system("pause");
        return;
    }

    if (p->data.status == "Discharged") {
        cout << "Patient is already discharged on " << p->data.dischargeDate << ".\n";
        system("pause");
        return;
    }

    cout << "Enter discharge date (e.g., 2025-09-21): ";
    getline(cin, p->data.dischargeDate);
    p->data.status = "Discharged";
    p->data.treatmentHistory.push("Patient discharged on " + p->data.dischargeDate);

    saveToFile();
    cout << "Patient " << p->data.name << " has been discharged.\n";
    system("pause");
}

void PatientList::displayPatients() {
    system("cls");
    if (!head) { cout << "No admitted patients.\n"; system("pause"); return; }

    cout << left
         << setw(5) << "ID"
         << setw(20) << "Name"
         << setw(5) << "Age"
         << setw(10) << "Gender"
         << setw(20) << "Diagnosis"
         << setw(12) << "Admission"
         << setw(12) << "Discharge"
         << setw(10) << "Status" << endl;

    ListNode* temp = head;
    while (temp) {
        cout << left
             << setw(5) << temp->data.id
             << setw(20) << temp->data.name
             << setw(5) << temp->data.age
             << setw(10) << temp->data.gender
             << setw(20) << temp->data.diagnosis
             << setw(12) << temp->data.admissionDate
             << setw(12) << temp->data.dischargeDate
             << setw(10) << temp->data.status << endl;
        temp = temp->next;
    }
    system("pause");
}

void PatientList::searchPatient() {
    system("cls");
    int choice = getIntInputSafe("Search by:\n1. ID\n2. Name\nEnter choice: ");

    if (choice == 1) {
        int id = getIntInputSafe("Enter ID: ");
        ListNode* p = findByID(id);
        if (!p) {
            cout << "Patient not found.\n";
            system("pause");
            return;
        }
        cout << "Found: " << p->data.name << ", Age: " << p->data.age
             << ", Status: " << p->data.status << endl;
    } else if (choice == 2) {
        string name;
        cout << "Enter name or part: ";
        getline(cin, name);
        ListNode* temp = head;
        bool found = false;
        while (temp) {
            if (temp->data.name.find(name) != string::npos) {
                cout << "Found: " << temp->data.name << ", ID: " << temp->data.id
                     << ", Status: " << temp->data.status << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) cout << "No patient matches.\n";
    } else {
        cout << "Invalid option.\n";
    }
    system("pause");
}

ListNode* PatientList::findByID(int id) {
    ListNode* temp = head;
    while (temp) {
        if (temp->data.id == id) return temp;
        temp = temp->next;
    }
    return nullptr;
}

void PatientList::saveToFile() {
    ofstream fout("patients.txt");
    ListNode* temp = head;
    while (temp) {
        fout << temp->data.id << '|'
             << temp->data.name << '|'
             << temp->data.age << '|'
             << temp->data.gender << '|'
             << temp->data.diagnosis << '|'
             << temp->data.admissionDate << '|'
             << temp->data.dischargeDate << '|'
             << temp->data.status << endl;

        stack<string> th = temp->data.treatmentHistory;
        vector<string> historyVec;
        while (!th.empty()) { historyVec.push_back(th.top()); th.pop(); }
        for (int i = historyVec.size() - 1; i >= 0; i--) {
            fout << historyVec[i];
            if (i > 0) fout << ',';
        }
        fout << endl;
        temp = temp->next;
    }
    fout.close();
}

void PatientList::loadFromFile() {
    ifstream fin("patients.txt");
    if (!fin) return;
    head = nullptr;
    nextID = 1;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        Patient p;
        size_t pos = 0;
        string token;
        vector<string> parts;
        while ((pos = line.find('|')) != string::npos) {
            token = line.substr(0, pos);
            parts.push_back(token);
            line.erase(0, pos + 1);
        }
        parts.push_back(line);
        if (parts.size() < 8) continue;
        p.id = stoi(parts[0]);
        p.name = parts[1];
        p.age = stoi(parts[2]);
        p.gender = parts[3];
        p.diagnosis = parts[4];
        p.admissionDate = parts[5];
        p.dischargeDate = parts[6];
        p.status = parts[7];
        nextID = max(nextID, p.id + 1);

        ListNode* newNode = new ListNode();
        newNode->data = p;
        newNode->next = head;
        head = newNode;

        if (!getline(fin, line)) break;
        stack<string> thStack;
        size_t start = 0;
        size_t end = line.find(',');
        vector<string> thVec;
        while (end != string::npos) {
            thVec.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find(',', start);
        }
        if (start < line.length()) thVec.push_back(line.substr(start));
        for (auto it = thVec.rbegin(); it != thVec.rend(); ++it) thStack.push(*it);
        head->data.treatmentHistory = thStack;
    }
    fin.close();
}

// ---------------- ERQueue Methods ----------------
void ERQueue::enqueue() {
    system("cls");
    ERPatient p;
    cout << "ER Patient Name: "; getline(cin, p.name);
    p.priority = getIntInputSafe("Priority (1-5, 5=highest): ");
    p.id = nextERID++;
    ERQueueNode* newNode = new ERQueueNode();
    newNode->data = p;
    newNode->next = nullptr;
    if (!rear) front = rear = newNode;
    else { rear->next = newNode; rear = newNode; }
    saveToFile();
    cout << "ER Patient added.\n"; system("pause");
}

void ERQueue::dequeue() {
    system("cls");
    if (!front) { cout << "ER Queue empty.\n"; system("pause"); return; }
    ERQueueNode* temp = front;
    front = front->next;
    if (!front) rear = nullptr;
    cout << "Processing ER Patient: " << temp->data.name << endl;
    delete temp;
    saveToFile();
    system("pause");
}

void ERQueue::displayQueue() {
    system("cls");
    if (!front) { cout << "ER Queue empty.\n"; system("pause"); return; }
    cout << left << setw(5) << "No." << setw(25) << "Name" << setw(10) << "Priority" << endl;
    ERQueueNode* temp = front;
    int i = 1;
    while (temp) {
        cout << left << setw(5) << i++
             << setw(25) << temp->data.name
             << setw(10) << temp->data.priority << endl;
        temp = temp->next;
    }
    system("pause");
}

void ERQueue::saveToFile() {
    ofstream fout("erqueue.txt");
    ERQueueNode* temp = front;
    while (temp) {
        fout << temp->data.id << '|'
             << temp->data.name << '|'
             << temp->data.priority << endl;
        temp = temp->next;
    }
    fout.close();
}

void ERQueue::loadFromFile() {
    ifstream fin("erqueue.txt");
    if (!fin) return;
    front = rear = nullptr;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        ERPatient p;
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        if (pos1 == string::npos || pos2 == string::npos) continue;
        p.id = stoi(line.substr(0, pos1));
        p.name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        p.priority = stoi(line.substr(pos2 + 1));
        nextERID = max(nextERID, p.id + 1);
        ERQueueNode* newNode = new ERQueueNode();
        newNode->data = p;
        newNode->next = nullptr;
        if (!rear) front = rear = newNode;
        else { rear->next = newNode; rear = newNode; }
    }
    fin.close();
}
