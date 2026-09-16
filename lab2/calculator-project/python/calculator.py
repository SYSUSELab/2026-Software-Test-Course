"""
科学计算器业务逻辑模块
原样对应 javascript/script.js 中的 Calculator 类及计算逻辑
"""

import math
import re


class Calculator:
    def __init__(self, display_callback=None):
        self.current = '0'
        self.history = ''
        self.is_new_calculation = True
        self.angle_mode = 'deg'  # 'deg' 为角度模式，'rad' 为弧度模式
        self.display_callback = display_callback
        self.update_display()

    # 更新显示
    def update_display(self):
        if callable(self.display_callback):
            self.display_callback(self.current, self.history)

    # 输入数字
    def input_number(self, num: str):
        if self.is_new_calculation:
            self.current = str(num)
            self.is_new_calculation = False
        else:
            if self.current == '0' and str(num) != '.':
                self.current = str(num)
            else:
                self.current += str(num)
        self.update_display()

    # 输入运算符
    def input_operator(self, operator: str):
        if self.is_new_calculation and operator not in ('(', ')'):
            self.history = self.current + ' ' + operator + ' '
        else:
            if operator == '(':
                if self.current == '0' or self.is_new_calculation:
                    self.current = '('
                else:
                    self.current += '('
            elif operator == ')':
                self.current += ')'
            else:
                self.current += ' ' + operator + ' '
        self.is_new_calculation = False
        self.update_display()

    # 输入函数
    def input_function(self, func: str):
        if self.is_new_calculation:
            self.current = func + '('
        else:
            self.current += func + '('
        self.is_new_calculation = False
        self.update_display()

    # 输入常数
    def input_constant(self, constant: str):
        if self.is_new_calculation:
            self.current = constant
            self.is_new_calculation = False
        else:
            if self.current == '0':
                self.current = constant
            else:
                self.current += constant
        self.update_display()

    # 清除所有
    def clear_all(self):
        self.current = '0'
        self.history = ''
        self.is_new_calculation = True
        self.update_display()

    # 清除当前输入
    def clear_entry(self):
        self.current = '0'
        self.update_display()

    # 删除最后一个字符
    def delete_last(self):
        if len(self.current) > 1:
            self.current = self.current[:-1]
        else:
            self.current = '0'
        self.update_display()

    # 角度转弧度
    def to_radians(self, degrees: float) -> float:
        return degrees * math.pi / 180.0

    # 弧度转角度
    def to_degrees(self, radians: float) -> float:
        return radians * 180.0 / math.pi

    # 阶乘函数
    def factorial(self, n: float) -> float:
        if n < 0:
            return float('nan')
        # 阶乘仅对非负整数有效
        if not (isinstance(n, int) or (isinstance(n, float) and n.is_integer())):
            return float('nan')
        n_int = int(n)
        if n_int == 0 or n_int == 1:
            return 1.0
        result = 1.0
        for i in range(2, n_int + 1):
            result *= i
        return result

    # 处理数学函数
    def evaluate_function(self, expression: str) -> str:
        # 替换常数
        expression = expression.replace('π', str(math.pi))
        expression = expression.replace('e', str(math.e))

        # 循环解析最内层数学函数
        func_regex = re.compile(r'(sin|cos|tan|log|ln|sqrt|factorial|abs)\(([^()]+)\)')
        while True:
            match = func_regex.search(expression)
            if not match:
                break

            func_name = match.group(1)
            arg_str = match.group(2)
            val = self.evaluate_expression(arg_str)

            if func_name == 'sin':
                angle = self.to_radians(val) if self.angle_mode == 'deg' else val
                res = math.sin(angle)
            elif func_name == 'cos':
                angle = self.to_radians(val) if self.angle_mode == 'deg' else val
                res = math.cos(angle)
            elif func_name == 'tan':
                angle = self.to_radians(val) if self.angle_mode == 'deg' else val
                res = math.tan(angle)
            elif func_name == 'log':
                if val <= 0:
                    raise ValueError('计算错误')
                res = math.log10(val)
            elif func_name == 'ln':
                if val <= 0:
                    raise ValueError('计算错误')
                res = math.log(val)
            elif func_name == 'sqrt':
                if val < 0:
                    raise ValueError('计算错误')
                res = math.sqrt(val)
            elif func_name == 'factorial':
                res = self.factorial(val)
                if math.isnan(res):
                    raise ValueError('计算错误')
            elif func_name == 'abs':
                res = abs(val)
            else:
                raise ValueError('计算错误')

            expression = expression[:match.start()] + str(res) + expression[match.end():]

        # 处理幂运算
        expression = expression.replace('^', '**')

        return expression

    # 计算表达式
    def evaluate_expression(self, expr: str) -> float:
        try:
            # 处理函数
            expr = self.evaluate_function(expr)

            # 对应 JavaScript Function('"use strict"; return (' + expr + ')')()
            # 在受限环境下安全计算四则运算及乘方
            result = eval(expr, {"__builtins__": None}, {})
            if isinstance(result, (int, float)):
                return float(result)
            raise ValueError('计算错误')
        except Exception:
            raise ValueError('计算错误')

    # 执行计算
    def calculate(self):
        try:
            self.history = self.current
            expression = self.current

            # 替换显示符号为运算符
            expression = expression.replace('×', '*')
            expression = expression.replace('÷', '/')

            result = self.evaluate_expression(expression)

            if math.isnan(result) or math.isinf(result):
                raise ValueError('计算错误')

            self.current = self.format_result(result)
            self.is_new_calculation = True
            self.update_display()
        except Exception:
            self.current = '错误'
            self.is_new_calculation = True
            self.update_display()

    # 格式化结果
    def format_result(self, result: float) -> str:
        # 处理非常大或非常小的数字
        if abs(result) > 1e15 or (abs(result) < 1e-10 and result != 0):
            return f"{result:.6e}"

        # 检查是否可直接作为整数表示
        if result.is_integer():
            return str(int(result))

        # 保留适当的小数位数 (对应 JavaScript toFixed(10))
        s = str(result)
        if '.' in s:
            decimal_places = len(s.split('.')[1])
            if decimal_places > 10:
                val = round(result, 10)
                if val.is_integer():
                    return str(int(val))
                formatted = f"{val:.10f}".rstrip('0').rstrip('.')
                return formatted if formatted else "0"

        return s

    # 切换角度/弧度模式
    def toggle_mode(self):
        self.angle_mode = 'rad' if self.angle_mode == 'deg' else 'deg'

    # 驼峰命名兼容方法，确保与原有 JS 调用名称完全一致
    inputNumber = input_number
    inputOperator = input_operator
    inputFunction = input_function
    inputConstant = input_constant
    clearAll = clear_all
    clearEntry = clear_entry
    deleteLast = delete_last
    toRadians = to_radians
    toDegrees = to_degrees
    evaluateFunction = evaluate_function
    evaluateExpression = evaluate_expression
    formatResult = format_result
    toggleMode = toggle_mode
