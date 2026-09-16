#include "Calculator.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <regex>
#include <stdexcept>
#include <cctype>
#include <limits>

namespace {
    const double PI_VAL = 3.141592653589793;
    const double E_VAL = 2.718281828459045;

    // 字符串全局替换辅助函数
    std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
        if (from.empty()) return str;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    // 正则回调替换辅助函数 (模拟 JavaScript 中的 String.prototype.replace(regex, callback))
    std::string replaceRegexWithCallback(
        const std::string& str,
        const std::regex& re,
        const std::function<std::string(const std::smatch&)>& callback
    ) {
        std::string result;
        auto words_begin = std::sregex_iterator(str.begin(), str.end(), re);
        auto words_end = std::sregex_iterator();
        size_t last_pos = 0;
        for (auto it = words_begin; it != words_end; ++it) {
            const std::smatch& match = *it;
            result.append(str, last_pos, match.position() - last_pos);
            result.append(callback(match));
            last_pos = match.position() + match.length();
        }
        result.append(str, last_pos, str.length() - last_pos);
        return result;
    }

    // 将浮点数转换为字符串 (用于函数内部代换，保留高精度)
    std::string numberToString(double val) {
        std::ostringstream oss;
        oss << std::setprecision(16) << val;
        return oss.str();
    }

    // UTF-8 字符计数与删除辅助函数
    size_t getUtf8Length(const std::string& str) {
        size_t length = 0;
        for (unsigned char c : str) {
            if ((c & 0xC0) != 0x80) {
                length++;
            }
        }
        return length;
    }

    void popBackUtf8(std::string& str) {
        if (str.empty()) return;
        while (!str.empty() && ((static_cast<unsigned char>(str.back()) & 0xC0) == 0x80)) {
            str.pop_back();
        }
        if (!str.empty()) {
            str.pop_back();
        }
    }

    // 表达式递归下降解析器 (模拟 JavaScript Function('"use strict"; return (' + expr + ')')())
    class ArithmeticParser {
        std::string src;
        size_t pos;

        void skipWhitespace() {
            while (pos < src.size() && std::isspace(static_cast<unsigned char>(src[pos]))) {
                pos++;
            }
        }

        char peek() {
            skipWhitespace();
            if (pos < src.size()) return src[pos];
            return '\0';
        }

        char get() {
            skipWhitespace();
            if (pos < src.size()) return src[pos++];
            return '\0';
        }

        bool match(const std::string& s) {
            skipWhitespace();
            if (src.compare(pos, s.size(), s) == 0) {
                pos += s.size();
                return true;
            }
            return false;
        }

    public:
        ArithmeticParser(const std::string& str) : src(str), pos(0) {}

        double parse() {
            skipWhitespace();
            if (src.empty() || pos >= src.size()) {
                throw std::runtime_error("Empty expression");
            }
            double val = parseExpression();
            skipWhitespace();
            if (pos < src.size()) {
                throw std::runtime_error("Unexpected trailing character");
            }
            return val;
        }

    private:
        double parseExpression() {
            return parseAddition();
        }

        double parseAddition() {
            double left = parseMultiplication();
            while (true) {
                skipWhitespace();
                if (pos < src.size() && src[pos] == '+') {
                    pos++;
                    double right = parseMultiplication();
                    left += right;
                } else if (pos < src.size() && src[pos] == '-' && (pos + 1 >= src.size() || src[pos + 1] != '-')) {
                    pos++;
                    double right = parseMultiplication();
                    left -= right;
                } else {
                    break;
                }
            }
            return left;
        }

        double parseMultiplication() {
            double left = parseExponent();
            while (true) {
                skipWhitespace();
                if (pos < src.size() && src[pos] == '*' && (pos + 1 >= src.size() || src[pos + 1] != '*')) {
                    pos++;
                    double right = parseExponent();
                    left *= right;
                } else if (pos < src.size() && src[pos] == '/') {
                    pos++;
                    double right = parseExponent();
                    left /= right;
                } else {
                    break;
                }
            }
            return left;
        }

        double parseExponent() {
            double left = parseUnary();
            skipWhitespace();
            if (match("**")) {
                // 右结合性: a ** b ** c = a ** (b ** c)
                double right = parseExponent();
                return std::pow(left, right);
            }
            return left;
        }

