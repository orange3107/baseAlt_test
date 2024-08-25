#include <iostream>
#include <string>
#include <fstream> 
#include "package_fetcher.h"

int main() {
    std::string branch1, branch2;

    std::cout << "Enter the first branch: ";
    std::getline(std::cin, branch1);

    std::cout << "Enter the second branch: ";
    std::getline(std::cin, branch2);

    PackageFetcher fetcher;

    nlohmann::json data1 = fetcher.fetchBranchData(branch1);
    nlohmann::json data2 = fetcher.fetchBranchData(branch2);

    nlohmann::json result = PackageFetcher::comparePackages(data1, data2);

    std::ofstream file("comparison_result.json");
    if (file.is_open()) {
        file << result.dump(4); // 4 пробела для форматирования
        file.close();
        std::cout << "Comparison result saved to comparison_result.json\n";
    } else {
        std::cerr << "Unable to open file for writing\n";
    }

    return 0;
}
