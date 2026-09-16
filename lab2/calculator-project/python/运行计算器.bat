@echo off
chcp 65001 >nul
title 科学计算器 - 启动中...

echo ========================================================
echo               正在检查 Python 及运行环境...
echo ========================================================

where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [错误] 未检测到系统安装的 Python，请先安装 Python 并勾选 "Add Python to PATH"。
    echo 官方下载地址: https://www.python.org/
    echo.
    pause
    exit /b 1
)

python -c "import PyQt5" >nul 2>nul
if %errorlevel% neq 0 (
    python -c "import PyQt6" >nul 2>nul
    if %errorlevel% neq 0 (
        python -c "import PySide2" >nul 2>nul
        if %errorlevel% neq 0 (
            python -c "import PySide6" >nul 2>nul
            if %errorlevel% neq 0 (
                echo.
                echo [提示] 检测到当前环境未安装 PyQt 界面依赖项（PyQt5）。
                echo 正在自动为您安装 PyQt5（使用清华镜像源加速）...
                echo.
                pip install PyQt5 -i https://pypi.tuna.tsinghua.edu.cn/simple
                if %errorlevel% neq 0 (
                    echo.
                    echo [安装失败] 自动安装 PyQt5 失败，请手动打开终端执行:
                    echo     pip install PyQt5
                    echo.
                    pause
                    exit /b 1
                )
                echo.
                echo [成功] PyQt5 依赖安装完成！
                echo.
            )
        )
    )
)

echo 正在启动科学计算器...
python "%~dp0main.py"
if %errorlevel% neq 0 (
    echo.
    echo [提示] 程序异常退出，请查看上方提示信息。
    pause
)
