#!/bin/bash
set -e

echo "🚀 开始发布 AlgorithmRitual 到 GitHub..."

# 检查 gh 是否安装
if ! command -v gh &> /dev/null; then
  echo "❌ GitHub CLI (gh) 未安装，请先运行：brew install gh"
  exit 1
fi

# 检查登录状态
if ! gh auth status &>/dev/null; then
  echo "🔐 需要登录 GitHub..."
  gh auth login
fi

# 检查压缩包是否存在
ZIP_FILE="build/AlgorithmRitual_macOS.zip"
if [ ! -f "$ZIP_FILE" ]; then
  echo "⚠️ 未找到 $ZIP_FILE，正在尝试重新打包..."
  mkdir -p AlgorithmRitual.app/Contents/MacOS
  cp build/AlgorithmRitual AlgorithmRitual.app/Contents/MacOS/ 2>/dev/null || true
  cp -r assets AlgorithmRitual.app/Contents/MacOS/ 2>/dev/null || true

  mkdir -p AlgorithmRitual.app/Contents
  cat > AlgorithmRitual.app/Contents/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleExecutable</key>
  <string>AlgorithmRitual</string>
  <key>CFBundleIdentifier</key>
  <string>com.zxy.algorithmritual</string>
  <key>CFBundleName</key>
  <string>AlgorithmRitual</string>
  <key>CFBundleVersion</key>
  <string>1.0</string>
</dict>
</plist>
EOF

  zip -r AlgorithmRitual_macOS.zip AlgorithmRitual.app > /dev/null
  mv AlgorithmRitual_macOS.zip build/
  echo "✅ 已生成 $ZIP_FILE"
fi

# 检查是否已存在仓库
if [ ! -d ".git" ]; then
  echo "📦 初始化 Git 仓库..."
  git init
  git add .
  git commit -m "Initial commit"
  gh repo create Zxy876/AlgorithmRitual --public --source=. --remote=origin --push
else
  echo "🔄 推送最新更改到 GitHub..."
  git add .
  git commit -m "Update build for release"
  git push origin main || git push -u origin main
fi

# 创建发布
echo "🪄 创建 GitHub Release..."
gh release create v1.0 "$ZIP_FILE" \
  --title "Algorithm as Ritual — Sorting Visualization" \
  --notes "🎮 可视化排序仪式体验版\n包含冒泡、快速、归并、堆排序算法与音乐脉冲同步效果。\n\n🕹 操作:\nQ 切换算法\nR 重新编译\nESC 退出程序"

echo "✅ 发布完成！下载链接："
gh release view v1.0 --json url -q ".url"