        double parseUnary() {
            skipWhitespace();
            if (match("+")) {
                return parseUnary();
            }
            if (match("-")) {
                return -parseUnary();
            }
            return parsePrimary();
        }

        double parsePrimary() {
            skipWhitespace();
            char c = peek();
            if (c == '(') {
                get();
                double val = parseExpression();
                if (get() != ')') {
                    throw std::runtime_error("Mismatched parentheses");
                }
                return val;
            }

            size_t start = pos;
            if (pos < src.size() && (std::isdigit(static_cast<unsigned char>(src[pos])) || src[pos] == '.')) {
                bool hasDot = (src[pos] == '.');
                pos++;
                while (pos < src.size()) {
                    char ch = src[pos];
                    if (std::isdigit(static_cast<unsigned char>(ch))) {
                        pos++;
                    } else if (ch == '.' && !hasDot) {
                        hasDot = true;
                        pos++;
                    } else if ((ch == 'e' || ch == 'E') && pos > start) {
                        size_t expPos = pos;
                        pos++;
                        if (pos < src.size() && (src[pos] == '+' || src[pos] == '-')) {
                            pos++;
                        }
                        if (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) {
                            while (pos < src.size() && std::isdigit(static_cast<unsigned char>(src[pos]))) pos++;
                        } else {
                            pos = expPos;
                            break;
                        }
                    } else {
                        break;
                    }
                }
                std::string numStr = src.substr(start, pos - start);
                try {
                    return std::stod(numStr);
                } catch (...) {
                    throw std::runtime_error("Invalid number: " + numStr);
                }
            }

            throw std::runtime_error("Syntax error");
        }
    };
}

Calculator::Calculator()
    : current("0"), history(""), isNewCalculation(true), angleMode("deg"), onDisplayUpdate(nullptr) {
    updateDisplay();
}

void Calculator::setDisplayCallback(DisplayCallback callback) {
    onDisplayUpdate = callback;
}

// 更新显示 (模拟前端更新)
void Calculator::updateDisplay() {
    if (onDisplayUpdate) {
        onDisplayUpdate(history, current);
    }
}

// 输入数字 (忠实还原 JS 逻辑及 bug)
void Calculator::inputNumber(const std::string& num) {
    if (isNewCalculation) {
        current = num;
        isNewCalculation = false;
    } else {
        if (current == "0" && num != ".") {
            current = num;
        } else {
            current += num;
        }
    }
    updateDisplay();
}

// 输入运算符 (忠实还原 JS 逻辑及 bug)
void Calculator::inputOperator(const std::string& op) {
    if (isNewCalculation && op != "(" && op != ")") {
        history = current + " " + op + " ";
    } else {
        if (op == "(") {
            if (current == "0" || isNewCalculation) {
                current = "(";
            } else {
                current += "(";
            }
        } else if (op == ")") {
            current += ")";
        } else {
            current += " " + op + " ";
        }
    }
    isNewCalculation = false;
    updateDisplay();
}

// 输入函数 (忠实还原 JS 逻辑及 bug)
void Calculator::inputFunction(const std::string& func) {
    if (isNewCalculation) {
        current = func + "(";
    } else {
        current += func + "(";
    }
    isNewCalculation = false;
    updateDisplay();
}

// 输入常数 (忠实还原 JS 逻辑及 bug)
void Calculator::inputConstant(const std::string& constant) {
    if (isNewCalculation) {
        current = constant;
        isNewCalculation = false;
    } else {
        if (current == "0") {
            current = constant;
        } else {
            current += constant;
        }
    }
    updateDisplay();
}

// 清除所有 (AC)
void Calculator::clearAll() {
    current = "0";
    history = "";
    isNewCalculation = true;
    updateDisplay();
}

// 清除当前输入 (CE，忠实保留未重置 isNewCalculation 的特性)
void Calculator::clearEntry() {
    current = "0";
    updateDisplay();
}

// 删除最后一个字符 (⌫)
void Calculator::deleteLast() {
    if (getUtf8Length(current) > 1) {
        popBackUtf8(current);
    } else {
        current = "0";
    }
    updateDisplay();
}

// 角度转弧度
double Calculator::toRadians(double degrees) const {
    return degrees * PI_VAL / 180.0;
}

// 弧度转角度
double Calculator::toDegrees(double radians) const {
    return radians * 180.0 / PI_VAL;
}

