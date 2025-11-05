#include " SortAlgorithm.h"
#include <algorithm>

void SortAlgorithm::bubbleSort(std::vector<int>& arr, std::mutex& mtx, bool& running) {
  size_t n = arr.size();
  for(size_t i = 0; i < n - 1 ;++i){
    for(size_t j = 0; j < n-i-1 ;++j){
      {
        std::lock_guard<std::mutex> lock(mtx);
        if(arr[j]>arr[j+1]){
          std::swap(arr[j], arr[j+1]);

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
  }
  running = false;
}