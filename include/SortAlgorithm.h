#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <chrono>

class SortAlgorithm {
public:
    // 构造函数：支持自定义延迟
    explicit SortAlgorithm(int delay = 50);

    // 延迟（控制动画速度）
    int sortDelay;

    // 同步控制变量
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;

    // 公共函数接口
   void updateAndWait(std::vector<int>& arr, std::atomic<bool>& running);
    void bubbleSort(std::vector<int>& arr, std::atomic<bool>& running);
    void quickSort(std::vector<int>& arr, std::atomic<bool>& running);
    void mergeSort(std::vector<int>& arr, std::atomic<bool>& running);
    void heapSort(std::vector<int>& arr, std::atomic<bool>& running);

private:
    // ---------- 快速排序 ----------
    void quickSortHelper(std::vector<int>& arr, std::atomic<bool>& running, int low, int high);
    int partition(std::vector<int>& arr, std::atomic<bool>& running, int low, int high);

    // ---------- 归并排序 ----------
    void merge(std::vector<int>& arr, int left, int mid, int right, std::atomic<bool>& running);
    void mergeSortHelper(std::vector<int>& arr, int left, int right, std::atomic<bool>& running);

    // ---------- 堆排序 ----------
    void heapify(std::vector<int>& arr, int n, int i, std::atomic<bool>& running);
};