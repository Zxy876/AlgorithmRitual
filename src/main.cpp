#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <filesystem>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <system_error>
#include <mach-o/dyld.h> // ✅ macOS 特有，用于获取当前可执行路径

#include "SortAlgorithm.h"
#include "Visualizer.h"

using namespace std::chrono_literals;

// ✅ 检测文件是否被修改
bool checkFileModified(const std::filesystem::path& path, std::filesystem::file_time_type& lastWriteTime) {
    std::error_code ec;
    if (!std::filesystem::exists(path, ec) || ec) return false;

    auto current = std::filesystem::last_write_time(path, ec);
    if (ec) return false;

    if (current != lastWriteTime) {
        lastWriteTime = current;
        return true;
    }
    return false;
}

// ✅ 随机生成数组
std::vector<int> generateRandomArray(size_t size) {
    std::vector<int> arr(size);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (auto& v : arr) v = std::rand() % 550 + 20;
    return arr;
}

// ✅ 启动排序线程
void restartSortThread(std::unique_ptr<std::thread>& sortThread,
                       SortAlgorithm& sorter,
                       std::vector<int>& arr,
                       std::atomic<bool>& running,
                       int currentAlgorithm) {
    running = true;
    sorter.ready = false;
    sortThread = std::make_unique<std::thread>([&]() {
        switch (currentAlgorithm) {
            case 0: sorter.bubbleSort(arr, running); break;
            case 1: sorter.quickSort(arr, running); break;
            case 2: sorter.mergeSort(arr, running); break;
            case 3: sorter.heapSort(arr, running); break;
            default: sorter.bubbleSort(arr, running); break;
        }
    });
}

// ✅ 获取正确的资源路径（兼容 .app / 开发目录）
std::filesystem::path getAppResourcePath() {
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        std::filesystem::path exePath(path);
        auto appAssets = exePath.parent_path() / "assets";
        if (std::filesystem::exists(appAssets))
            return appAssets;
    }
    return std::filesystem::path("assets");
}

