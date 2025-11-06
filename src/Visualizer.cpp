#include "Visualizer.h"
#include <cmath>
#include <iostream>
#include <random>

namespace {
    float globalTime = 0.0f;
    float pulse = 1.0f;
    std::vector<sf::CircleShape> particles; // 金色粒子
    std::mt19937 rng{std::random_device{}()};
}

void Visualizer::setPulse(float p) {
    pulse = p;
}

// 初始化粒子
void initParticles(const sf::RenderWindow& window, int count = 80) {
    particles.clear();
    std::uniform_real_distribution<float> xDist(0, window.getSize().x);
    std::uniform_real_distribution<float> yDist(0, window.getSize().y);
    std::uniform_real_distribution<float> rDist(1.5f, 3.5f);

    for (int i = 0; i < count; ++i) {
        sf::CircleShape p(rDist(rng));
        p.setPosition(xDist(rng), yDist(rng));
        p.setFillColor(sf::Color(255, 230, 150, 90));
        particles.push_back(p);
    }
}

void Visualizer::drawArray(sf::RenderWindow& window, const std::vector<int>& arr, const std::string& algoName) {
    if (particles.empty()) initParticles(window);
    globalTime += 0.016f;

    float width = static_cast<float>(window.getSize().x) / arr.size();
    float baseY = window.getSize().y;

    // 🌈 背景呼吸渐变：深紫 → 金色
    sf::RectangleShape bg(sf::Vector2f(window.getSize().x, window.getSize().y));
    float t = (std::sin(globalTime * 0.4f) + 1.0f) / 2.0f;
    sf::Color c1(25, 5, 45);
    sf::Color c2(240, 190, 100);
    bg.setFillColor(sf::Color(
        c1.r * (1 - t) + c2.r * t,
        c1.g * (1 - t) + c2.g * t,
        c1.b * (1 - t) + c2.b * t
    ));
    window.draw(bg);

    // 🌟 粒子漂浮
    for (auto& p : particles) {
        auto pos = p.getPosition();
        pos.y -= 0.5f + std::sin(globalTime * 0.5f) * 0.3f;
        if (pos.y < -5) pos.y = window.getSize().y + 5;
        p.setPosition(pos);
        float flicker = 0.5f + 0.5f * std::sin(globalTime * 3 + pos.x * 0.1f);
        sf::Color c(255, 230, 150, static_cast<sf::Uint8>(100 * pulse * flicker));
        p.setFillColor(c);
        window.draw(p);
    }

    // 📊 渲染条形柱
    for (size_t i = 0; i < arr.size(); ++i) {
        float height = static_cast<float>(arr[i]);
        float x = i * width;
        float offset = std::sin(i * 0.25f + globalTime * 2.0f) * 12.0f;

        sf::RectangleShape bar(sf::Vector2f(width - 1, -height));
        bar.setPosition(x, baseY - offset);

        float hueShift = std::sin(globalTime + i * 0.2f) * 0.5f + 0.5f;
        sf::Color color(
            180 + 70 * hueShift * pulse,
            140 + 80 * (1 - hueShift) * pulse,
            255 - 120 * hueShift,
            230
        );
        bar.setFillColor(color);
        window.draw(bar);

        // ✨ 光晕点
        sf::CircleShape aura(3 + pulse * 3);
        aura.setPosition(x + width / 2, baseY - height - offset);
        aura.setFillColor(sf::Color(255, 255, 210, 80 * pulse));
        window.draw(aura);
    }

    // ⚡ 顶部能量波线
    sf::VertexArray wave(sf::LinesStrip, window.getSize().x / 4);
    for (size_t i = 0; i < wave.getVertexCount(); ++i) {
        float x = i * 4.0f;
        float y = 40 + std::sin(globalTime * 2.0f + i * 0.2f) * 8.0f * pulse;
        wave[i].position = sf::Vector2f(x, y);
        wave[i].color = sf::Color(255, 220, 150, 180);
    }
    window.draw(wave);

    // --- 🎴 算法名呼吸闪烁 ---
    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded) {
        if (font.loadFromFile("../assets/SourceHanSerifSC-Regular.otf") ||
            font.loadFromFile("assets/SourceHanSerifSC-Regular.otf")) {
            fontLoaded = true;
        } else {
            std::cerr << "⚠️ 字体文件未找到！" << std::endl;
        }
    }

    sf::Text label;
    label.setFont(font);
    label.setString(algoName);
    label.setCharacterSize(30);
    float flicker = 0.5f + 0.5f * std::sin(globalTime * 2.2f);
    label.setFillColor(sf::Color(
        200 + 55 * flicker * pulse,
        220,
        255,
        240
    ));
    label.setPosition(20, 20);
    window.draw(label);

    // 💫 下方能量标题
    sf::Text subtitle;
    subtitle.setFont(font);
    subtitle.setString("Algorithm as Ritual");
    subtitle.setCharacterSize(20);
    subtitle.setFillColor(sf::Color(255, 220, 200, 180));
    subtitle.setPosition(20, window.getSize().y - 40 + std::sin(globalTime * 1.5f) * 3);
    window.draw(subtitle);
}