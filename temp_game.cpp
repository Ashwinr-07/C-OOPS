#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
namespace calculations
{
    class Appraisal {
        private:
            double value;

        public:
            Appraisal(double value) : value(value) {}

            operator double() const {
                return value;
            }

            bool operator>=(const Appraisal& other) const {
                return value >= other.value;
            }

            bool operator<=(const Appraisal& other) const {
                return value <= other.value;
            }
    };

void checkYearlyAppraisal(int employeeID) {
    std::ifstream file("final.txt");

    if (!file.is_open()) {
        std::cout << "HR is yet to calculate appraisal. Please wait." << std::endl;
        return;
    }

    std::string line;
    bool appraisalFound = false;

    while (std::getline(file, line)) {
        // Find the line with the employee ID
        size_t idPos = line.find("Employee ID:");
        if (idPos != std::string::npos) {
            std::string idStr = line.substr(idPos + 13);
            int id = std::stoi(idStr);

            if (id == employeeID) {
                // Extract the appraisal value
                size_t appraisalPos = line.find("Appraisal:");
                if (appraisalPos != std::string::npos) {
                    std::string appraisalStr = line.substr(appraisalPos + 10);
                    double appraisal = std::stod(appraisalStr);

                    appraisalFound = true;
                    Appraisal appraisalObj(appraisal);

                    // Calculate final appraisal for the employee
                    if (appraisalObj >= Appraisal(0) && appraisalObj <= Appraisal(5)) {
                        std::cout << "Congrats for 5% appraisal" << std::endl;
                    }
                    else if (appraisalObj > Appraisal(5) && appraisalObj <= Appraisal(7.5)) {
                        std::cout << "Congrats for 10% appraisal" << std::endl;
                    }
                    else if (appraisalObj > Appraisal(7.5) && appraisalObj <= Appraisal(10)) {
                        std::cout << "Congrats for 15% appraisal" << std::endl;
                    }
                    else {
                        std::cout << "Invalid appraisal value." << std::endl;
                    }

                    break;  // No need to continue searching
                }
            }
        }
    }

    if (!appraisalFound) {
        std::cout << "No appraisal found for the employee ID: " << employeeID << std::endl;
    }

    file.close();
}

    // Function to calculate appraisal for employees
void calculateAppraisal(std::unordered_map<int, std::string>& hrEmployees) {
    std::ifstream bossRatingsFile("boss_ratings.txt");
    std::ofstream finalFile("final.txt");

    if (!bossRatingsFile) {
        std::cout << "Error opening boss_ratings.txt file.\n";
        return;
    }

    if (!finalFile) {
        std::cout << "Error creating final.txt file.\n";
        return;
    }

    std::string line;
    std::string employeeName;
    int employeeId = 0;
    bool isEmployeeLine = false;

    while (std::getline(bossRatingsFile, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (line.find("Employee: ") != std::string::npos) {
            isEmployeeLine = true;
            employeeName = line.substr(line.find(": ") + 2);
            ++employeeId;
            continue;
        }

        if (isEmployeeLine && std::getline(iss, key, ':') && std::getline(iss, value)) {
            if (key.find("Task ") != std::string::npos) {
                int taskNumber = std::stoi(key.substr(key.find("Task ") + 5));
                hrEmployees[employeeId] += value + ",";
            }
        }
    }

    for (const auto& employee : hrEmployees) {
        std::string taskRatings = employee.second;
        taskRatings.pop_back();  // Remove the trailing comma

        std::istringstream taskRatingsStream(taskRatings);
        std::vector<int> ratings;
        std::string rating;
        while (std::getline(taskRatingsStream, rating, ',')) {
            try {
                
                int ratingValue = std::stoi(rating);
                ratings.push_back(ratingValue);
            } catch (const std::invalid_argument& e) {
                
                continue;
            }
        }

        int weightSum = 0;
        int weightedRatingSum = 0;
        int weight = 1;
        for (int i = 0; i < ratings.size(); ++i) {
            weightSum += weight;
            weightedRatingSum += weight * ratings[i];
            ++weight;
        }

        float appraisal = static_cast<float>(weightedRatingSum) / weightSum;

        finalFile << "Employee ID: " << std::setw(4) << employee.first << " | Appraisal: " << std::fixed << std::setprecision(2) << appraisal << std::endl;
    }

    bossRatingsFile.close();
    finalFile.close();

    std::cout << "Appraisals calculated and written to final.txt file.\n";
}

}
