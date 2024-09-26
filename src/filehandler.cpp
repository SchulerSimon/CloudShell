#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using namespace std;
namespace fs = std::filesystem;

const string STORAGE_LOCATION = "./data/";

static string getCurrentTime() {
    time_t now = time(nullptr);
    tm *localTime = localtime(&now);
    ostringstream oss;
    oss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

namespace filehandler {
class File {
   private:
    bool trashed = false;
    string fileName;
    string filePath;
    string fileType;
    size_t fileSize;
    string creationDate;
    string fileContent;
    string completeFilePath;

    void save() {
        // create directories
        fs::create_directories(STORAGE_LOCATION + filePath);

        // create filestream
        ofstream contentFile(this->completeFilePath);
        if (contentFile.is_open()) {
            contentFile << this->fileContent;
            contentFile.close();
        } else {
            throw runtime_error("Unable to save fileContent: " + completeFilePath);
        }

        ofstream metaFile(this->completeFilePath + ".meta.json");
        if (metaFile.is_open()) {
            metaFile << this->serialize();
            metaFile.close();
        } else {
            throw runtime_error("Unable to save metaData: " + completeFilePath + ".meta.json");
        }
    }

    // Private default constructor for deserialization
    File() {}

   public:
    File(const string &fileName, const string &filePath, const string &fileContent)
        : fileName(fileName),
          filePath(filePath),
          fileContent(fileContent),
          completeFilePath(STORAGE_LOCATION + filePath + fileName),
          creationDate(getCurrentTime()) {
        // set filesize automatically
        this->fileSize = fileContent.size();

        // set fileType automatically
        size_t dotPos = fileName.find_last_of('.');
        if (dotPos != string::npos) {
            this->fileType = fileName.substr(dotPos + 1);
        } else {
            this->fileType = "";
        }

        // save this file after creating it
        this->save();
    }
    /**
     * @brief serializes this file object
     *
     * @return string json string of this object
     */
    string serialize() const {
        nlohmann::json j;
        j["trashed"] = trashed;
        j["fileName"] = fileName;
        j["filePath"] = filePath;
        j["completeFilePath"] = completeFilePath;
        j["fileType"] = fileType;
        j["fileSize"] = fileSize;
        j["creationDate"] = creationDate;
        return j.dump();
    }

    /**
     * @brief deserializes the given json string into an object
     *
     * @param jsonString the json string
     * @return File the object
     */
    static File deserialize(const string &jsonString) {
        nlohmann::json j = nlohmann::json::parse(jsonString);
        File file;
        file.trashed = j["trashed"];
        file.fileName = j["fileName"];
        file.filePath = j["filePath"];
        file.completeFilePath = j["completeFilePath"];
        file.fileType = j["fileType"];
        file.fileSize = j["fileSize"];
        file.creationDate = j["creationDate"];
        return file;
    }
};
};  // namespace filehandler