#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <direct.h> // for _mkdir on Windows

using namespace std;

// Function Prototypes
void inputNames(string* names, int count);
void inputGrades(float* grades, int count);
float getValidatedGrade(int studentNumber);
void displayStudentData(string* names, float* grades, int count);
float calculateAverage(float* grades, int count);
string generateUniqueFilename();
void saveToFile(string* names, float* grades, int count, const string& filename);
void displayFromFile(const string& filename);
void deleteFile(const string& filename);
void renameFile(const string& oldFilename, string& newFilename);
void listSavedFiles(vector<string>& fileList);
void editFile(const string& filename); // <-- Added prototype

int main() {
    _mkdir("saved_files"); // Create folder if it doesn't exist

    int choice;
    string lastFilename = "";

    do {
        system("cls");
        cout << "===== Student Info Menu =====\n";
        cout << "[1] Input and Save Student Data\n";
        cout << "[2] View Saved File\n";
        cout << "[3] Delete Saved File\n";
        cout << "[4] Rename Saved File\n";
        cout << "[5] Edit Saved File\n"; // <-- Added option
        cout << "[6] Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            int count;
            cout << "Enter number of students: ";
            while (!(cin >> count) || count <= 0) {
                cout << "Invalid input. Enter a positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore();

            string* names = new string[count];
            float* grades = new float[count];

            inputNames(names, count);
            inputGrades(grades, count);

            cout << "\n=== Student Records ===\n";
            displayStudentData(names, grades, count);

            float avg = calculateAverage(grades, count);
            cout << fixed << setprecision(2);
            cout << "\nAverage Grade: " << avg << endl;

            lastFilename = generateUniqueFilename();
            saveToFile(names, grades, count, lastFilename);

            time_t now = time(0);
            cout << "Saved at: " << ctime(&now);

            delete[] names;
            delete[] grades;

            system("pause");
        }

        else if (choice == 2) {
            vector<string> files;
            listSavedFiles(files);
            if (files.empty()) {
                cout << "No saved files found.\n";
            } else {
                int sel;
                cout << "Select a file to view:\n";
                for (int i = 0; i < files.size(); i++) {
                    cout << "[" << i + 1 << "] " << files[i] << endl;
                }
                cout << "Enter number: ";
                cin >> sel;
                cin.ignore();
                if (sel > 0 && sel <= files.size()) {
                    displayFromFile(files[sel - 1]);
                } else {
                    cout << "Invalid selection.\n";
                }
            }
            system("pause");
        }

        else if (choice == 3) {
            vector<string> files;
            listSavedFiles(files);
            if (files.empty()) {
                cout << "No saved files found.\n";
            } else {
                int sel;
                cout << "Select a file to delete:\n";
                for (int i = 0; i < files.size(); i++) {
                    cout << "[" << i + 1 << "] " << files[i] << endl;
                }
                cout << "Enter number: ";
                cin >> sel;
                cin.ignore();
                if (sel > 0 && sel <= files.size()) {
                    deleteFile(files[sel - 1]);
                } else {
                    cout << "Invalid selection.\n";
                }
            }
            system("pause");
        }

        else if (choice == 4) {
            vector<string> files;
            listSavedFiles(files);
            if (files.empty()) {
                cout << "No saved files found.\n";
            } else {
                int sel;
                cout << "Select a file to rename:\n";
                for (int i = 0; i < files.size(); i++) {
                    cout << "[" << i + 1 << "] " << files[i] << endl;
                }
                cout << "Enter number: ";
                cin >> sel;
                cin.ignore();
                if (sel > 0 && sel <= files.size()) {
                    string newFilename;
                    renameFile(files[sel - 1], newFilename);
                } else {
                    cout << "Invalid selection.\n";
                }
            }
            system("pause");
        }

        else if (choice == 5) { // <-- Edit option
            vector<string> files;
            listSavedFiles(files);
            if (files.empty()) {
                cout << "No saved files found.\n";
            } else {
                int sel;
                cout << "Select a file to edit:\n";
                for (int i = 0; i < files.size(); i++) {
                    cout << "[" << i + 1 << "] " << files[i] << endl;
                }
                cout << "Enter number: ";
                cin >> sel;
                cin.ignore();
                if (sel > 0 && sel <= files.size()) {
                    editFile(files[sel - 1]);
                } else {
                    cout << "Invalid selection.\n";
                }
            }
            system("pause");
        }

        else if (choice == 6) {
            cout << "Exiting the program.\n";
        }

        else {
            cout << "Invalid choice.\n";
            system("pause");
        }

    } while (choice != 6);

    return 0;
}

void inputNames(string* names, int count) {
    for (int i = 0; i < count; i++) {
        cout << "Enter name for student " << (i + 1) << ": ";
        getline(cin, names[i]);
    }
}

