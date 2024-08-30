// compare_version.cpp

#include "compare_versions.h"
#include <iostream>
#include <cctype>

std::vector<std::string> splitVersion(const std::string& version) {
    std::vector<std::string> result;
    std::string currentPart;
    char tempC = version[0];
    int i = 0;

    for (char c : version) {
        tempC = version[i + 1];
        ++i;

        if (std::isalnum(c)) {
            currentPart += c;
        }

        if (!(isdigit(c) && isdigit(tempC)) && !(isalpha(c) && isalpha(tempC))) {
            if (isalnum(currentPart[0])) result.push_back(currentPart);
            currentPart.clear();
            continue;
        }
    }

    return result;
}

int compareVersionParts(const std::string& part1, const std::string& part2) {
    try {
        int num1 = std::stoi(part1);
        int num2 = std::stoi(part2);

        if (num1 < num2) return -1;
        if (num1 > num2) return 1;
    } catch (const std::invalid_argument&) {
        if (!(isdigit(part1[0]) && isdigit(part2[0])) && !(isalpha(part1[0]) && isalpha(part2[0]))) {
            if (isalpha(part1[0])) return -1;
            if (isalpha(part2[0])) return 1;
        }

        if (part1 < part2) return -1;
        if (part1 > part2) return 1;
    }

    return 0;
}

int compareRpmVersions(const std::string& version1, const std::string& release1, 
                       const std::string& version2, const std::string& release2) {
    std::vector<std::string> parts1 = splitVersion(version1);
    std::vector<std::string> parts2 = splitVersion(version2);

    for (size_t i = 0; i < std::min(parts1.size(), parts2.size()); ++i) {
        int cmp = compareVersionParts(parts1[i], parts2[i]);
        if (cmp != 0) return cmp;
    }

    if (parts1.size() < parts2.size()) return -1;
    if (parts1.size() > parts2.size()) return 1;

    return compareVersionParts(release1, release2);
}