// 阶乘函数 (忠实保留 loop 步进 double 以及 NaN 处理的特性)
double Calculator::factorial(double n) const {
    if (n < 0) return std::numeric_limits<double>::quiet_NaN();
    if (n == 0 || n == 1) return 1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// 处理数学函数 (忠实保留正则 [^)]+ 无法匹配嵌套括号以及常数 e 误替换等特性)
std::string Calculator::evaluateFunction(std::string expression) {
    // 替换常数
    expression = replaceAll(expression, "π", "3.141592653589793");
    expression = std::regex_replace(expression, std::regex("e"), "2.718281828459045");

    // 处理三角函数
    expression = replaceRegexWithCallback(expression, std::regex(R"(sin\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = std::sin(this->angleMode == "deg" ? this->toRadians(val) : val);
        return numberToString(res);
    });

    expression = replaceRegexWithCallback(expression, std::regex(R"(cos\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = std::cos(this->angleMode == "deg" ? this->toRadians(val) : val);
        return numberToString(res);
    });

    expression = replaceRegexWithCallback(expression, std::regex(R"(tan\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = std::tan(this->angleMode == "deg" ? this->toRadians(val) : val);
        return numberToString(res);
    });

    // 处理对数函数
    expression = replaceRegexWithCallback(expression, std::regex(R"(log\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = std::log10(val);
        return numberToString(res);
    });

    expression = replaceRegexWithCallback(expression, std::regex(R"(ln\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = std::log(val);
        return numberToString(res);
    });

    // 处理平方根
    expression = replaceRegexWithCallback(expression, std::regex(R"(sqrt\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = std::sqrt(val);
        return numberToString(res);
    });

    // 处理阶乘
    expression = replaceRegexWithCallback(expression, std::regex(R"(factorial\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = this->factorial(val);
        return numberToString(res);
    });

    // 处理绝对值
    expression = replaceRegexWithCallback(expression, std::regex(R"(abs\(([^)]+)\))"), [this](const std::smatch& m) {
        double val = this->evaluateExpression(m[1].str());
        double res = std::abs(val);
        return numberToString(res);
    });

    // 处理幂运算
    expression = replaceAll(expression, "^", "**");

    return expression;
}

// 计算表达式
double Calculator::evaluateExpression(std::string expr) {
    try {
        expr = evaluateFunction(expr);
        ArithmeticParser parser(expr);
        return parser.parse();
    } catch (...) {
        throw std::runtime_error("计算错误");
    }
}

// 执行计算 (=)
void Calculator::calculate() {
    try {
        history = current;
        std::string expression = current;

        // 替换显示符号为运算符
        expression = replaceAll(expression, "×", "*");
        expression = replaceAll(expression, "÷", "/");

        double result = evaluateExpression(expression);

        if (std::isnan(result) || std::isinf(result)) {
            throw std::runtime_error("计算错误");
        }

        current = formatResult(result);
        isNewCalculation = true;
        updateDisplay();
    } catch (...) {
        current = "错误";
        isNewCalculation = true;
        updateDisplay();
    }
}

// 格式化结果
std::string Calculator::formatResult(double result) const {
    // 处理非常大或非常小的数字
    if (std::abs(result) > 1e15 || (std::abs(result) < 1e-10 && result != 0.0)) {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(6) << result;
        std::string s = oss.str();
        size_t ePos = s.find('e');
        if (ePos != std::string::npos && ePos + 2 < s.size()) {
            char sign = s[ePos + 1];
            if ((sign == '+' || sign == '-') && s[ePos + 2] == '0' && ePos + 3 < s.size()) {
                s.erase(ePos + 2, 1);
            }
        }
        return s;
    }

    // 保留适当的小数位数
    std::ostringstream oss;
    oss << std::setprecision(15) << result;
    std::string str = oss.str();

    size_t dotPos = str.find('.');
    if (dotPos != std::string::npos) {
        size_t decimalPlaces = str.length() - dotPos - 1;
        if (decimalPlaces > 10) {
            std::ostringstream fixedOss;
            fixedOss << std::fixed << std::setprecision(10) << result;
            double rounded = std::stod(fixedOss.str());
            std::ostringstream roundOss;
            roundOss << std::setprecision(15) << rounded;
            return roundOss.str();
        }
    }

    return str;
}

// 切换角度/弧度模式
void Calculator::toggleMode() {
    angleMode = (angleMode == "deg" ? "rad" : "deg");
}
