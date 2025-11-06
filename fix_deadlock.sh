#!/bin/bash
echo "🔧 修复 updateAndWait 声明与定义..."
sed -i '' 's|updateAndWait(std::vector<int>& arr);|updateAndWait(std::vector<int>& arr, std::atomic<bool>& running);|' src/SortAlgorithm.h
if ! grep -q "std::atomic" src/SortAlgorithm.h; then
  sed -i '' '1i\
#include <atomic>
' src/SortAlgorithm.h
fi

echo "🔍 替换所有调用..."
grep -rl "updateAndWait(arr)" src | while read -r f; do
  sed -i '' 's|updateAndWait(arr)|updateAndWait(arr, running)|g' "$f"
done

echo "🧱 重新编译..."
cd build && cmake .. > /dev/null && make -j4
