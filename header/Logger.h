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
    Logger() {}

    void log(const std::string& level, const std::string& process, const std::string& mes) {

        /*std::string thread_id_str = std::to_string(
                std::hash<std::thread::id>{}(std::this_thread::get_id())
        );*/

        log_file << "timestamp: " << currentTime();
        log_file << " level: " << level;
        log_file << " process: " << process;
        log_file << " message: " << mes << "\n";
    }

    ~Logger() {
        log_file.close();
    }
    void open(const std::string& filename) {
        log_file.open(filename, std::ios::app);
    }
    void close() {
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
