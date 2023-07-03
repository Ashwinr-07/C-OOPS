#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include "names.h"

class tasker
{
    public:
        int task1;
        int task2;
        int task3;
        int task4;

    friend void task(tasker&);

};

void task(tasker&obj)
{
    std::cout << "Rate task 1 (out of 10): ";
            std::cin >> obj.task1;
            std::cout << "Rate task 2 (out of 10): ";
            std::cin >> obj.task2;
            std::cout << "Rate task 3 (out of 10): ";
            std::cin >> obj.task3;
            std::cout << "Rate task 4 (out of 10): ";
            std::cin >> obj.task4;
}



// Function to check if a username and password combination exists in the map
bool checkCredentials(const std::unordered_map<std::string, std::string>& credentials, const std::string& username, const std::string& password) {
    auto it = credentials.find(username);
    if (it != credentials.end() && it->second == password) {
        return true;
    }
    return false;
}



// Function to read credentials from the file and store them in a map
std::unordered_map<std::string, std::string> readCredentialsFromFile(const std::string& filename) {
    std::unordered_map<std::string, std::string> credentials;
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line)) {
        std::string username = line.substr(0, line.find(' '));
        std::string password = line.substr(line.find(' ') + 1);
        credentials[username] = password;
    }
    file.close();
    return credentials;
}

