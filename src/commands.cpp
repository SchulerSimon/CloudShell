#include <cstdlib>
#include <nlohmann/json.hpp>
#include <string>

using namespace std;

namespace commands {

/**
 * @brief the location for all the files to be stored
 *
 */
const string STORAGE_LOCATION = "~/CloudShell";

static string ls(const string &json) {
    nlohmann::json j = nlohmann::json::parse(json);
    string path = j["path"];
    string params = j["params"];

    string result;

    FILE *pipe = popen(string("ls " + params + " " + STORAGE_LOCATION + path).c_str(), "r");
    if (!pipe) { return "{\"path\" : " + path + ", \"error\" : \"Could not open Pipe\"}"; }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
        // std::cout << buffer;
    }
    pclose(pipe);

    return "{\"path\" : " + path + ", \"answer\" : \"" + result + "\"}";
};
};  // namespace commands