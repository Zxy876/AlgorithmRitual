#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Visualizer {
public:
    // 绘制排序数组，可显示算法名称
    static void drawArray(sf::RenderWindow& window, const std::vector<int>& arr, const std::string& algoName);

    // 与音乐同步的亮度脉冲
    static void setPulse(float p);
};