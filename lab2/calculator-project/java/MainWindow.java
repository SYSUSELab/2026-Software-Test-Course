import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * 科学计算器 Swing GUI 界面
 * 原样对应 javascript 版本的 index.html 与 style.css
 */
public class MainWindow extends JFrame {

    private final Calculator calculator;
    private JLabel historyLabel;
    private JLabel currentLabel;
    private JButton modeToggleBtn;
    private JLabel modeIndicatorLabel;

    public MainWindow() {
        super("科学计算器");
        this.calculator = new Calculator();

        initUI();

        // 绑定计算器显示更新回调
        calculator.setDisplayCallback((current, history) -> {
            SwingUtilities.invokeLater(() -> {
                historyLabel.setText(history.isEmpty() ? " " : history);
                currentLabel.setText(current);
                if ("错误".equals(current)) {
                    currentLabel.setForeground(new Color(0xe7, 0x4c, 0x3c));
                } else {
                    currentLabel.setForeground(Color.WHITE);
                }
            });
        });

        // 初始显示刷新
        calculator.updateDisplay();
    }

    private void initUI() {
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setResizable(false);

        // 背景渐变外层面板
        GradientPanel backgroundPanel = new GradientPanel();
        backgroundPanel.setLayout(new GridBagLayout());
        backgroundPanel.setBorder(new EmptyBorder(24, 24, 24, 24));

        // 白色圆角卡片容器 (对应 .calculator)
        RoundedPanel cardPanel = new RoundedPanel(24, Color.WHITE);
        cardPanel.setLayout(new BorderLayout(0, 16));
        cardPanel.setBorder(new EmptyBorder(20, 20, 20, 20));
        cardPanel.setPreferredSize(new Dimension(380, 580));

        // 1. 显示屏 (对应 .display)
        RoundedPanel displayPanel = new RoundedPanel(16, new Color(0x2d, 0x37, 0x48));
        displayPanel.setLayout(new BoxLayout(displayPanel, BoxLayout.Y_AXIS));
        displayPanel.setBorder(new EmptyBorder(14, 16, 14, 16));

        historyLabel = new JLabel(" ");
        historyLabel.setFont(new Font("Segoe UI", Font.PLAIN, 14));
        historyLabel.setForeground(new Color(0xa0, 0xae, 0xc0));
        historyLabel.setAlignmentX(Component.RIGHT_ALIGNMENT);
        historyLabel.setHorizontalAlignment(SwingConstants.RIGHT);

        currentLabel = new JLabel("0");
        currentLabel.setFont(new Font("Segoe UI", Font.BOLD, 30));
        currentLabel.setForeground(Color.WHITE);
        currentLabel.setAlignmentX(Component.RIGHT_ALIGNMENT);
        currentLabel.setHorizontalAlignment(SwingConstants.RIGHT);

        displayPanel.add(historyLabel);
        displayPanel.add(Box.createVerticalStrut(4));
        displayPanel.add(currentLabel);
        cardPanel.add(displayPanel, BorderLayout.NORTH);

        // 2. 按钮网格 (对应 .buttons，8 行 4 列)
        JPanel buttonsGrid = new JPanel(new GridLayout(8, 4, 8, 8));
        buttonsGrid.setOpaque(false);

        // 行 0: AC, CE, ⌫, ÷
        buttonsGrid.add(createButton("AC", ButtonType.FUNCTION, () -> calculator.clearAll()));
        buttonsGrid.add(createButton("CE", ButtonType.FUNCTION, () -> calculator.clearEntry()));
        buttonsGrid.add(createButton("⌫", ButtonType.FUNCTION, () -> calculator.deleteLast()));
        buttonsGrid.add(createButton("÷", ButtonType.OPERATOR, () -> calculator.inputOperator("/")));

        // 行 1: sin, cos, tan, ×
        buttonsGrid.add(createButton("sin", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("sin")));
        buttonsGrid.add(createButton("cos", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("cos")));
        buttonsGrid.add(createButton("tan", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("tan")));
        buttonsGrid.add(createButton("×", ButtonType.OPERATOR, () -> calculator.inputOperator("*")));

        // 行 2: log, ln, √, -
        buttonsGrid.add(createButton("log", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("log")));
        buttonsGrid.add(createButton("ln", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("ln")));
        buttonsGrid.add(createButton("√", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("sqrt")));
        buttonsGrid.add(createButton("-", ButtonType.OPERATOR, () -> calculator.inputOperator("-")));

        // 行 3: x^y, 7, 8, 9
        buttonsGrid.add(createButton("x^y", ButtonType.SCIENTIFIC, () -> calculator.inputOperator("^")));
        buttonsGrid.add(createButton("7", ButtonType.NUMBER, () -> calculator.inputNumber("7")));
        buttonsGrid.add(createButton("8", ButtonType.NUMBER, () -> calculator.inputNumber("8")));
        buttonsGrid.add(createButton("9", ButtonType.NUMBER, () -> calculator.inputNumber("9")));

        // 行 4: π, 4, 5, 6
        buttonsGrid.add(createButton("π", ButtonType.SCIENTIFIC, () -> calculator.inputConstant("π")));
        buttonsGrid.add(createButton("4", ButtonType.NUMBER, () -> calculator.inputNumber("4")));
        buttonsGrid.add(createButton("5", ButtonType.NUMBER, () -> calculator.inputNumber("5")));
        buttonsGrid.add(createButton("6", ButtonType.NUMBER, () -> calculator.inputNumber("6")));

        // 行 5: e, 1, 2, 3
        buttonsGrid.add(createButton("e", ButtonType.SCIENTIFIC, () -> calculator.inputConstant("e")));
        buttonsGrid.add(createButton("1", ButtonType.NUMBER, () -> calculator.inputNumber("1")));
        buttonsGrid.add(createButton("2", ButtonType.NUMBER, () -> calculator.inputNumber("2")));
        buttonsGrid.add(createButton("3", ButtonType.NUMBER, () -> calculator.inputNumber("3")));

        // 行 6: (, ), 0, .
        buttonsGrid.add(createButton("(", ButtonType.FUNCTION, () -> calculator.inputOperator("(")));
        buttonsGrid.add(createButton(")", ButtonType.FUNCTION, () -> calculator.inputOperator(")")));
        buttonsGrid.add(createButton("0", ButtonType.NUMBER, () -> calculator.inputNumber("0")));
        buttonsGrid.add(createButton(".", ButtonType.NUMBER, () -> calculator.inputNumber(".")));

        // 行 7: +, n!, |x|, =
        buttonsGrid.add(createButton("+", ButtonType.OPERATOR, () -> calculator.inputOperator("+")));
        buttonsGrid.add(createButton("n!", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("factorial")));
        buttonsGrid.add(createButton("|x|", ButtonType.SCIENTIFIC, () -> calculator.inputFunction("abs")));
        buttonsGrid.add(createButton("=", ButtonType.EQUALS, () -> calculator.calculate()));

        cardPanel.add(buttonsGrid, BorderLayout.CENTER);

        // 3. 底部模式切换栏 (对应 .mode-toggle)
        JPanel modePanel = new JPanel(new BorderLayout(10, 0));
        modePanel.setOpaque(false);

        modeToggleBtn = new StyledButton("切换到弧度模式", ButtonType.MODE);
        modeToggleBtn.setPreferredSize(new Dimension(140, 36));
        modeToggleBtn.addActionListener(e -> toggleAngleMode());

        modeIndicatorLabel = new JLabel("角度模式", SwingConstants.RIGHT);
        modeIndicatorLabel.setFont(new Font("Segoe UI", Font.BOLD, 13));
        modeIndicatorLabel.setForeground(new Color(0x71, 0x80, 0x96));

        modePanel.add(modeToggleBtn, BorderLayout.WEST);
        modePanel.add(modeIndicatorLabel, BorderLayout.CENTER);
        cardPanel.add(modePanel, BorderLayout.SOUTH);

        backgroundPanel.add(cardPanel);
        setContentPane(backgroundPanel);
        pack();
        setLocationRelativeTo(null);
    }

