#include "SortAlgorithm.h"
#include <algorithm>
#include <iostream>
#include <thread>

SortAlgorithm::SortAlgorithm(int delay)
    : sortDelay(delay) {}

// 通知主线程绘制一帧
void SortAlgorithm::updateAndWait(std::vector<int>& arr, std::atomic<bool>& running) {
    {
        std::unique_lock<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this, &running] { return !ready || !running.load(); });
}

// ================= 冒泡排序 =================
void SortAlgorithm::bubbleSort(std::vector<int>& arr, std::atomic<bool>& running) {
    for (size_t i = 0; i < arr.size() && running.load(); ++i) {
        for (size_t j = 0; j < arr.size() - i - 1 && running.load(); ++j) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (arr[j] > arr[j + 1])
                    std::swap(arr[j], arr[j + 1]);
            }
            updateAndWait(arr, running);
            std::this_thread::sleep_for(std::chrono::milliseconds(sortDelay));
        }
    }
    running.store(false);
}

// ================= 快速排序 =================
int SortAlgorithm::partition(std::vector<int>& arr, std::atomic<bool>& running, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high && running.load(); ++j) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (arr[j] < pivot)
                std::swap(arr[++i], arr[j]);
        }
        updateAndWait(arr, running);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortDelay));
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::swap(arr[i + 1], arr[high]);
    }
    updateAndWait(arr, running);
    return i + 1;
}

void SortAlgorithm::quickSortHelper(std::vector<int>& arr, std::atomic<bool>& running, int low, int high) {
    if (low < high && running.load()) {
        int pi = partition(arr, running, low, high);
        quickSortHelper(arr, running, low, pi - 1);
        quickSortHelper(arr, running, pi + 1, high);
    }
}

void SortAlgorithm::quickSort(std::vector<int>& arr, std::atomic<bool>& running) {
    quickSortHelper(arr, running, 0, arr.size() - 1);
    running.store(false);
}

// ================= 归并排序 =================
void SortAlgorithm::merge(std::vector<int>& arr, int left, int mid, int right, std::atomic<bool>& running) {
    std::vector<int> temp;
    int i = left, j = mid + 1;
    while (i <= mid && j <= right && running.load()) {
        if (arr[i] <= arr[j]) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }
    while (i <= mid && running.load()) temp.push_back(arr[i++]);
    while (j <= right && running.load()) temp.push_back(arr[j++]);

    for (int k = 0; k < temp.size(); ++k) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            arr[left + k] = temp[k];
        }
        updateAndWait(arr, running);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortDelay));
    }
}

void SortAlgorithm::mergeSortHelper(std::vector<int>& arr, int left, int right, std::atomic<bool>& running) {
    if (left < right && running.load()) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, running);
        mergeSortHelper(arr, mid + 1, right, running);
        merge(arr, left, mid, right, running);
    }
}

void SortAlgorithm::mergeSort(std::vector<int>& arr, std::atomic<bool>& running) {
    mergeSortHelper(arr, 0, arr.size() - 1, running);
    running.store(false);
}

// ================= 堆排序 =================
void SortAlgorithm::heapify(std::vector<int>& arr, int n, int i, std::atomic<bool>& running) {
    int largest = i;
    int l = 2 * i + 1, r = 2 * i + 2;
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;
        if (largest != i)
            std::swap(arr[i], arr[largest]);
    }
    updateAndWait(arr, running);
    std::this_thread::sleep_for(std::chrono::milliseconds(sortDelay));
    if (largest != i && running.load())
        heapify(arr, n, largest, running);
}

void SortAlgorithm::heapSort(std::vector<int>& arr, std::atomic<bool>& running) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0 && running.load(); --i)
        heapify(arr, n, i, running);
    for (int i = n - 1; i > 0 && running.load(); --i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::swap(arr[0], arr[i]);
        }
        updateAndWait(arr, running);
        std::this_thread::sleep_for(std::chrono::milliseconds(sortDelay));
        heapify(arr, i, 0, running);
    }
    running.store(false);
}