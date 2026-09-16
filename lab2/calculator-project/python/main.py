"""
科学计算器程序入口
"""

import sys

# 兼容 PyQt5 / PyQt6 / PySide2 / PySide6
try:
    from PyQt5.QtWidgets import QApplication
except ImportError:
    try:
        from PyQt6.QtWidgets import QApplication
    except ImportError:
        try:
            from PySide2.QtWidgets import QApplication
        except ImportError:
            try:
                from PySide6.QtWidgets import QApplication
            except ImportError:
                print("\n" + "=" * 55)
                print("【错误】未检测到 Qt GUI 依赖库！")
                print("请在终端或命令行中执行以下命令安装依赖：")
                print("    pip install PyQt5")
                print("国内用户推荐使用清华镜像源加速：")
                print("    pip install PyQt5 -i https://pypi.tuna.tsinghua.edu.cn/simple")
                print("=" * 55 + "\n")
                sys.exit(1)

from main_window import MainWindow


def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_() if hasattr(app, 'exec_') else app.exec())


if __name__ == '__main__':
    main()