    private void toggleAngleMode() {
        calculator.toggleMode();
        if ("rad".equals(calculator.getAngleMode())) {
            modeToggleBtn.setText("切换到角度模式");
            modeIndicatorLabel.setText("弧度模式");
        } else {
            modeToggleBtn.setText("切换到弧度模式");
            modeIndicatorLabel.setText("角度模式");
        }
    }

    private JButton createButton(String text, ButtonType type, Runnable action) {
        StyledButton btn = new StyledButton(text, type);
        btn.addActionListener(e -> action.run());
        return btn;
    }

    // 按钮类型与配色枚举
    public enum ButtonType {
        NUMBER(new Color(0x4e, 0x8f, 0xd4), new Color(0x3f, 0x7e, 0xc1), new Color(0x35, 0x6f, 0xa8), Color.WHITE, 16),
        OPERATOR(new Color(0xd0, 0x53, 0x44), new Color(0xbd, 0x46, 0x38), new Color(0xa8, 0x3b, 0x2e), Color.WHITE, 18),
        FUNCTION(new Color(0x8f, 0xa1, 0xa4), new Color(0x7e, 0x8f, 0x92), new Color(0x6c, 0x7d, 0x80), Color.WHITE, 14),
        SCIENTIFIC(new Color(0x8e, 0x54, 0xa8), new Color(0x7d, 0x43, 0x97), new Color(0x6c, 0x35, 0x85), Color.WHITE, 13),
        EQUALS(new Color(0x48, 0xbb, 0x78), new Color(0x38, 0xa1, 0x69), new Color(0x2f, 0x85, 0x5a), Color.WHITE, 18),
        MODE(new Color(0xed, 0xf2, 0xf7), new Color(0xe2, 0xe8, 0xf0), new Color(0xcb, 0xd5, 0xe0), new Color(0x4a, 0x55, 0x68), 12);

