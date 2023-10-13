#include <iostream>
#include <string>
#include <boost/process.hpp>
#include <chrono>
#include <thread>
#include <csignal>
#include <windows.h>

volatile sig_atomic_t pythonExited = false;

void pythonExitHandler(int /* signum */) {
    pythonExited = true;
}

int main() {
    // 设置Python脚本退出信号处理函数
    signal(SIGTERM, pythonExitHandler);

    while (true) {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::string cmd = "C://Users//24016//miniconda3//python.exe d://random_run.py";
        boost::process::child c(cmd, boost::process::std_out > boost::process::null, boost::process::std_err > boost::process::null);

        // 打印启动时间和进程ID
        std::cout << "Started Python script with PID: " << c.id() << " at " << start_time.time_since_epoch().count() << std::endl;

        // 等待直到Python脚本退出
        while (true) {
            DWORD exitCode;
            if (GetExitCodeProcess(c.native_handle(), &exitCode)) {
                if (exitCode != STILL_ACTIVE) {
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Python脚本退出后的处理
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        std::cout << "Python script ran for " << duration.count() << " seconds." << std::endl;

        // 检查Python脚本是否已退出
        if (pythonExited) {
            std::cout << "Python script has exited." << std::endl;
        }

        // 暂停一段时间，然后继续下一轮循环
        std::this_thread::sleep_for(std::chrono::seconds(5)); // 为了演示目的，这里暂停5秒
    }

    return 0;
}
