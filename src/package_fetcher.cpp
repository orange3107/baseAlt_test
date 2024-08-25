#include "package_fetcher.h"
#include <iostream>
#include <curl/curl.h>
#include <map>
#include <fstream>
#include <set>

using json = nlohmann::json;

PackageFetcher::PackageFetcher() {
    curl = curl_easy_init();
}

PackageFetcher::~PackageFetcher() {
    if (curl) {
        curl_easy_cleanup(static_cast<CURL*>(curl));
    }
}

json PackageFetcher::fetchBranchData(const std::string& branchName) {
    if (!curl) {
        std::cerr << "CURL initialization failed!" << std::endl;
        return json();
    }

    std::string readBuffer;
    std::string url = "https://rdb.altlinux.org/api/export/branch_binary_packages/" + branchName;

    curl_easy_setopt(static_cast<CURL*>(curl), CURLOPT_URL, url.c_str());
    curl_easy_setopt(static_cast<CURL*>(curl), CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(static_cast<CURL*>(curl), CURLOPT_WRITEDATA, &readBuffer);

    CURLcode res = curl_easy_perform(static_cast<CURL*>(curl));
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return json();
    }

    try {
        return json::parse(readBuffer);
    } catch (json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        return json();
    }
}

json PackageFetcher::comparePackages(const json& branch1, const json& branch2) {
    json result;
    result["only_in_branch1"] = json::array();
    result["only_in_branch2"] = json::array();
    result["higher_in_branch1"] = json::array();

    std::map<std::string, json> branch1Packages;
    std::map<std::string, json> branch2Packages;
    std::set<std::string> uniqueNamesBranch1;
    std::set<std::string> uniqueNamesBranch2;

        for (const auto& package : branch1["packages"]) {
            std::string key = package["name"].get<std::string>() + ":" + package["arch"].get<std::string>();
            branch1Packages[key] = package;
        }

        for (const auto& package : branch2["packages"]) {
            std::string key = package["name"].get<std::string>() + ":" + package["arch"].get<std::string>();
            branch2Packages[key] = package;
        }

        for (const auto& [key, pkg1] : branch1Packages) {
            if (branch2Packages.find(key) == branch2Packages.end()) {
                result["only_in_branch1"].push_back({{"name", pkg1["name"]}, {"arch", pkg1["arch"]}});
            }
        }

        for (const auto& [key, pkg2] : branch2Packages) {

            if (branch1Packages.find(key) == branch1Packages.end()) {
                result["only_in_branch2"].push_back({{"name", pkg2["name"]}, {"arch", pkg2["arch"]}});
            }
        }

        for (const auto& [key, pkg1] : branch1Packages) {
            if (branch2Packages.find(key) != branch2Packages.end()) {
                const auto& pkg2 = branch2Packages[key];
                if (compareRpmVersions(pkg1["version"], pkg1["release"], pkg2["version"], pkg2["release"])) {
                    result["higher_in_branch1"].push_back({{"name", pkg1["name"]}, {"arch", pkg1["arch"]}});
                }
            }
        }

    return result;
}

void PackageFetcher::saveJsonToFile(const json& jsonObj, const std::string& fileName) {
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << jsonObj.dump(4);
        file.close();
        std::cout << "JSON сохранен в файл: " << fileName << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи!" << std::endl;
    }
}

size_t PackageFetcher::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t totalSize = size * nmemb;
    s->append((char*)contents, totalSize);
    return totalSize;
}

bool PackageFetcher::compareRpmVersions(const std::string& version1, const std::string& release1,
                                        const std::string& version2, const std::string& release2) {
    if (version1 != version2) {
        return version1 > version2;
    }
    return release1 > release2;
}