        final Color normalColor;
        final Color hoverColor;
        final Color pressedColor;
        final Color textColor;
        final int fontSize;

        ButtonType(Color normal, Color hover, Color pressed, Color textColor, int fontSize) {
            this.normalColor = normal;
            this.hoverColor = hover;
            this.pressedColor = pressed;
            this.textColor = textColor;
            this.fontSize = fontSize;
        }
    }

    // 自定义现代圆角按钮组件
    public static class StyledButton extends JButton {
        private final ButtonType type;
        private boolean isHover = false;
        private boolean isPressed = false;

        public StyledButton(String text, ButtonType type) {
            super(text);
            this.type = type;
            setFont(new Font("Segoe UI", Font.BOLD, type.fontSize));
            setForeground(type.textColor);
            setFocusPainted(false);
            setBorderPainted(false);
            setContentAreaFilled(false);
            setCursor(new Cursor(Cursor.HAND_CURSOR));

            addMouseListener(new MouseAdapter() {
                @Override
                public void mouseEntered(MouseEvent e) {
                    isHover = true;
                    repaint();
                }

                @Override
                public void mouseExited(MouseEvent e) {
                    isHover = false;
                    isPressed = false;
                    repaint();
                }

                @Override
                public void mousePressed(MouseEvent e) {
                    isPressed = true;
                    repaint();
                }

                @Override
                public void mouseReleased(MouseEvent e) {
                    isPressed = false;
                    repaint();
                }
            });
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

            Color bgColor = isPressed ? type.pressedColor : (isHover ? type.hoverColor : type.normalColor);
            g2.setColor(bgColor);
            g2.fillRoundRect(0, 0, getWidth(), getHeight(), 12, 12);

            g2.dispose();
            super.paintComponent(g);
        }
    }

    // 圆角面板容器
    public static class RoundedPanel extends JPanel {
        private final int radius;
        private final Color bgColor;

        public RoundedPanel(int radius, Color bgColor) {
            this.radius = radius;
            this.bgColor = bgColor;
            setOpaque(false);
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setColor(bgColor);
            g2.fillRoundRect(0, 0, getWidth(), getHeight(), radius, radius);
            g2.dispose();
            super.paintComponent(g);
        }
    }

    // 渐变背景外层面板 (linear-gradient(135deg, #667eea 0%, #764ba2 100%))
    public static class GradientPanel extends JPanel {
        public GradientPanel() {
            setOpaque(true);
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            g2.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);
            GradientPaint gp = new GradientPaint(
                    0, 0, new Color(0x66, 0x7e, 0xea),
                    getWidth(), getHeight(), new Color(0x76, 0x4b, 0xa2)
            );
            g2.setPaint(gp);
            g2.fillRect(0, 0, getWidth(), getHeight());
            g2.dispose();
        }
    }
}
