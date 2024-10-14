#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;
namespace fs = filesystem;

namespace commands {

inline fs::path resolvePath(const fs::path& path) {
    fs::path p;
    // check for ~
    if (path.string().front() == '~') {
        const char* home = getenv("HOME");
        if (home) {
            p = fs::path(home) / path.relative_path().string().substr(1);
        } else {
            p = path;
        }
    } else {
        p = path;
    }
    // return absolute and canonical path
    p = fs::absolute(fs::canonical(p));
    cout << p.string() << endl;
    return p;
}

/**
 * @brief the location for all the files to be stored
 *
 */
const fs::path STORAGE_LOCATION = resolvePath(fs::path(getenv("HOME")) / "CloudShell");

inline void ensurePath(const fs::path& path) {
    // make sure, that directories exist.
    if (!fs::exists(path)) { fs::create_directories(path); }
}

inline bool isSubpath(const fs::path& parent, const fs::path& child) {
    // Compute the relative path from the child to the parent
    fs::path relativePath = fs::relative(child, parent);

    // If the relative path does not start with "..", then child is within the parent directory
    return !relativePath.empty() && relativePath.string().find("..") == std::string::npos;
}

static string notFound(const string& json) {
    // parse the json, resolve path, prepare result
    nlohmann::json j = nlohmann::json::parse(json);
    fs::path path = fs::path(j["path"]);
    nlohmann::json result;
    result["answer"] = "Command not found";
    result["path"] = path.string();
    return result.dump();
}

static string ls(const string& json) {
    // parse the json, resolve path, prepare result
    nlohmann::json j = nlohmann::json::parse(json);
    fs::path path = fs::path(j["path"]);
    fs::path actualPath = resolvePath(STORAGE_LOCATION / path);
    ensurePath(actualPath);
    string params = j["params"];
    nlohmann::json result;
    result["path"] = path.string();

    FILE* pipe = popen(string("ls " + params + " " + actualPath.string()).c_str(), "r");
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
    fs::path path = fs::path(j["path"]);
    fs::path actualPath = resolvePath(STORAGE_LOCATION / path);
    ensurePath(actualPath);
    string name = j["params"];
    nlohmann::json result;

    if (!fs::exists(resolvePath(actualPath / name))) {
        result["path"] = path;
        result["answer"] = "could not find path";
        return result.dump();
    }

    // // make sure, that .. cant go beyond the original location
    // if (name == "..") {
    //     if (isSubpath(STORAGE_LOCATION, resolvePath(actualPath / name))) { path = "."; }
    // } else {
    //     path = resolvePath(actualPath / name);
    // }

    path = fs::relative(path, STORAGE_LOCATION);

    result["path"] = path;
    result["answer"] = "";
    return result.dump();
}

static string mkdir(const string& json) {
    // parse the json, resolve path, prepare result
    nlohmann::json j = nlohmann::json::parse(json);
    fs::path path = fs::path(j["path"]);
    fs::path actualPath = resolvePath(STORAGE_LOCATION / path);
    ensurePath(actualPath);
    string name = j["params"];
    nlohmann::json result;
    result["path"] = path;

    FILE* pipe = popen(string("mkdir " + (actualPath / name).string()).c_str(), "r");
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