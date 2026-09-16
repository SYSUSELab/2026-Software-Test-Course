#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <functional>

/**
 * @brief 科学计算器类
 * 从 Lab1-4/calculator-project/script.js 严格 1:1 翻译为 C++。
 * 注意：按照要求，原 JavaScript 代码中的所有逻辑缺陷与 bug 均被忠实保留，未作修复。
 */
class Calculator {
public:
    // 回调函数类型：用于模拟前端 updateDisplay 的状态通知
    using DisplayCallback = std::function<void(const std::string& history, const std::string& current)>;

private:
    std::string current;
    std::string history;
    bool isNewCalculation;
    std::string angleMode; // "deg" 表示角度制，"rad" 表示弧度制
    DisplayCallback onDisplayUpdate;

public:
    Calculator();

    // 更新显示
    void updateDisplay();

    // 设置显示更新回调
    void setDisplayCallback(DisplayCallback callback);

    // 输入数字
    void inputNumber(const std::string& num);

    // 输入运算符
    void inputOperator(const std::string& op);

    // 输入函数
    void inputFunction(const std::string& func);

    // 输入常数
    void inputConstant(const std::string& constant);

    // 清除所有 (AC)
    void clearAll();

    // 清除当前输入 (CE)
    void clearEntry();

    // 删除最后一个字符 (⌫)
    void deleteLast();

    // 角度转弧度
    double toRadians(double degrees) const;

    // 弧度转角度
    double toDegrees(double radians) const;

    // 阶乘函数
    double factorial(double n) const;

    // 处理数学函数
    std::string evaluateFunction(std::string expression);

    // 计算表达式
    double evaluateExpression(std::string expr);

    // 执行计算 (=)
    void calculate();

    // 格式化结果
    std::string formatResult(double result) const;

    // 切换角度/弧度模式
    void toggleMode();

    // 状态获取器 (便于测试与状态检查)
    const std::string& getCurrent() const { return current; }
    const std::string& getHistory() const { return history; }
    bool getIsNewCalculation() const { return isNewCalculation; }
    const std::string& getAngleMode() const { return angleMode; }
};

#endif // CALCULATOR_H
