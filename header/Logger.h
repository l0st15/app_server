//
// Created by Даниил on 26.05.2025.
//

#ifndef APP_SERVER_LOGGER_H
#define APP_SERVER_LOGGER_H

#include <json.hpp>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <thread>

class Logger {
public:
    Logger(const std::string& filename) : log_file(filename, std::ios::app) {}

    void log(const std::string& level, const std::string& mes) {

        /*std::string thread_id_str = std::to_string(
                std::hash<std::thread::id>{}(std::this_thread::get_id())
        );*/

        std::string log_entry = "timestamp: " + currentTime() + " level: " + level + " message: " + mes;

        log_file << log_entry << "\n";
    }

    ~Logger() {
        log_file.close();
    }
private:
    std::string static currentTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    }
    std::ofstream log_file;
};


#endif //APP_SERVER_LOGGER_H
