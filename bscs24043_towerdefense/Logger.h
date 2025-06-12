#ifndef LOGGER_H
#define LOGGER_H
#include "myString.h"
#include "Exceptions.h"
#include <fstream>
#include <ctime>


class Logger {
private:
    static Logger* instance;
    ofstream logFile;

    Logger() {
        logFile.open("logs/log.txt", ios::app);
        if (!logFile.is_open()) {
            throw FileNotFound();
        }
        logFile << "\n=== NEW SESSION ===\n";
    }

public:
    static Logger* getInstance() {
        if (!instance) instance = new Logger();
        return instance;
    }

    void log(const String& category, const String& message, const String& type = "INFO") {

        time_t now = time(0);
        char timestamp[20];
        strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

        logFile << "[" << timestamp << "] "
            << category.c_str() << "\t"
            << type.c_str() << ": "
            << message.c_str() << "\n";
        logFile.flush(); 
    }

    ~Logger() {
        logFile.close();
    }
};

Logger* Logger::instance = nullptr;

#endif