void inputGrades(float* grades, int count) {
    for (int i = 0; i < count; i++) {
        grades[i] = getValidatedGrade(i + 1);
    }
}

float getValidatedGrade(int studentNumber) {
    float grade;
    while (true) {
        cout << "Enter grade for student " << studentNumber << ": ";
        cin >> grade;
        if (!cin.fail() && grade >= 0 && grade <= 100) {
            break;
        } else {
            cout << "Invalid input. Enter a number between 0 and 100.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore();
    return grade;
}

void displayStudentData(string* names, float* grades, int count) {
    cout << left << setw(20) << "Name" << setw(10) << "Grade" << endl;
    for (int i = 0; i < 30; i++) cout << "-";
    cout << endl;
    for (int i = 0; i < count; i++) {
        cout << left << setw(20) << names[i] << setw(10) << grades[i] << endl;
    }
}

float calculateAverage(float* grades, int count) {
    float sum = 0;
    for (int i = 0; i < count; i++) {
        sum += grades[i];
    }
    return sum / count;
}

string generateUniqueFilename() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << "saved_files\\students_";
    ss << (1900 + ltm->tm_year);
    ss << (1 + ltm->tm_mon);
    ss << ltm->tm_mday << "_";
    ss << ltm->tm_hour << ltm->tm_min << ltm->tm_sec;
    ss << ".txt";
    return ss.str();
}

void saveToFile(string* names, float* grades, int count, const string& filename) {
    ofstream outFile(filename.c_str());
    if (!outFile) {
        cout << "Error opening file.\n";
        return;
    }

    outFile << "Name,Grade\n";
    for (int i = 0; i < count; i++) {
        outFile << names[i] << "," << grades[i] << "\n";
    }

    outFile.close();
    cout << "Data saved to " << filename << endl;
}

void displayFromFile(const string& filename) {
    ifstream inFile(filename.c_str());
    if (!inFile) {
        cout << "Error: File not found.\n";
        return;
    }

    string line;
    cout << "\n=== Contents of " << filename << " ===\n";
    while (getline(inFile, line)) {
        cout << line << endl;
    }

    inFile.close();
}

void deleteFile(const string& filename) {
    if (remove(filename.c_str()) == 0) {
        cout << "File \"" << filename << "\" deleted successfully.\n";
    } else {
        cout << "Error deleting file.\n";
    }
}

void renameFile(const string& oldFilename, string& newFilename) {
    string newName;
    cout << "Enter new filename (without extension): ";
    getline(cin, newName);
    newFilename = "saved_files\\" + newName + ".txt";

    if (rename(oldFilename.c_str(), newFilename.c_str()) == 0) {
        cout << "File renamed to " << newName << ".txt" << endl;
    } else {
        cout << "Error renaming file.\n";
    }
}

void listSavedFiles(vector<string>& fileList) {
    DIR* dir;
    struct dirent* ent;
    fileList.clear();

    dir = opendir("saved_files");
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            string name = ent->d_name;

            // Filter for .txt files only
            if (name.length() > 4 && name.substr(name.length() - 4) == ".txt") {
                string fullPath = "saved_files\\" + name;
                fileList.push_back(fullPath);
            }
        }
        closedir(dir);
    } else {
        cout << "Unable to open directory 'saved_files'.\n";
    }
}

// --- Added function for editing student names and grades ---
void editFile(const string& filename) {
    ifstream inFile(filename.c_str());
    if (!inFile) {
        cout << "Error: File not found.\n";
        return;
    }

    vector<string> names;
    vector<float> grades;
    string line;
    getline(inFile, line); // Skip header

    while (getline(inFile, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            names.push_back(line.substr(0, pos));
            grades.push_back(static_cast<float>(atof(line.substr(pos + 1).c_str())));
        }
    }
    inFile.close();

    for (size_t i = 0; i < names.size(); i++) {
        cout << "Current name for student " << (i + 1) << ": " << names[i] << endl;
        cout << "Enter new name (or press Enter to keep): ";
        string newName;
        getline(cin, newName);
        if (!newName.empty()) names[i] = newName;

        cout << "Current grade for student " << (i + 1) << ": " << grades[i] << endl;
        cout << "Enter new grade (or press Enter to keep): ";
        string gradeInput;
        getline(cin, gradeInput);
        if (!gradeInput.empty()) {
            float newGrade = static_cast<float>(atof(gradeInput.c_str()));
            if (newGrade >= 0 && newGrade <= 100) grades[i] = newGrade;
            else cout << "Invalid grade. Keeping previous value.\n";
        }
    }

    ofstream outFile(filename.c_str());
    outFile << "Name,Grade\n";
    for (size_t i = 0; i < names.size(); i++) {
        outFile << names[i] << "," << grades[i] << "\n";
    }
    outFile.close();

    cout << "File updated.\n";
}
