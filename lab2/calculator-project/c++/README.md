# 如何运行该计算器

## 1. 直接运行已构建的 C++ Qt 桌面端计算器（推荐）

计算器程序已经编译并打包部署完成，你可以直接通过以下任一方式打开：

### 方式一：终端命令行启动
在当前项目目录下（或 `build` 目录下）直接运行：
```powershell
.\build\calculator.exe
```
或者如果你当前已经在 `build` 目录内：
```powershell
.\calculator.exe
```

### 方式二：双击运行
在 Windows 文件资源管理器中进入 "你存放的目录\lab1\calculator-project\c++\build\calculator.exe" 目录，双击 **`calculator.exe`** 即可直接打开。

---

## 2. 从源码重新编译与运行

如果你需要重新编译源码，可使用本机已安装的 Qt 6 套件：

### 方式一：使用 Qt Creator
1. 打开 `C:\Qt\Tools\QtCreator\bin\qtcreator.exe`。
2. 点击 **文件 -> 打开文件或项目**，选择本项目目录下的 `CMakeLists.txt`（或 `CalculatorQt.pro`）。
3. 点击左下角的 **运行 (Ctrl + R)** 即可。

### 方式二：使用本机 Qt 自带的 CMake 命令行构建
```powershell
# 在项目根目录下执行
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" -S . -B build -DCMAKE_PREFIX_PATH="C:\Qt\6.9.1\mingw_64" -G "Ninja" -DCMAKE_MAKE_PROGRAM="C:\Qt\Tools\Ninja\ninja.exe" -DCMAKE_CXX_COMPILER="C:\Qt\Tools\mingw1310_64\bin\g++.exe"
& "C:\Qt\Tools\CMake_64\bin\cmake.exe" --build build
.\build\calculator.exe
```
