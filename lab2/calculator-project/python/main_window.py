"""
科学计算器 Qt 前端界面
原样对应 index.html, style.css/style.qss 以及 script.js 中的 UI 和键盘事件处理
"""

import os
import sys

# 兼容 PyQt5 / PyQt6 / PySide2 / PySide6
try:
    from PyQt5.QtWidgets import (
        QWidget, QFrame, QLabel, QPushButton,
        QVBoxLayout, QHBoxLayout, QGridLayout, QSizePolicy
    )
    from PyQt5.QtCore import Qt, QTimer
    from PyQt5.QtGui import QKeyEvent
except ImportError:
    try:
        from PyQt6.QtWidgets import (
            QWidget, QFrame, QLabel, QPushButton,
            QVBoxLayout, QHBoxLayout, QGridLayout, QSizePolicy
        )
        from PyQt6.QtCore import Qt, QTimer
        from PyQt6.QtGui import QKeyEvent
    except ImportError:
        try:
            from PySide2.QtWidgets import (
                QWidget, QFrame, QLabel, QPushButton,
                QVBoxLayout, QHBoxLayout, QGridLayout, QSizePolicy
            )
            from PySide2.QtCore import Qt, QTimer
            from PySide2.QtGui import QKeyEvent
        except ImportError:
            from PySide6.QtWidgets import (
                QWidget, QFrame, QLabel, QPushButton,
                QVBoxLayout, QHBoxLayout, QGridLayout, QSizePolicy
            )
            from PySide6.QtCore import Qt, QTimer
            from PySide6.QtGui import QKeyEvent

from calculator import Calculator

# 兼容 Qt5 / Qt6 枚举命名
ALIGN_RIGHT_VCENTER = (
    Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter
    if hasattr(Qt, 'AlignmentFlag')
    else (Qt.AlignRight | Qt.AlignVCenter)
)
CURSOR_POINTING_HAND = (
    Qt.CursorShape.PointingHandCursor
    if hasattr(Qt, 'CursorShape')
    else Qt.PointingHandCursor
)
FOCUS_STRONG = (
    Qt.FocusPolicy.StrongFocus
    if hasattr(Qt, 'FocusPolicy')
    else Qt.StrongFocus
)


