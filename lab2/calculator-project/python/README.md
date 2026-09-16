# Python Qt 科学计算器

本项目为从 `javascript/` 原始科学计算器完全原样改写的 Python + Qt 版本，提供与 Web 端一致的视觉体验与科学计算功能。

---

## ⚡ 运行前必读（环境准备）

本项目基于 Python 和 Qt 图形界面库开发。在首次运行前，**必须确保已安装 Python 及 GUI 依赖库**。

### 1. 安装 Python
- 要求 **Python 3.8 及以上** 版本。
- 安装 Python 时，请务必勾选 **“Add Python to PATH”**（将 Python 添加到系统环境变量）。

### 2. 安装界面依赖库（PyQt5）
在终端（CMD 或 PowerShell）中执行以下命令安装依赖：
```powershell
pip install PyQt5
```

> [!TIP]
> **网络下载慢或超时？**
> 可使用国内清华镜像源加速安装：
> ```powershell
> pip install PyQt5 -i https://pypi.tuna.tsinghua.edu.cn/simple
> ```

> [!NOTE]
> 本程序也完全兼容 `PyQt6`、`PySide2`、`PySide6`，已安装其中任一库即可直接运行。

---

## 🚀 运行方式

### 方式一：双击运行（推荐）
直接双击当前目录下的 **`运行计算器.bat`** 即可启动。
- 该脚本内置了环境检测：若检测到未安装依赖，会引导或自动协助安装 PyQt5。

### 方式二：命令行运行
1. 打开 PowerShell、CMD 或终端；
2. 进入当前 `calculator-project/python` 目录：
   ```powershell
   cd calculator-project/python
   ```
3. 执行启动命令：
   ```powershell
   python main.py
   ```

### 方式三：在 VS Code 中直接运行
1. 在 VS Code 中打开本目录；
2. 打开 `main.py`；
3. 点击右上角 **播放按钮（Run Python File）** 或按下 `F5` 即可运行。

---

## ❓ 常见问题排查（FAQ）

### Q1：双击 `运行计算器.bat` 提示“依赖项不足”或闪退？
- **原因**：当前系统的全局 Python 环境中未安装 `PyQt5`，或者系统中存在多个 Python 版本导致环境未对应。
- **解决方法**：
  1. 按 `Win + R` 键，输入 `cmd` 并回车打开命令提示符；
  2. 运行 `python --version` 确认 Python 正常识别；
  3. 执行 `pip install PyQt5 -i https://pypi.tuna.tsinghua.edu.cn/simple` 安装依赖；
  4. 安装完成后重新双击 `运行计算器.bat`。

### Q2：提示 `'python' 不是内部或外部命令，也不是可运行的程序`？
- **原因**：Python 未安装或未添加到 Windows 的系统环境变量 `PATH` 中。
- **解决方法**：
  1. 重新运行 Python 安装包，选择 **Modify** 并勾选 **Add Python to PATH**；
  2. 或在“系统属性 -> 环境变量”中将 Python 安装路径（如 `C:\Users\用户名\AppData\Local\Programs\Python\Python3x` 及 `Scripts` 目录）手动添加到 `Path` 中。

---

## 📁 项目结构
- `calculator.py`: 计算器核心算法与计算逻辑（完整对应原 JavaScript 的 Calculator 类）
- `main_window.py`: PyQt 界面与按键、键盘事件响应（高保真还原网页端界面色彩、按键阴影与交互）
- `style.qss`: Qt 样式表（定义配色、圆角、高光渐变与按键悬浮/按下动效）
- `main.py`: 程序主入口（包含环境依赖检查与安全退出处理）
- `运行计算器.bat`: 一键启动批处理脚本（内置依赖环境自检与引导）
- `README.md`: 本使用文档
