#ifndef PACKAGE_FETCHER_H
#define PACKAGE_FETCHER_H

#include <string>
#include <nlohmann/json.hpp>

class PackageFetcher {
public:
    PackageFetcher();
    ~PackageFetcher();

    nlohmann::json fetchBranchData(const std::string& branchName);
    static nlohmann::json comparePackages(const nlohmann::json& branch1, const nlohmann::json& branch2);
    static void saveJsonToFile(const nlohmann::json& jsonObj, const std::string& fileName);

private:
    void* curl; // CURL pointer

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);
};

#endif // PACKAGE_FETCHER_H
