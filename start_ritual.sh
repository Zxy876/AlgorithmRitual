#!/bin/bash
set -e

echo "🌙 启动 AlgorithmRitual 仪式中..."
cd ~/AlgorithmRitual

# 如果 build 不存在则新建
if [ ! -d "build" ]; then
    echo "📦 正在创建 build..."
    mkdir build
fi

cd build

# 重新编译（若已存在 cmake 缓存会自动复用）
echo "⚙️ 编译中..."
cmake .. >/dev/null
make -j4

# 构建 .app 目录结构
APP_DIR="AlgorithmRitual_macOS/AlgorithmRitual.app"
MACOS_DIR="$APP_DIR/Contents/MacOS"

mkdir -p "$MACOS_DIR"
cp AlgorithmRitual "$MACOS_DIR/"

# 复制资源（音乐、字体）
echo "🎵 正在复制资源..."
rm -rf "$MACOS_DIR/assets"
cp -r ../assets "$MACOS_DIR/assets"

# 写入 Info.plist（用于 macOS 显示 app 名）
cat > "$APP_DIR/Contents/Info.plist" <<EOL
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleExecutable</key><string>AlgorithmRitual</string>
  <key>CFBundleIdentifier</key><string>com.zxy.algorithmritual</string>
  <key>CFBundleName</key><string>AlgorithmRitual</string>
  <key>CFBundleVersion</key><string>1.0</string>
</dict>
</plist>
EOL

# 启动 app
echo "✨ 构建完成，启动仪式..."
open "$APP_DIR"

echo "🕯️ 仪式已启动完毕！"