#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
namespace fs = filesystem;

namespace commands {

/**
 * @brief the location for all the files to be stored
 *
 */
const string STORAGE_LOCATION = "~/CloudShell";

inline string resolvePath(const string& path) {
    if (path[0] == '~') {
        const char* home = getenv("HOME");
        return string(home) + path.substr(1);  // Remove the '~' and append the rest
    }
    return path;  // Return the original path if it doesn't start with '~'
}

static string ls(const string& json) {
    // parse the json, resolve path, prepare result
    nlohmann::json j = nlohmann::json::parse(json);
    string path = j["path"];
    string actualPath = resolvePath(STORAGE_LOCATION + path);
    string params = j["params"];
    nlohmann::json result;
    result["path"] = path;

    // make sure, that directories exist.
    if (!fs::exists(actualPath)) {
        cout << "creating " << actualPath << endl;
        fs::create_directories(actualPath);
    }

    FILE* pipe = popen(string("ls " + params + " " + actualPath).c_str(), "r");
    if (!pipe) {
        result["answer"] = "could not open PIPE";
        return result.dump();
    }
    string answer;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) { answer += buffer; }
    pclose(pipe);
    result["answer"] = answer;
    return result.dump();
};

static string cd(const string& json) {
    // parse the json, resolve path, prepare result
    nlohmann::json j = nlohmann::json::parse(json);
    string path = j["path"];
    string actualPath = resolvePath(STORAGE_LOCATION + path);
    string name = j["params"];
    nlohmann::json result;

    // make sure, that directories exist.
    if (!fs::exists(actualPath)) {
        cout << "creating " << actualPath << endl;
        fs::create_directories(actualPath);
    }

    if (!fs::exists(actualPath + "/" + name)) {
        result["path"] = path;
        result["answer"] = "could not find " + name;
        return result.dump();
    }

    path += "/" + name;
    result["path"] = path;
    result["answer"] = "";
    return result.dump();
}

static string mkdir(const string& json) {
    // parse the json, resolve path, prepare result
    nlohmann::json j = nlohmann::json::parse(json);
    string path = j["path"];
    string actualPath = resolvePath(STORAGE_LOCATION + path);
    string name = j["params"];
    nlohmann::json result;
    result["path"] = path;

    // make sure, that directories exist.
    if (!fs::exists(actualPath)) {
        cout << "creating " << actualPath << endl;
        fs::create_directories(actualPath);
    }

    FILE* pipe = popen(string("mkdir " + actualPath + "/" + name).c_str(), "r");
    if (!pipe) {
        result["answer"] = "could not open PIPE";
        return result.dump();
    }
    string answer;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) { answer += buffer; }
    pclose(pipe);
    result["answer"] = answer;
    return result.dump();
}

};  // namespace commands