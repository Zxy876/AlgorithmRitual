# AlgorithmRitual

[![Zread](https://img.shields.io/badge/Zread-Explore-blue?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCIgZmlsbD0ibm9uZSIgc3Ryb2tlPSJjdXJyZW50Q29sb3IiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLWxpbmVjYXA9InJvdW5kIiBzdHJva2UtbGluZWpvaW49InJvdW5kIj48cGF0aCBkPSJNMTIgMlMzIDcgMyAxMnM5IDUgOSA1IDktNSA5LTEyUzEyIDIgMTIgMnoiLz48cGF0aCBkPSJNMTIgMTZ2NCIvPz48cGF0aCBkPSJNMTIgOHY0Ii8+PC9zdmc+)](https://zread.ai/Zxy876/AlgorithmRitual)

> *"我的执念，万千千千。放不下地，放不下天。"*

![Algorithm as Ritual](assets/icon.png)

AlgorithmRitual 是一个交互式、多线程的排序算法可视化应用程序，它将算法执行转化为一种美学的视听体验。该应用基于 C++17 和 SFML 构建，通过同步的图形、声音和粒子效果实时演示算法行为，并具有热重载开发系统，支持立即进行代码迭代。

## 观看视频

[![我的执念，万千千千。放不下地，放不下天](https://img.shields.io/badge/Bilibili-观看视频-FF69B4?logo=bilibili)](https://b23.tv/0p08Riu)

**[我的执念，万千千千。放不下地，放不下天](https://b23.tv/0p08Riu)**

> 在这件作品中，我将代码的执行过程转化为一种视觉与听觉的仪式。每一次排序的启动、切换与重编译，都是"秩序"与"混乱"之间的反复召唤。算法不再只是理性的工具，而是一种节奏，一种冥想，一种现代的祈祷。

## 核心概念

该应用程序将算法可视化视为一种"仪式"——一种沉浸式的体验，其中代码执行成为数据移动、视觉变换和音乐伴奏的同步表演。每次比较、交换和分区操作都会实时渲染，使开发者和学习者能够通过分析和艺术的角度观察算法模式。

我以不同的排序方式（冒泡、快速、归并、堆）作为仪式的四种"咒语"。它们在屏幕上构建出跳动的几何与光脉，与音乐同步呼吸。音乐《超度我》的旋律成为算法的心跳，代码的循环则化为节奏的脉冲。

## 主要特性

| 特性 | 描述 |
|------|------|
| 多算法支持 | 冒泡排序、快速排序、归并排序、堆排序 |
| 线程安全执行 | 使用互斥锁/条件变量同步的工作线程，算法执行期间保持流畅的 60 FPS 渲染 |
| 热重载系统 | 代码修改时自动重新编译，无需重启应用程序即可快速进行开发迭代 |
| 视听同步 | 背景音乐与亮度脉冲同步，沉浸式体验，算法节奏的可视化 |
| 粒子系统 | 动态背景效果，增强视觉深度和吸引力 |
| macOS App Bundle | 原生 .app 打包，集成图标，专业的部署体验 |

## 技术栈

| 组件 | 技术 |
|------|------|
| 语言 | C++17 |
| 图形 | SFML 2.5 |
| 构建系统 | CMake |
| 线程 | C++ STL (`std::thread`, `std::mutex`, `std::condition_variable`) |
| 音频格式 | OGG Vorbis |
| 字体 | Source Han Serif SC |

## 项目结构

```
AlgorithmRitual/
├── CMakeLists.txt          # 跨平台构建配置
├── start_ritual.sh         # 自动化构建和启动脚本
├── assets/                 # 应用程序资源
│   ├── chaoduwo_climax.ogg           # 背景音乐
│   ├── SourceHanSerifSC-Regular.otf  # 字体文件
│   ├── icon.png                      # 应用程序图标
│   └── icon.iconset/                 # 多分辨率图标
├── include/                # 公共头文件
│   ├── SortAlgorithm.h     # 排序算法接口
│   └── Visualizer.h        # 可视化接口
└── src/                    # 实现文件
    ├── main.cpp           # 应用程序入口和编排
    ├── SortAlgorithm.cpp  # 算法实现
    └── Visualizer.cpp     # 渲染和粒子系统
```

## 快速开始

### 环境要求

- macOS 10.15+
- CMake 3.20+
- C++17 编译器
- SFML 2.5

### 安装依赖

```bash
brew install sfml@2 cmake
```

### 构建与运行

```bash
# 克隆仓库
git clone https://github.com/Zxy876/AlgorithmRitual.git
cd AlgorithmRitual

# 运行启动脚本（自动编译、打包、启动）
./start_ritual.sh
```

## 键盘控制

| 按键 | 功能 |
|------|------|
| `Q` | 切换排序算法（冒泡 → 快速 → 归并 → 堆） |
| `R` | 手动重新编译并重启排序 |
| `ESC` / 关闭窗口 | 退出程序 |

## 算法能力

| 算法 | 时间复杂度 | 视觉特征 |
|------|-----------|---------|
| 冒泡排序 | O(n²) | 缓慢、有条理的相邻比较 |
| 快速排序 | O(n log n) 平均 | 戏剧性的分区操作，递归模式 |
| 归并排序 | O(n log n) | 可预测的分治合并 |
| 堆排序 | O(n log n) | 结构化的堆化操作，逐步有序化 |

## 深入探索

- [在 Zread 上探索代码](https://zread.ai/Zxy876/AlgorithmRitual) - AI 驱动的代码阅读体验

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。

## 致谢

- 音乐《超度我》
- SFML 图形库
- 思源宋体字体

---

*"当 Q 键按下，就像切换信仰的姿态，观察理性之舞在屏幕上自我重组。这不是关于效率的计算，而是关于存在的秩序。"*

[![Zread](https://zread.ai/Zxy876/AlgorithmRitual/opengraph-image)](https://zread.ai/Zxy876/AlgorithmRitual)
