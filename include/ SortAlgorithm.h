#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

class SortAlgorithm {
  public:
  static void bubbleSort(std::vector<int>& arr,std::mutex& mtx,bool& running);
};