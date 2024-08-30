// compare_version.h

#ifndef COMPARE_VERSIONS_H
#define COMPARE_VERSIONS_H

#include <string>
#include <vector>

// Функция для разбиения строки на числа и строки
std::vector<std::string> splitVersion(const std::string& version);

// Функция для сравнения двух частей версии
int compareVersionParts(const std::string& part1, const std::string& part2);

// Основная функция сравнения версий
int compareRpmVersions(const std::string& version1, const std::string& release1, 
                       const std::string& version2, const std::string& release2);

#endif // COMPARE_VERSIONS_H
