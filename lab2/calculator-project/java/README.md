# Java Swing 科学计算器

本项目为从 `javascript/` 原始科学计算器完全原样改写的 **Java (Swing)** 版本，提供与 Web 端一致的高保真视觉体验与完整的科学计算功能。基于标准 Java SE 构建，无需任何第三方外部依赖。

---

## ⚡ 运行前必读（环境准备）

本项目基于 **Java 语言 (Java Swing)** 开发。在运行前，请确保系统中已安装 Java 运行时环境（JRE 或 JDK）。

### 1. 检查 Java 环境
打开终端（CMD 或 PowerShell），执行以下命令检查是否安装了 Java：
```powershell
java -version
```
- 若显示类似 `java version "1.8..."` 或 `openjdk version "21..."` 等版本信息，说明环境已就绪。
- 本项目预编译字节码目标为 **Java 8**，可在 **Java 8 ~ Java 25** 的任意 JDK / JRE 版本上无缝直接运行。

### 2. 若未安装 Java
- 可前往 Oracle 官网或 Adoptium 下载安装 [Eclipse Temurin OpenJDK](https://adoptium.net/) 或 Oracle JDK。
- 安装时请勾选 **"Add to PATH"**（将 Java 添加到系统环境变量）。

---

## 🚀 运行方式

本项目提供了多种运行方式，任选一种即可：

### 方式一：双击运行（最简便，推荐）
- 直接双击当前目录下的 **`运行计算器.bat`** 或 **`run.bat`** 即可一键启动。
- 若系统已关联 `.jar` 文件的打开方式，也可以直接双击 **`calculator.jar`** 启动。

### 方式二：命令行运行独立可执行 JAR
在终端中进入当前目录，直接运行预先打包好的 JAR 包：
```powershell
java -jar calculator.jar
```

### 方式三：命令行直接运行 Class 类
在当前目录下执行：
```powershell
java Main
```

### 方式四：在 VS Code / IntelliJ IDEA 中运行
- **VS Code**：在 VS Code 中打开本 `java` 文件夹，安装并启用 *Extension Pack for Java* 插件，打开 `Main.java`，点击右上角运行按钮或按 `F5` 启动。
- **IntelliJ IDEA**：以项目形式打开当前文件夹，直接定位到 `Main.java` 并右键点击 **Run 'Main.main()'**。

---

## 🛠️ 从源码重新编译

如果对源码进行了修改，可使用以下命令重新编译源码：

```powershell
# 1. 使用 UTF-8 编码编译所有 Java 源文件（兼容 Java 8+）
javac --release 8 -encoding UTF-8 *.java

# 2. 重新打包为独立可执行 JAR 文件
jar --create --file calculator.jar --main-class Main *.class
```

---

## 📁 项目结构与对应关系

| 文件名 | 对应原 JS 项目 | 作用与职责 |
| :--- | :--- | :--- |
| **`Calculator.java`** | `script.js` 中的 `Calculator` 类 | 核心算法与状态机：处理输入数值、运算符、函数调用、常数、括号、角度/弧度切换及历史记录 |
| **`ExpressionParser.java`** | `script.js` 中的 `eval / Function` | 递归下降数学表达式解析器，替代 JS 原生 eval，安全解析四则运算、高优先级幂运算及负数 |
| **`MainWindow.java`** | `index.html` + `style.css` | 基于 Swing 构建的现代化 GUI 界面，高保真还原网页端圆角卡片布局、配色体系、渐变背景及按键交互 |
| **`Main.java`** | 程序主入口 | 应用程序启动点，配置原生 Look and Feel，并提供全局未捕获异常弹窗拦截 |
| **`calculator.jar`** | - | 独立可执行 JAR 包，包含全部已编译类与入口清单，解压即用 |
| **`运行计算器.bat`** | - | Windows 一键启动批处理脚本（解决终端编码及路径问题） |
| **`run.bat`** | - | 一键启动批处理脚本（纯 ASCII 英文兼容版） |
| **`README.md`** | - | 本项目使用与开发文档 |

---

## 🧮 支持的计算功能

1. **基础四则运算与运算符**：
   - 加法（`+`）、减法（`-`）、乘法（`×`）、除法（`÷`）
   - 括号优先级匹配（`(` 与 `)`）
   - 幂运算（`x^y`）
   - 清除全部（`AC`）、清除当前输入（`CE`）、退格删除（`⌫`）

2. **科学函数**：
   - 三角函数：正弦（`sin`）、余弦（`cos`）、正切（`tan`）
   - 对数函数：常用对数（`log`，以 10 为底）、自然对数（`ln`，以 e 为底）
   - 根号：平方根（`√`）
   - 数学常数：圆周率（`π` $\approx 3.14159$）、自然底数（`e` $\approx 2.71828$）
   - 进阶函数：阶乘（`n!`）、绝对值（`|x|`）

3. **角度 / 弧度模式**：
   - 底部提供 **角度模式 (deg)** 与 **弧度模式 (rad)** 实时无缝切换按钮及指示器，三角函数求值自动遵循选定模式。

---

## ❓ 常见问题排查（FAQ）

### Q1：双击运行闪退或没有反应？
- **原因 1**：计算机尚未将 Java 安装路径加入到系统环境变量 `PATH` 中。
  - **排查**：打开 CMD 输入 `java -version`，若提示 `'java' 不是内部或外部命令`，请重新安装 JDK 并勾选设置环境变量。
- **原因 2**：在某些受限或未关联 `.jar` 的系统中，请直接在终端运行 `java -jar calculator.jar` 或执行 `运行计算器.bat`。

### Q2：计算器显示“错误”字样？
- **原因**：算式中存在非法数学运算（如除以 0、对负数开平方根、对非正数取对数等）。
- **处理**：点击 `AC` 重新输入合法运算式即可。
