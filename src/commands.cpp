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
    nlohmann::json j = nlohmann::json::parse(json);
    string path = j["path"];
    string actualPath = resolvePath(STORAGE_LOCATION + path);
    string params = j["params"];

    string result;

    // make sure, that directories exist.
    if (!fs::exists(actualPath)) {
        cout << "creating " << actualPath << endl;
        fs::create_directories(actualPath);
    }

    FILE* pipe = popen(string("ls " + params + " " + actualPath).c_str(), "r");
    if (!pipe) { return "{\"path\" : \"" + path + "\", \"error\" : \"Could not open Pipe\"}"; }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
        // cout << buffer;
    }
    pclose(pipe);
    cout << result << endl;
    return "{\"path\" : \"" + path + "\", \"answer\" : \"" + result + "\"}";
};
};  // namespace commands