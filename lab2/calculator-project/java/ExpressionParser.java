/**
 * 递归下降数学表达式解析器
 * 对应 script.js 中 Function('"use strict"; return (' + expr + ')')() 的替代方案
 * 支持：+  -  *  /  **（幂）、括号、负数、浮点数
 */
public class ExpressionParser {

    private final String src;
    private int pos;

    public ExpressionParser(String src) {
        this.src = src.trim();
        this.pos = 0;
    }

    public double parse() throws Exception {
        double val = parseExpression();
        skipWhitespace();
        if (pos < src.length()) {
            throw new Exception("Unexpected trailing character: " + src.charAt(pos));
        }
        return val;
    }

    private void skipWhitespace() {
        while (pos < src.length() && Character.isWhitespace(src.charAt(pos))) {
            pos++;
        }
    }

    // expression → addition
    private double parseExpression() throws Exception {
        return parseAddition();
    }

    // addition → multiplication (('+' | '-') multiplication)*
    private double parseAddition() throws Exception {
        double left = parseMultiplication();
        while (true) {
            skipWhitespace();
            if (pos < src.length() && src.charAt(pos) == '+') {
                pos++;
                left += parseMultiplication();
            } else if (pos < src.length() && src.charAt(pos) == '-'
                    && (pos + 1 >= src.length() || src.charAt(pos + 1) != '-')) {
                pos++;
                left -= parseMultiplication();
            } else {
                break;
            }
        }
        return left;
    }

    // multiplication → exponent (('*' | '/') exponent)*
    private double parseMultiplication() throws Exception {
        double left = parseExponent();
        while (true) {
            skipWhitespace();
            if (pos < src.length() && src.charAt(pos) == '*'
                    && (pos + 1 >= src.length() || src.charAt(pos + 1) != '*')) {
                pos++;
                left *= parseExponent();
            } else if (pos < src.length() && src.charAt(pos) == '/') {
                pos++;
                double right = parseExponent();
                left /= right;
            } else {
                break;
            }
        }
        return left;
    }

    // exponent → unary ('**' unary)*  (右结合)
    private double parseExponent() throws Exception {
        double base = parseUnary();
        skipWhitespace();
        if (pos + 1 < src.length() && src.charAt(pos) == '*' && src.charAt(pos + 1) == '*') {
            pos += 2;
            double exp = parseExponent(); // 右结合
            return Math.pow(base, exp);
        }
        return base;
    }

    // unary → '-' unary | '+' unary | primary
    private double parseUnary() throws Exception {
        skipWhitespace();
        if (pos < src.length() && src.charAt(pos) == '-') {
            pos++;
            return -parseUnary();
        }
        if (pos < src.length() && src.charAt(pos) == '+') {
            pos++;
            return parseUnary();
        }
        return parsePrimary();
    }

    // primary → '(' expression ')' | number
    private double parsePrimary() throws Exception {
        skipWhitespace();
        if (pos < src.length() && src.charAt(pos) == '(') {
            pos++; // skip '('
            double val = parseExpression();
            skipWhitespace();
            if (pos < src.length() && src.charAt(pos) == ')') {
                pos++;
            } else {
                throw new Exception("Expected ')'");
            }
            return val;
        }

        // number (supports optional scientific notation like 1e-5)
        int start = pos;
        if (pos < src.length() && (Character.isDigit(src.charAt(pos)) || src.charAt(pos) == '.')) {
            boolean hasDot = src.charAt(pos) == '.';
            pos++;
            while (pos < src.length()) {
                char ch = src.charAt(pos);
                if (Character.isDigit(ch)) {
                    pos++;
                } else if (ch == '.' && !hasDot) {
                    hasDot = true;
                    pos++;
                } else if ((ch == 'e' || ch == 'E') && pos > start) {
                    int expPos = pos;
                    pos++;
                    if (pos < src.length() && (src.charAt(pos) == '+' || src.charAt(pos) == '-')) {
                        pos++;
                    }
                    if (pos < src.length() && Character.isDigit(src.charAt(pos))) {
                        while (pos < src.length() && Character.isDigit(src.charAt(pos))) pos++;
                    } else {
                        pos = expPos;
                        break;
                    }
                } else {
                    break;
                }
            }
            return Double.parseDouble(src.substring(start, pos));
        }

        throw new Exception("Syntax error at pos " + pos + ": " + src.substring(pos));
    }
}