class MainWindow(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setObjectName("centralWidget")
        self.setWindowTitle("科学计算器")
        # 宽 410，高 800
        self.setFixedSize(410, 800)

        # 必须先初始化 UI 组件
        self.init_ui()
        self.load_style()

        # 初始化计算器后端，随后绑定显示回调
        self.calculator = Calculator()
        self.calculator.display_callback = self.on_display_update

        # 初始刷新显示
        self.update_display()

    def init_ui(self):
        # 对应 index.html 中的外层布局
        body_layout = QVBoxLayout(self)
        body_layout.setContentsMargins(18, 18, 18, 18)

        # 对应 index.html 中的 <div class="calculator">
        self.calculator_container = QFrame(self)
        self.calculator_container.setObjectName("calculator")
        calc_layout = QVBoxLayout(self.calculator_container)
        calc_layout.setContentsMargins(20, 20, 20, 20)
        calc_layout.setSpacing(16)

        # 对应 index.html 中的 <div class="display">
        self.display_container = QFrame(self.calculator_container)
        self.display_container.setObjectName("display")
        self.display_container.setFixedHeight(105)
        display_layout = QVBoxLayout(self.display_container)
        display_layout.setContentsMargins(18, 14, 18, 14)
        display_layout.setSpacing(2)

        # <div class="history" id="history"></div>
        self.lbl_history = QLabel("", self.display_container)
        self.lbl_history.setObjectName("history")
        self.lbl_history.setAlignment(ALIGN_RIGHT_VCENTER)
        self.lbl_history.setWordWrap(True)

        # <div class="current" id="current">0</div>
        self.lbl_current = QLabel("0", self.display_container)
        self.lbl_current.setObjectName("current")
        self.lbl_current.setAlignment(ALIGN_RIGHT_VCENTER)
        self.lbl_current.setWordWrap(True)

        display_layout.addWidget(self.lbl_history)
        display_layout.addWidget(self.lbl_current)
        calc_layout.addWidget(self.display_container)

        # 对应 index.html 中的 <div class="buttons">
        buttons_grid = QGridLayout()
        buttons_grid.setHorizontalSpacing(13)
        buttons_grid.setVerticalSpacing(13)

        # 行 0: AC, CE, ⌫, ÷
        btn_ac = self.create_button("AC", "btn-function")
        btn_ac.clicked.connect(self.clear_all)
        buttons_grid.addWidget(btn_ac, 0, 0)

        btn_ce = self.create_button("CE", "btn-function")
        btn_ce.clicked.connect(self.clear_entry)
        buttons_grid.addWidget(btn_ce, 0, 1)

        btn_del = self.create_button("⌫", "btn-function")
        btn_del.clicked.connect(self.delete_last)
        buttons_grid.addWidget(btn_del, 0, 2)

        btn_div = self.create_button("÷", "btn-operator")
        btn_div.clicked.connect(lambda: self.input_operator("/"))
        buttons_grid.addWidget(btn_div, 0, 3)

        # 行 1: sin, cos, tan, ×
        btn_sin = self.create_button("sin", "btn-scientific")
        btn_sin.clicked.connect(lambda: self.input_function("sin"))
        buttons_grid.addWidget(btn_sin, 1, 0)

        btn_cos = self.create_button("cos", "btn-scientific")
        btn_cos.clicked.connect(lambda: self.input_function("cos"))
        buttons_grid.addWidget(btn_cos, 1, 1)

        btn_tan = self.create_button("tan", "btn-scientific")
        btn_tan.clicked.connect(lambda: self.input_function("tan"))
        buttons_grid.addWidget(btn_tan, 1, 2)

        btn_mul = self.create_button("×", "btn-operator")
        btn_mul.clicked.connect(lambda: self.input_operator("*"))
        buttons_grid.addWidget(btn_mul, 1, 3)

        # 行 2: log, ln, √, -
        btn_log = self.create_button("log", "btn-scientific")
        btn_log.clicked.connect(lambda: self.input_function("log"))
        buttons_grid.addWidget(btn_log, 2, 0)

        btn_ln = self.create_button("ln", "btn-scientific")
        btn_ln.clicked.connect(lambda: self.input_function("ln"))
        buttons_grid.addWidget(btn_ln, 2, 1)

        btn_sqrt = self.create_button("√", "btn-scientific")
        btn_sqrt.clicked.connect(lambda: self.input_function("sqrt"))
        buttons_grid.addWidget(btn_sqrt, 2, 2)

        btn_sub = self.create_button("-", "btn-operator")
        btn_sub.clicked.connect(lambda: self.input_operator("-"))
        buttons_grid.addWidget(btn_sub, 2, 3)

        # 行 3: x^y, 7, 8, 9
        btn_pow = self.create_button("x^y", "btn-scientific")
        btn_pow.clicked.connect(lambda: self.input_operator("^"))
        buttons_grid.addWidget(btn_pow, 3, 0)

        btn_7 = self.create_button("7", "btn-number")
        btn_7.clicked.connect(lambda: self.input_number("7"))
        buttons_grid.addWidget(btn_7, 3, 1)

        btn_8 = self.create_button("8", "btn-number")
        btn_8.clicked.connect(lambda: self.input_number("8"))
        buttons_grid.addWidget(btn_8, 3, 2)

        btn_9 = self.create_button("9", "btn-number")
        btn_9.clicked.connect(lambda: self.input_number("9"))
        buttons_grid.addWidget(btn_9, 3, 3)

        # 行 4: π, 4, 5, 6
        btn_pi = self.create_button("π", "btn-scientific")
        btn_pi.clicked.connect(lambda: self.input_constant("π"))
        buttons_grid.addWidget(btn_pi, 4, 0)

        btn_4 = self.create_button("4", "btn-number")
        btn_4.clicked.connect(lambda: self.input_number("4"))
        buttons_grid.addWidget(btn_4, 4, 1)

        btn_5 = self.create_button("5", "btn-number")
        btn_5.clicked.connect(lambda: self.input_number("5"))
        buttons_grid.addWidget(btn_5, 4, 2)

        btn_6 = self.create_button("6", "btn-number")
        btn_6.clicked.connect(lambda: self.input_number("6"))
        buttons_grid.addWidget(btn_6, 4, 3)

        # 行 5: e, 1, 2, 3
        btn_e = self.create_button("e", "btn-scientific")
        btn_e.clicked.connect(lambda: self.input_constant("e"))
        buttons_grid.addWidget(btn_e, 5, 0)

        btn_1 = self.create_button("1", "btn-number")
        btn_1.clicked.connect(lambda: self.input_number("1"))
        buttons_grid.addWidget(btn_1, 5, 1)

        btn_2 = self.create_button("2", "btn-number")
        btn_2.clicked.connect(lambda: self.input_number("2"))
        buttons_grid.addWidget(btn_2, 5, 2)

        btn_3 = self.create_button("3", "btn-number")
        btn_3.clicked.connect(lambda: self.input_number("3"))
        buttons_grid.addWidget(btn_3, 5, 3)

        # 行 6: (, ), 0, .
        btn_lparen = self.create_button("(", "btn-function")
        btn_lparen.clicked.connect(lambda: self.input_operator("("))
        buttons_grid.addWidget(btn_lparen, 6, 0)

        btn_rparen = self.create_button(")", "btn-function")
        btn_rparen.clicked.connect(lambda: self.input_operator(")"))
        buttons_grid.addWidget(btn_rparen, 6, 1)

        btn_0 = self.create_button("0", "btn-number")
        btn_0.clicked.connect(lambda: self.input_number("0"))
        buttons_grid.addWidget(btn_0, 6, 2)

        btn_dot = self.create_button(".", "btn-number")
        btn_dot.clicked.connect(lambda: self.input_number("."))
        buttons_grid.addWidget(btn_dot, 6, 3)

        # 行 7: +, n!, |x|, =
        btn_add = self.create_button("+", "btn-operator")
        btn_add.clicked.connect(lambda: self.input_operator("+"))
        buttons_grid.addWidget(btn_add, 7, 0)

        btn_fact = self.create_button("n!", "btn-scientific")
        btn_fact.clicked.connect(lambda: self.input_function("factorial"))
        buttons_grid.addWidget(btn_fact, 7, 1)

        btn_abs = self.create_button("|x|", "btn-scientific")
        btn_abs.clicked.connect(lambda: self.input_function("abs"))
        buttons_grid.addWidget(btn_abs, 7, 2)

        btn_eq = self.create_button("=", "btn-equals")
        btn_eq.clicked.connect(self.calculate)
        buttons_grid.addWidget(btn_eq, 7, 3)

        calc_layout.addLayout(buttons_grid)

        # 对应 index.html 中的 <div class="mode-toggle">
        mode_layout = QHBoxLayout()
        mode_layout.setContentsMargins(0, 4, 4, 0)

        self.btn_mode = QPushButton("切换到弧度模式", self.calculator_container)
        self.btn_mode.setObjectName("btnMode")
        self.btn_mode.setFixedHeight(46)
        self.btn_mode.setCursor(CURSOR_POINTING_HAND)
        self.btn_mode.clicked.connect(self.toggle_mode)

        self.mode_indicator = QLabel("角度模式", self.calculator_container)
        self.mode_indicator.setObjectName("modeIndicator")
        self.mode_indicator.setAlignment(ALIGN_RIGHT_VCENTER)

        mode_layout.addWidget(self.btn_mode)
        mode_layout.addStretch()
        mode_layout.addWidget(self.mode_indicator)

        calc_layout.addLayout(mode_layout)
        body_layout.addWidget(self.calculator_container)

        self.setFocusPolicy(FOCUS_STRONG)

    def create_button(self, text: str, class_name: str) -> QPushButton:
        btn = QPushButton(text, self.calculator_container)
        btn.setProperty("btnClass", class_name)
        btn.setCursor(CURSOR_POINTING_HAND)
        btn.setSizePolicy(QSizePolicy.Policy.Expanding if hasattr(QSizePolicy, 'Policy') else QSizePolicy.Expanding,
                          QSizePolicy.Policy.Fixed if hasattr(QSizePolicy, 'Policy') else QSizePolicy.Fixed)
        btn.setFixedHeight(52)
        return btn

    def load_style(self):
        # 优先读取同目录下的 style.qss
        cur_dir = os.path.dirname(os.path.abspath(__file__))
        qss_path = os.path.join(cur_dir, "style.qss")
        if os.path.exists(qss_path):
            with open(qss_path, "r", encoding="utf-8") as f:
                self.setStyleSheet(f.read())
        else:
            # 内置兜底样式
            self.setStyleSheet("""
                QWidget#centralWidget { background: #606fe8; font-family: 'Segoe UI', sans-serif; }
                QFrame#calculator { background: #ffffff; border-radius: 24px; }
                QFrame#display { background: #2c3748; border-radius: 16px; }
                QLabel#history { font-size: 14px; color: #a0aec0; min-height: 18px; }
                QLabel#current { font-size: 34px; font-weight: bold; color: #ffffff; }
                QLabel#current[error="true"] { color: #e74c3c; }
                QPushButton { border: none; border-radius: 12px; font-size: 16px; font-weight: bold; color: #ffffff; }
                QPushButton[btnClass="btn-number"] { background: #4e8fd4; }
                QPushButton[btnClass="btn-number"]:hover { background: #3f7ec1; }
                QPushButton[btnClass="btn-operator"] { background: #d05344; }
                QPushButton[btnClass="btn-operator"]:hover { background: #bd4638; }
                QPushButton[btnClass="btn-function"] { background: #8fa1a4; }
                QPushButton[btnClass="btn-function"]:hover { background: #7e8f92; }
                QPushButton[btnClass="btn-scientific"] { background: #8e54a8; font-size: 14px; }
                QPushButton[btnClass="btn-scientific"]:hover { background: #7e4697; }
                QPushButton[btnClass="btn-equals"] { background: #52ab67; }
                QPushButton[btnClass="btn-equals"]:hover { background: #459959; }
                QPushButton#btnMode { background: #e59838; color: #ffffff; padding: 10px 18px; border-radius: 10px; font-size: 14px; font-weight: bold; }
                QPushButton#btnMode:hover { background: #d4882b; }
                QLabel#modeIndicator { font-size: 15px; color: #2c3748; font-weight: bold; }
            """)

    def on_display_update(self, current_val: str, history_val: str):
        self.update_display()

    # 更新屏幕显示
    def update_display(self):
        if not hasattr(self, 'lbl_history') or not hasattr(self, 'lbl_current') or not hasattr(self, 'calculator'):
            return

        self.lbl_history.setText(self.calculator.history)
        self.lbl_current.setText(self.calculator.current)

        if self.calculator.current == "错误":
            self.lbl_current.setProperty("error", True)
            self.lbl_current.style().unpolish(self.lbl_current)
            self.lbl_current.style().polish(self.lbl_current)

            # 对应 script.js 中 setTimeout(() => this.clearAll(), 2000);
            QTimer.singleShot(2000, self.on_error_timeout)
        else:
            self.lbl_current.setProperty("error", False)
            self.lbl_current.style().unpolish(self.lbl_current)
            self.lbl_current.style().polish(self.lbl_current)

    def on_error_timeout(self):
        if hasattr(self, 'calculator') and self.calculator.current == "错误":
            self.clear_all()

    # 按钮事件转发
    def input_number(self, num: str):
        self.calculator.input_number(num)

    def input_operator(self, op: str):
        self.calculator.input_operator(op)

    def input_function(self, func: str):
        self.calculator.input_function(func)

    def input_constant(self, constant: str):
        self.calculator.input_constant(constant)

    def clear_all(self):
        self.calculator.clear_all()

    def clear_entry(self):
        self.calculator.clear_entry()

    def delete_last(self):
        self.calculator.delete_last()

    def calculate(self):
        self.calculator.calculate()

    def toggle_mode(self):
        self.calculator.toggle_mode()
        if self.calculator.angle_mode == "rad":
            self.btn_mode.setText("切换到角度模式")
            self.mode_indicator.setText("弧度模式")
        else:
            self.btn_mode.setText("切换到弧度模式")
            self.mode_indicator.setText("角度模式")

    # 键盘支持 (原样对应 script.js 中的 keydown 事件监听)
    def keyPressEvent(self, event: QKeyEvent):
        key = event.key()
        text = event.text()

        # 数字键
        if text and text in '0123456789':
            self.input_number(text)
        elif (hasattr(Qt, 'Key_0') and Qt.Key_0 <= key <= Qt.Key_9) or \
             (hasattr(Qt, 'Key') and hasattr(Qt.Key, 'Key_0') and Qt.Key.Key_0 <= key <= Qt.Key.Key_9):
            digit = key - (Qt.Key_0 if hasattr(Qt, 'Key_0') else Qt.Key.Key_0)
            self.input_number(str(digit))
        # 运算符
        elif text == '+':
            self.input_operator('+')
        elif text == '-':
            self.input_operator('-')
        elif text == '*':
            self.input_operator('*')
        elif text == '/':
            self.input_operator('/')
        # 小数点
        elif text == '.':
            self.input_number('.')
        # 括号
        elif text == '(':
            self.input_operator('(')
        elif text == ')':
            self.input_operator(')')
        # 等号和回车
        elif text == '=' or key in (
            getattr(Qt, 'Key_Return', None),
            getattr(Qt, 'Key_Enter', None),
            getattr(getattr(Qt, 'Key', None), 'Key_Return', None),
            getattr(getattr(Qt, 'Key', None), 'Key_Enter', None),
        ):
            self.calculate()
        # 退格键
        elif key in (
            getattr(Qt, 'Key_Backspace', None),
            getattr(getattr(Qt, 'Key', None), 'Key_Backspace', None)
        ):
            self.delete_last()
        # 清除键 (Escape)
        elif key in (
            getattr(Qt, 'Key_Escape', None),
            getattr(getattr(Qt, 'Key', None), 'Key_Escape', None)
        ):
            self.clear_all()
        # Delete 键
        elif key in (
            getattr(Qt, 'Key_Delete', None),
            getattr(getattr(Qt, 'Key', None), 'Key_Delete', None)
        ):
            self.clear_entry()
        else:
            super().keyPressEvent(event)
