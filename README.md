## README

Hiccup, training the C++ dragon.

## Prepare

环境要求：
- MacOS / Linux / Windows + WSL / Windows + MinGW
- CMake，版本不低于 3.10；
- gcc 或 clang 编译器及标准库(支持C++17），工具链安装见下；
- IDE：VSCode(安装 C++ 插件) / CLion / Eclipse-CDT 均可；

### macOS

- 安装 Xcode Command Line Tools，等待安装完成，这将安装 gcc/g++ 编译器和基本开发工具。
```bash
xcode-select --install
```

- 登录 https://cmake.org/download/ 下载 CMake 安装包，手动安装；

- 确认安装成功：
```bash
g++ --version
cmake --version  # 确认版本 ≥ 3.10
```

### Windows + WSL

- 安装 WSL：以管理员身份打开 PowerShell，执行：
```powershell
wsl --install
```

- 重启电脑，在 WSL 终端中执行：
```bash
sudo apt update && sudo apt upgrade
sudo apt install build-essential cmake
```

- 确认安装成功：
```bash
g++ --version
cmake --version  # 确认版本 ≥ 3.10
```

### Windows + MinGW

- 安装 MinGW-w64
    - 下载 MSYS2 安装程序：https://www.msys2.org/
    - 运行安装程序，安装至默认位置（C:\msys64）

- 打开 MSYS2 终端，执行以下命令：
```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make
```

- 配置环境变量： 打开系统环境变量设置，在 Path 变量中添加：
   - C:\msys64\mingw64\bin
   - C:\msys64\usr\bin

- 打开 MSYS2 终端，执行以下命令，确认安装成功：
```bash
g++ --version
cmake --version  # 确认版本 ≥ 3.10
```

## Build & Test

- 从 Shell 进入工程目录，设置 ccup.sh 脚本权限：
```bash
cd hiccup
chmod +x ./ccup.sh
```

- 执行构建和测试：
```bash
./ccup.sh -ubt
```

- 期望结果：构建成功，执行测试结果显示 "SUCCESS: test hiccup!"

## Cli Commands

```sh
# help
./ccup.sh -h

# update depends and execute cmake generating
./ccup.sh -u

# build
./ccup.sh -b

# update & build
./ccup.sh -ub

# run tests
./ccup.sh -t

# build & test
./ccup.sh -bt

# update & build & test
./ccup.sh -ubt

# run executable
./ccup.sh -r

# format code
./ccup.sh -f

# lint code
./ccup.sh -l <file_path>

# install
./ccup.sh -i

# build & install
./ccup.sh -bi

# update & build & install
./ccup.sh -ubi

# clean build
./ccup.sh -c

# clean all
./ccup.sh -C

# start project from docker env
# make sure docker is pre-installed on the system.
./ccup.sh -e
```