#include <iostream>
#include <crow.h>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "filehandler.cpp"

using namespace std;
using namespace crow;
using namespace filehandler;

/**
 * @brief Reads a file and returns its content
 *
 * @param fileName the name of the file
 * @return string the content of the file
 */
string readStaticFile(const string &fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        throw runtime_error("Unable to open file: " + fileName);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Starts a REST server, that servers the application
 */
void startServer()
{
    SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
     {
        try {
            string htmlContent = readStaticFile("web/index.html");
            return response(200, htmlContent);
        } catch (const exception& e) {
            return response(404, "HTML file not found.");
        } });

    CROW_ROUTE(app, "/script.js")
    ([]()
     {
        try {
            std::string jsContent = readStaticFile("web/script.js");
            return response(200, jsContent);
        } catch (const std::exception& e) {
            return response(404, "JS file not found.");
        } });

    CROW_ROUTE(app, "/style.css")
    ([]()
     {
        try {
            std::string cssContent = readStaticFile("web/style.css");
            return response(200, cssContent);
        } catch (const std::exception& e) {
            return response(404, "CSS file not found.");
        } });

    CROW_ROUTE(app, "/favicon/favicon-32x32.png")
    ([]()
     {
        try {
            string faviconContent = readStaticFile("web/favicon/favicon-16x16.png");
            return response(200, faviconContent);
        } catch (const std::exception& e) {
            return response(404, "favicon not found.");
        } });

    CROW_ROUTE(app, "/command/<str>")
    ([](string command)
     {
        if (command == "ls") {
            return response(200, "List of files:\n\tfile1.txt\n\tfile2.png\n\tfile3.cpp");
        } else {
            return response(200, "command not found");
        } });

    app.port(50505).multithreaded().run();
}

int main()
{
    filehandler::File f("filename.txt", "test/", "hello world");
    cout << f.serialize() << endl;
    return 0;
}