int main() {
    try {
        // ✅ 修复 Finder 启动路径（让 .app 能正常加载资源）
        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0) {
            std::filesystem::path exePath(path);
            auto macPath = exePath.parent_path();
            std::filesystem::current_path(macPath);
            std::cout << "📁 已切换工作目录到: " << macPath << std::endl;
        }

        // 🎬 窗口初始化
        sf::RenderWindow window(sf::VideoMode(800, 600), "Algorithm as Ritual");
        window.setFramerateLimit(60);

        std::cout << "🕹️ 控制说明:\n"
                  << "   Q - 切换算法\n"
                  << "   R - 重新编译并重启排序\n"
                  << "   ESC / 关闭窗口 - 退出程序\n\n";

        // 🎵 加载音乐
        sf::Music music;
        std::filesystem::path basePath = getAppResourcePath();
        std::string musicPath;

        std::vector<std::filesystem::path> possiblePaths = {
            basePath / "chaoduwo_climax.ogg",
            std::filesystem::path("assets/chaoduwo_climax.ogg"),
            std::filesystem::path("../assets/chaoduwo_climax.ogg")
        };

        for (const auto& p : possiblePaths) {
            if (std::filesystem::exists(p)) {
                musicPath = p.string();
                break;
            }
        }

        if (musicPath.empty()) {
            std::cerr << "⚠️ 未找到音乐文件 chaoduwo_climax.ogg\n";
        } else if (music.openFromFile(musicPath)) {
            music.setLoop(true);
            music.setVolume(75);
            music.play();
            std::cout << "🎧 成功播放《超度我》: " << musicPath << "\n";
        } else {
            std::cerr << "⚠️ 无法打开音乐文件: " << musicPath << "\n";
        }

        // 🧮 初始化数据
        std::vector<int> arr = generateRandomArray(80);
        std::atomic<bool> running(true);
        SortAlgorithm sorter(30);
        bool recompileRequested = false;
        int currentAlgorithm = 0;

        // 🔍 检测 SortAlgorithm.cpp 是否被修改
        std::filesystem::path filePath = "src/SortAlgorithm.cpp";
        if (!std::filesystem::exists(filePath))
            filePath = "../src/SortAlgorithm.cpp";

        std::error_code ec;
        std::filesystem::file_time_type lastWriteTime =
            std::filesystem::exists(filePath, ec)
                ? std::filesystem::last_write_time(filePath, ec)
                : std::filesystem::file_time_type::min();

        // 🧵 启动排序线程
        std::unique_ptr<std::thread> sortThread;
        restartSortThread(sortThread, sorter, arr, running, currentAlgorithm);

        auto lastFileCheck = std::chrono::steady_clock::now();

        // 🎮 主循环
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    running = false;
                    sorter.cv.notify_all();
                    if (sortThread && sortThread->joinable()) sortThread->join();
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R)
                        recompileRequested = true;

                    // 🔁 切换算法（带防卡冷却）
                    if (event.key.code == sf::Keyboard::Q) {
                        static auto lastSwitch = std::chrono::steady_clock::now();
                        auto now = std::chrono::steady_clock::now();
                        if (now - lastSwitch > 800ms) {
                            currentAlgorithm = (currentAlgorithm + 1) % 4;
                            recompileRequested = true;
                            lastSwitch = now;
                            std::cout << "🔁 切换到算法 #" << currentAlgorithm << std::endl;
                        }
                    }
                }
            }

            // 🔄 检测改动或手动触发重新编译
            auto now = std::chrono::steady_clock::now();
            if (now - lastFileCheck > 500ms) {
                lastFileCheck = now;
                if ((std::filesystem::exists(filePath, ec) &&
                     checkFileModified(filePath, lastWriteTime)) ||
                    recompileRequested) {
                    std::cout << "\n🔧 检测到改动或按下 R/Q 键，重新编译中...\n";
                    recompileRequested = false;

                    running = false;
                    sorter.cv.notify_all();
                    if (sortThread && sortThread->joinable()) sortThread->join();

                    int code = 0;
                    if (std::filesystem::exists("/Users/zxydediannao/AlgorithmRitual/build")) {
                        code = system("cd /Users/zxydediannao/AlgorithmRitual/build && make -j4");
                    } else {
                        std::cerr << "⚠️ build 目录不存在，跳过重新编译。\n";
                    }

                    if (code == 0) {
                        std::cout << "✅ 编译成功，重启排序...\n";
                        for (float v = 75; v >= 45; v -= 5) {
                            music.setVolume(v);
                            std::this_thread::sleep_for(10ms);
                        }
                        for (float v = 45; v <= 75; v += 5) {
                            music.setVolume(v);
                            std::this_thread::sleep_for(10ms);
                        }
                    } else {
                        std::cerr << "❌ 编译失败，请检查语法。\n";
                    }

                    arr = generateRandomArray(80);
                    restartSortThread(sortThread, sorter, arr, running, currentAlgorithm);
                }
            }

            // 🎶 音乐脉冲同步
            float pulse = 1.0f;
            if (music.getStatus() == sf::Music::Playing) {
                float t = music.getPlayingOffset().asSeconds();
                pulse = 0.5f + 0.5f * std::sin(t * 2.4f);
            }
            Visualizer::setPulse(pulse);

            // 🧭 算法标题
            static const char* algoNames[4] = {"Bubble Sort", "Quick Sort", "Merge Sort", "Heap Sort"};
            std::string algoName =
                (currentAlgorithm >= 0 && currentAlgorithm < 4)
                    ? algoNames[currentAlgorithm]
                    : "Unknown Algorithm";

            std::string titleText = "Algorithm as Ritual — " + algoName + " [R=Recompile, Q=Switch]";
            sf::String safeTitle = sf::String::fromUtf8(titleText.begin(), titleText.end());
            try {
                window.setTitle(safeTitle);
            } catch (...) {
                std::cerr << "⚠️ Title update failed (invalid UTF-8). Skipping...\n";
            }

            // 🕰️ 等待排序线程交出一帧
            {
                std::unique_lock<std::mutex> lock(sorter.mtx);
                sorter.cv.wait_for(lock, 50ms, [&] { return sorter.ready || !running.load(); });
                if (sorter.ready) sorter.ready = false;
            }

            // 🎨 绘制界面
            window.clear(sf::Color::Black);
            Visualizer::drawArray(window, arr, algoName);
            window.display();

            sorter.cv.notify_one();
        }

        // ✅ 清理
        running = false;
        sorter.cv.notify_all();
        if (sortThread && sortThread->joinable()) sortThread->join();
        music.stop();

        std::cout << "🕊️ 仪式结束。算法已超度。\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "💥 捕获异常: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "💥 未知异常导致崩溃。\n";
        return 1;
    }
}