#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==================================================
//      ER DOCTOR INTERRUPTION SYSTEM USING STACK 
// ==================================================

struct Patient {
    int id;
    string name;
    string condition;
    string severity;
};

class InterruptionStack {
private:
    Patient* holdZone;
    int top;
    int capacity;

public:
    InterruptionStack(int size) {
        capacity = size;
        holdZone = new Patient[capacity];
        top = -1;
    }

    ~InterruptionStack() {
        delete[] holdZone;
    }

    bool isFull() { return top == capacity - 1; }
    bool isEmpty() { return top == -1; }

    void push(Patient p) {
        if (isFull()) {
            cout << " [ERROR] Hold Zone Full! Cannot suspend.\n";
            return;
        }
        holdZone[++top] = p;
    }

    Patient pop() {
        if (isEmpty()) {
            return {-1, "", "", ""};
        }
        return holdZone[top--];
    }

    void displayStack() {
        cout << "  ||  SUSPENDED PATIENTS (STACK)  ||\n";
        cout << "  ||==============================||\n";

        if (isEmpty()) {
            cout << "  ||         (Empty)              ||\n";
        } else {
            for (int i = top; i >= 0; i--) {
                cout << "  || [" << i + 1 << "] " << left << setw(22)
                     << holdZone[i].name << "||\n";
                cout << "  || Reason: " << left << setw(18)
                     << holdZone[i].condition << "||\n";
                cout << "  ||------------------------------||\n";
            }
        }

        cout << "  ||==============================||\n";
    }
};

class EmergencyRoomSystem {
private:
    Patient currentPatient;
    bool hasActivePatient;
    InterruptionStack* stack;

    void clearScreen() {
        cout << "\033[2J\033[1;1H";
    }

public:
    EmergencyRoomSystem(int capacity) {
        stack = new InterruptionStack(capacity);
        hasActivePatient = false;
        currentPatient = {-1, "", "", ""};
    }

    ~EmergencyRoomSystem() {
        delete stack;
    }

    void admitPatient() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string name, condition, severity;
        int id = rand() % 9000 + 1000;

        cout << "\n >>> INCOMING EMERGENCY <<<\n";
        cout << " Enter Patient Name: ";
        getline(cin, name);
        cout << " Enter Condition: ";
        getline(cin, condition);
        cout << " Severity (Critical/Severe): ";
        getline(cin, severity);

        Patient newP = {id, name, condition, severity};

        if (hasActivePatient) {
            cout << "\n [!] INTERRUPTION: Suspending " << currentPatient.name << "...\n";
            stack->push(currentPatient);
        }

        currentPatient = newP;
        hasActivePatient = true;
        cout << " [SUCCESS] Doctor now treating " << currentPatient.name << ".\n";
    }

    void dischargeCurrent() {
        if (!hasActivePatient) {
            cout << "\n [INFO] No active patient.\n";
            return;
        }

        cout << "\n [?] COMPLETED: Discharging " << currentPatient.name << ".\n";
        hasActivePatient = false;

        if (!stack->isEmpty()) {
            currentPatient = stack->pop();
            hasActivePatient = true;
            cout << " [RESUME] Retrieved " << currentPatient.name << " from stack.\n";
        } else {
            cout << " [INFO] No suspended patients.\n";
        }
    }

    void visualize() {
        clearScreen();
        cout << "==============================================\n";
        cout << "     ER DOCTOR STACK INTERRUPTION SYSTEM      \n";
        cout << "==============================================\n\n";

        cout << "       __________________________\n";
        cout << "      /                          \\\n";

        if (hasActivePatient) {
            cout << "      |   Currently Treating:    |\n";
            cout << "      | " << left << setw(23) << currentPatient.name << "|\n";
            cout << "      | " << left << setw(23) << currentPatient.severity << "|\n";
        } else {
            cout << "      |      DOCTOR IS IDLE      |\n";
            cout << "      |  Waiting for new patient |\n";
            cout << "      |                          |\n";
        }

        cout << "      \\__________________________/\n\n";

        stack->displayStack();

        cout << "\n==============================================\n";
    }

    void run() {
        int choice;

        while (true) {
            visualize();
            cout << "\n 1. New Emergency Patient (PUSH)\n";
            cout << " 2. Treatment Complete (POP)\n";
            cout << " 3. Exit\n";
            cout << " Select: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
                case 1: admitPatient(); break;
                case 2: dischargeCurrent(); break;
                case 3: return;
            }

            cout << "\n Press Enter...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
};

int main() {
    srand(time(0));

    EmergencyRoomSystem er(5);
    er.run();

    return 0;
}