#include <iostream>
#include <crow.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

using namespace std;
using namespace crow;

string read_file(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw runtime_error("Unable to open file: " + filename);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void create_file(const string &filename, const string &content)
{
    ofstream file(filename);
    if (file.is_open())
    {
        file << content;
    }
    else
    {
        throw runtime_error("Unable to open file");
    }
    file.close();
}

int main()
{
    SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
     {
        try {
            string htmlContent = read_file("web/index.html");
            return response(200, htmlContent);
        } catch (const exception& e) {
            return response(404, "HTML file not found.");
        } });

    CROW_ROUTE(app, "/script.js")
    ([]()
     {
        try {
            std::string jsContent = read_file("web/script.js");
            return response(200, jsContent);
        } catch (const std::exception& e) {
            return response(404, "JS file not found.");
        } });

    CROW_ROUTE(app, "/style.css")
    ([]()
     {
        try {
            std::string cssContent = read_file("web/style.css");
            return response(200, cssContent);
        } catch (const std::exception& e) {
            return response(404, "CSS file not found.");
        } });

    CROW_ROUTE(app, "/favicons/favicon-32x32.png")
    ([]()
     {
        try {
            string faviconContent = read_file("web/favicon/favicon-16x16.png");
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

    app.port(5000).multithreaded().run();

    return 0;
}