bool hasFilledSelfAppraisal(const std::string& username) {
    std::string formattedUsername = std::string(1, std::toupper(username[0])) + username.substr(1);
  size_t pos = formattedUsername.find_first_of("0123456789");
  if (pos != std::string::npos) {
    formattedUsername.insert(pos, " ");
  }

    std::ifstream file("self_appraisal.txt");
    std::string line;
    while (getline(file, line)) {
        if (line == formattedUsername) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}


void updateSelfAppraisal(const std::string& username, int task1Rating, int task2Rating, int task3Rating, int task4Rating) {
  std::string formattedUsername = std::string(1, std::toupper(username[0])) + username.substr(1);
  size_t pos = formattedUsername.find_first_of("0123456789");
  if (pos != std::string::npos) {
    formattedUsername.insert(pos, " ");
  }
  std::ofstream file("self_appraisal.txt", std::ios_base::app);
  file << formattedUsername << '\n';
  file << task1Rating << '\n';
  file << task2Rating << '\n';
  file << task3Rating << '\n';
  file << task4Rating << '\n';
  file.close();
}



// Function to check if the boss has already rated an employee
bool hasBossRatedEmployee(const std::string& employeeName) {
    std::ifstream file("boss_ratings.txt");
    std::string line;
    while (getline(file, line)) {
        if (line.find("Employee: " + employeeName) != std::string::npos) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Function to login as an employee
void loginAsEmployee(const std::unordered_map<std::string, std::string>& employeeCredentials) {
    std::string username, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;
    char empid;
    empid=password.back();
    int empd=atoi(&empid);
    

    if (checkCredentials(employeeCredentials, username, password)) 
    {
        std::cout << "Successfully logged in as employee: " << username << std::endl;

        int a1=0;
        
        std::cout << "Options:" << std::endl;
        std::cout << "1) Fill in self appraisal" << std::endl;
        std::cout << "2) Check appraisal for the year" << std::endl;

        int option;
        std::cout << "Enter your choice (1 or 2): ";
        
        if (!(std::cin >> option)) {
        // Input extraction failed (non-integer input)
        std::cin.clear(); // Clear the fail state
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
        std::cout << "Invalid input. Please enter a valid integer." << std::endl;
        }
        else{
       

        if (option == 1)
        {
            if (hasFilledSelfAppraisal(username)) {
                std::cout << "You have already filled the self-appraisal form.\n";
            } 
            else {
            int task1Rating, task2Rating, task3Rating, task4Rating;
            std::cout << "Self-appraisal form:\n";
            tasker obj;
            task(obj);
            std::cout<<obj.task1;
            updateSelfAppraisal(username, obj.task1, obj.task2,obj.task3, obj.task4);

            std::cout << "Self-appraisal form submitted successfully.\n";
            }
        }
        else if (option == 2)
        {calculations::checkYearlyAppraisal(empd);}
        
        else
        {
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    }
    } 
    else 
    {
        std::cout << "Invalid username or password.\n";
    }
}

// Function to login as a boss
void loginAsBoss(const std::unordered_map<std::string, std::string>& bossCredentials) {
    std::string username, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (checkCredentials(bossCredentials, username, password)) {
        std::cout << "Successfully logged in as boss: " << username << std::endl;

        std::unordered_map<int, std::string> bossEmployees = {
            {1, "Employee 1"},
            {2, "Employee 2"},
            {3, "Employee 3"},
            {4, "Employee 4"}
        };

        std::unordered_map<int, std::string> boss1Employees = {
            {1, "Employee 1"},
            {2, "Employee 2"}
        };

        std::unordered_map<int, std::string> boss2Employees = {
            {3, "Employee 3"},
            {4, "Employee 4"}
        };

        std::unordered_map<int, std::string> selectedEmployees;

        if (username == "boss1") {
            selectedEmployees = boss1Employees;
        } else if (username == "boss2") {
            selectedEmployees = boss2Employees;
        }

        std::cout << "Employees under your supervision:\n";
        for (const auto& entry : selectedEmployees) {
            std::cout << entry.first << ". " << entry.second << '\n';
        }

        int employeeChoice;
        std::cout << "Enter the employee number you want to rate: ";
        std::cin >> employeeChoice;

        auto it = selectedEmployees.find(employeeChoice);
        if (it != selectedEmployees.end()) {
            std::string employeeName = it->second;

            if (hasBossRatedEmployee(employeeName)) {
                std::cout << "You have already rated " << employeeName << ".\n";
            } else 
            {
                std::cout << "Rate " << employeeName << " for the following tasks:\n";
                tasker obj2;
                task(obj2);
                std::ofstream file("boss_ratings.txt", std::ios_base::app);
                file << "Employee: " << employeeName << '\n';
                file << "Task 1: " << obj2.task1 << '\n';
                file << "Task 2: " << obj2.task2 << '\n';
                file << "Task 3: " << obj2.task3 << '\n';
                file << "Task 4: " << obj2.task4 << '\n';
                file.close();

                std::cout << "Ratings for " << employeeName << " submitted successfully.\n";
            }
        } else {
            std::cout << "Invalid employee choice.\n";
        }
    } else {
        std::cout << "Invalid username or password.\n";
    }
}


// Function to login as an HR
void loginAsHR(const std::unordered_map<std::string, std::string>& hrCredentials) {
    std::string username, password;
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (checkCredentials(hrCredentials, username, password)) {
        std::cout << "Successfully logged in as HR: " << username << std::endl;

        std::unordered_map<int, std::string> hrEmployees = {
            {1, "Employee 1"},
            {2, "Employee 2"},
            {3, "Employee 3"},
            {4, "Employee 4"}
        };

        

        int choice;
        
        std::cout << "Select an option:\n";
        std::cout << "1. Calculate the appraisal for the employees\n";
        std::cout << "2. View ratings for an employee\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            // Option 1: Calculate the appraisal for the employees
            calculations::calculateAppraisal(hrEmployees);
        } else if (choice == 2) {
            std::cout << "Employees:\n";
        for (const auto& entry : hrEmployees) {
            std::cout << entry.first << ". " << entry.second << '\n';
        }
            // Option 2: View ratings for an employee
            int employeeChoice;
        std::cout << "Enter the employee number you want to view ratings for: ";
        std::cin >> employeeChoice;

        auto it = hrEmployees.find(employeeChoice);
        if (it != hrEmployees.end()) {
            std::string employeeName = it->second;

            std::ifstream selfAppraisalFile("self_appraisal.txt");
            std::ifstream bossRatingsFile("boss_ratings.txt");

            std::string line;
            bool foundEmployee = false;

            std::cout << "Ratings for " << employeeName << ":\n";
            std::cout << "-----------------------\n";

            // Search self-appraisal file
            std::cout << "Self-appraisal ratings:\n";
            while (getline(selfAppraisalFile, line)) {
                
                if (line.find(employeeName) != std::string::npos) {
                    
                    foundEmployee = true;
                    std::cout << "Task 1: ";
                    getline(selfAppraisalFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    std::cout << "Task 2: ";
                    getline(selfAppraisalFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    std::cout << "Task 3: ";
                    getline(selfAppraisalFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    std::cout << "Task 4: ";
                    getline(selfAppraisalFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    break;
                }
                
            }

            if (!foundEmployee) {
                std::cout << "No self-appraisal ratings found for " << employeeName << '\n';
            }

            foundEmployee = false;

            // Search boss ratings file
            std::cout << "Boss ratings:\n";
            while (getline(bossRatingsFile, line)) {
                if (line.find("Employee: " + employeeName) != std::string::npos) {
                    foundEmployee = true;
                    std::cout << "Task 1: ";
                    getline(bossRatingsFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    std::cout << "Task 2: ";
                    getline(bossRatingsFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    std::cout << "Task 3: ";
                    getline(bossRatingsFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    std::cout << "Task 4: ";
                    getline(bossRatingsFile, line);
                    std::cout << "    Rating: " << line << '\n';
                    break;
                }
            }

            if (!foundEmployee) {
                std::cout << "No boss ratings found for " << employeeName << '\n';
            }

            selfAppraisalFile.close();
            bossRatingsFile.close();
        } else {
            std::cout << "Invalid employee choice.\n";
        }
        } else {
            std::cout << "Invalid choice.\n";
        }
    } else {
        std::cout << "Invalid username or password.\n";
    }
}


int main() {
    std::unordered_map<std::string, std::string> employeeCredentials = readCredentialsFromFile("employee_credentials.txt");
    std::unordered_map<std::string, std::string> bossCredentials = readCredentialsFromFile("boss_credentials.txt");
    std::unordered_map<std::string, std::string> hrCredentials = readCredentialsFromFile("hr_credentials.txt");

    std::string role;
    int a=0;
    while(a==0)
    {
        std::cout << "Enter your role (employee/boss/hr): or e to exit ";    
        std::cin >> role;

        if (role == "employee") {
            loginAsEmployee(employeeCredentials);
        }
        else if (role == "boss") {
            loginAsBoss(bossCredentials);
        }
        
        else if (role == "hr") {
        loginAsHR(hrCredentials);
        }
        else if(role=="e"){a=1;}
        else {
        std::cout << "Invalid role.\n";
        }   

    }    

     return 0;

}