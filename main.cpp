#include <iostream>
#include <string>
#include <boost/process.hpp>
#include <chrono>
#include <thread>
#include <csignal>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>

volatile sig_atomic_t processExited = false;

void processExitHandler(int /* signum */) {
    processExited = true;
}

int main() {
    // 设置process脚本退出信号处理函数
    signal(SIGTERM, processExitHandler);

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now_c);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H-%M-%S", &timeinfo);

    // 创建文件并打开以写入内容
    std::ofstream outputFile(time_str + std::string(".txt"));

    // 创建一个tee_device，将输出分别传递到std::cout和文件
    boost::iostreams::tee_device<std::ostream, std::ofstream> tee_out(std::cout, outputFile);
    boost::iostreams::stream<boost::iostreams::tee_device<std::ostream, std::ofstream>> out(tee_out);

    while (true) {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::string cmd = "./test.exe";
             boost::process::child c(cmd, boost::process::std_out > boost::process::null, boost::process::std_err > boost::process::null);
        // 打印启动时间、进程ID和PID
        out << "Started process with PID: " << c.id() << " at " << time_str << std::endl;

        // 等待直到process脚本退出
        while (true) {
            DWORD exitCode;
            if (GetExitCodeProcess(c.native_handle(), &exitCode)) {
                if (exitCode != STILL_ACTIVE) {
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // process脚本退出后的处理
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        out << "process ran for " << duration.count() << " seconds." << std::endl;

        // 检查process脚本是否已退出
        if (processExited) {
            out << "process has exited." << std::endl;
        }

        // 暂停一段时间，然后继续下一轮循环
        std::this_thread::sleep_for(std::chrono::seconds(5)); // 为了演示目的，这里暂停5秒
    }

    // 关闭文件
    outputFile.close();
    return 0;
}
