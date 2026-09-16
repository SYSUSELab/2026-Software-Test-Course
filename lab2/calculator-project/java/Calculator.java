import java.util.function.BiConsumer;

/**
 * 科学计算器核心逻辑
 * 原样对应 javascript/script.js 中的 Calculator 类
 */
public class Calculator {

    private String current;
    private String history;
    private boolean isNewCalculation;
    private String angleMode; // "deg" 或 "rad"
    private BiConsumer<String, String> displayCallback; // callback(current, history)

    public Calculator() {
        this.current = "0";
        this.history = "";
        this.isNewCalculation = true;
        this.angleMode = "deg";
    }

    public void setDisplayCallback(BiConsumer<String, String> callback) {
        this.displayCallback = callback;
    }

    // 更新显示
    public void updateDisplay() {
        if (displayCallback != null) {
            displayCallback.accept(current, history);
        }
    }

    // 输入数字
    public void inputNumber(String num) {
        if (isNewCalculation) {
            current = num;
            isNewCalculation = false;
        } else {
            if (current.equals("0") && !num.equals(".")) {
                current = num;
            } else {
                current += num;
            }
        }
        updateDisplay();
    }

    // 输入运算符
    public void inputOperator(String operator) {
        if (isNewCalculation && !operator.equals("(") && !operator.equals(")")) {
            history = current + " " + operator + " ";
        } else {
            if (operator.equals("(")) {
                if (current.equals("0") || isNewCalculation) {
                    current = "(";
                } else {
                    current += "(";
                }
            } else if (operator.equals(")")) {
                current += ")";
            } else {
                current += " " + operator + " ";
            }
        }
        isNewCalculation = false;
        updateDisplay();
    }

    // 输入函数
    public void inputFunction(String func) {
        if (isNewCalculation) {
            current = func + "(";
        } else {
            current += func + "(";
        }
        isNewCalculation = false;
        updateDisplay();
    }

    // 输入常数
    public void inputConstant(String constant) {
        if (isNewCalculation) {
            current = constant;
            isNewCalculation = false;
        } else {
            if (current.equals("0")) {
                current = constant;
            } else {
                current += constant;
            }
        }
        updateDisplay();
    }

    // 清除所有 (AC)
    public void clearAll() {
        current = "0";
        history = "";
        isNewCalculation = true;
        updateDisplay();
    }

    // 清除当前输入 (CE)
    public void clearEntry() {
        current = "0";
        updateDisplay();
    }

    // 删除最后一个字符 (⌫)
    public void deleteLast() {
        if (current.length() > 1) {
            current = current.substring(0, current.offsetByCodePoints(current.length(), -1));
        } else {
            current = "0";
        }
        updateDisplay();
    }

    // 角度转弧度
    public double toRadians(double degrees) {
        return degrees * Math.PI / 180.0;
    }

    // 弧度转角度
    public double toDegrees(double radians) {
        return radians * 180.0 / Math.PI;
    }

    // 阶乘函数
    public double factorial(double n) {
        if (n < 0) return Double.NaN;
        long ni = Math.round(n);
        if (Math.abs(n - ni) > 1e-9) return Double.NaN;
        if (ni == 0 || ni == 1) return 1.0;
        double result = 1.0;
        for (long i = 2; i <= ni; i++) {
            result *= i;
        }
        return result;
    }

    // 执行计算
    public void calculate() {
        try {
            history = current;
            String expression = current;

            // 替换显示符号为运算符
            expression = expression.replace("×", "*");
            expression = expression.replace("÷", "/");

            double result = evaluateExpression(expression);

            if (Double.isNaN(result) || Double.isInfinite(result)) {
                throw new ArithmeticException("计算错误");
            }

            current = formatResult(result);
            isNewCalculation = true;
            updateDisplay();
        } catch (Exception e) {
            current = "错误";
            isNewCalculation = true;
            updateDisplay();
        }
    }

    // 处理数学函数并求值（递归处理最内层函数）
    public double evaluateExpression(String expr) throws Exception {
        // 替换常数
        expr = expr.replace("π", String.valueOf(Math.PI));
        expr = expr.replace("e", String.valueOf(Math.E));

        // 循环解析最内层函数
        expr = resolveInnerFunctions(expr);

        // 处理幂运算（^→**，用 ExpressionParser 处理）
        expr = expr.replace("^", "**");

        return new ExpressionParser(expr).parse();
    }

    /**
     * 使用正则循环替换最内层函数调用：fn(arg)
     * 对应 script.js 中的 evaluateFunction 正则替换逻辑
     */
    private String resolveInnerFunctions(String expr) throws Exception {
        String funcPattern = "(sin|cos|tan|log|ln|sqrt|factorial|abs)\\(([^()]+)\\)";
        java.util.regex.Pattern p = java.util.regex.Pattern.compile(funcPattern);

        int maxIter = 100;
        for (int i = 0; i < maxIter; i++) {
            java.util.regex.Matcher m = p.matcher(expr);
            if (!m.find()) break;

            String funcName = m.group(1);
            String argStr = m.group(2);

            // 递归求值参数
            double val = new ExpressionParser(argStr).parse();
            double res;

            switch (funcName) {
                case "sin":
                    res = Math.sin(angleMode.equals("deg") ? toRadians(val) : val);
                    break;
                case "cos":
                    res = Math.cos(angleMode.equals("deg") ? toRadians(val) : val);
                    break;
                case "tan":
                    res = Math.tan(angleMode.equals("deg") ? toRadians(val) : val);
                    break;
                case "log":
                    if (val <= 0) throw new ArithmeticException("计算错误");
                    res = Math.log10(val);
                    break;
                case "ln":
                    if (val <= 0) throw new ArithmeticException("计算错误");
                    res = Math.log(val);
                    break;
                case "sqrt":
                    if (val < 0) throw new ArithmeticException("计算错误");
                    res = Math.sqrt(val);
                    break;
                case "factorial":
                    res = factorial(val);
                    if (Double.isNaN(res)) throw new ArithmeticException("计算错误");
                    break;
                case "abs":
                    res = Math.abs(val);
                    break;
                default:
                    throw new ArithmeticException("计算错误");
            }

            expr = expr.substring(0, m.start()) + res + expr.substring(m.end());
        }
        return expr;
    }

    // 格式化结果（对应 script.js 中的 formatResult）
    public String formatResult(double result) {
        // 极大或极小数字使用科学记数法
        if (Math.abs(result) > 1e15 || (Math.abs(result) < 1e-10 && result != 0)) {
            return String.format("%.6e", result);
        }
        // 整数直接返回
        if (result == Math.floor(result) && !Double.isInfinite(result)) {
            return String.valueOf((long) result);
        }
        // 保留最多 10 位小数
        String s = String.valueOf(result);
        if (s.contains(".")) {
            int decimalLen = s.length() - s.indexOf('.') - 1;
            if (decimalLen > 10) {
                s = String.format("%.10f", result);
                s = s.replaceAll("0+$", "").replaceAll("\\.$", "");
            }
        }
        return s;
    }

    // 切换角度/弧度模式
    public void toggleMode() {
        angleMode = angleMode.equals("deg") ? "rad" : "deg";
    }

    // Getters
    public String getCurrent() { return current; }
    public String getHistory() { return history; }
    public String getAngleMode() { return angleMode; }
    public boolean isNewCalculation() { return isNewCalculation; }
}
