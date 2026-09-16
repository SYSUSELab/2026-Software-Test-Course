#include "MainWindow.h"
#include <QFile>
#include <QTimer>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    setObjectName("centralWidget");
    setWindowTitle("科学计算器");
    // 宽 410，高 800，完美呈现 Image 2 的宽适比例
    setFixedSize(410, 800);

    initUI();
    loadStyle();

    // 绑定显示更新
    calculator.setDisplayCallback([this](const std::string&, const std::string&) {
        updateDisplay();
    });

    updateDisplay();
}

void MainWindow::initUI() {
    // 对应 index.html 中的 body 内部居中容器
    auto *bodyLayout = new QVBoxLayout(this);
    bodyLayout->setContentsMargins(18, 18, 18, 18);

    // 对应 index.html 中的 <div class="calculator">
    calculatorContainer = new QFrame(this);
    calculatorContainer->setObjectName("calculator");
    auto *calcLayout = new QVBoxLayout(calculatorContainer);
    calcLayout->setContentsMargins(20, 20, 20, 20);
    calcLayout->setSpacing(16);

    // 对应 index.html 中的 <div class="display">
    displayContainer = new QFrame(calculatorContainer);
    displayContainer->setObjectName("display");
    displayContainer->setFixedHeight(105);
    auto *displayLayout = new QVBoxLayout(displayContainer);
    displayLayout->setContentsMargins(18, 14, 18, 14);
    displayLayout->setSpacing(2);

    // <div class="history" id="history"></div>
    history = new QLabel("", displayContainer);
    history->setObjectName("history");
    history->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    history->setWordWrap(true);

    // <div class="current" id="current">0</div>
    current = new QLabel("0", displayContainer);
    current->setObjectName("current");
    current->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    current->setWordWrap(true);

    displayLayout->addWidget(history);
    displayLayout->addWidget(current);
    calcLayout->addWidget(displayContainer);

    // 对应 index.html 中的 <div class="buttons">
    auto *buttonsGrid = new QGridLayout();
    buttonsGrid->setHorizontalSpacing(13);
    buttonsGrid->setVerticalSpacing(13);

    // 行 0: AC, CE, ⌫, ÷
    auto *btnAC = createButton("AC", "btn-function");
    connect(btnAC, &QPushButton::clicked, this, &MainWindow::clearAll);
    buttonsGrid->addWidget(btnAC, 0, 0);

    auto *btnCE = createButton("CE", "btn-function");
    connect(btnCE, &QPushButton::clicked, this, &MainWindow::clearEntry);
    buttonsGrid->addWidget(btnCE, 0, 1);

    auto *btnDel = createButton("⌫", "btn-function");
    connect(btnDel, &QPushButton::clicked, this, &MainWindow::deleteLast);
    buttonsGrid->addWidget(btnDel, 0, 2);

    auto *btnDiv = createButton("÷", "btn-operator");
    connect(btnDiv, &QPushButton::clicked, this, [this]() { inputOperator("/"); });
    buttonsGrid->addWidget(btnDiv, 0, 3);

    // 行 1: sin, cos, tan, ×
    auto *btnSin = createButton("sin", "btn-scientific");
    connect(btnSin, &QPushButton::clicked, this, [this]() { inputFunction("sin"); });
    buttonsGrid->addWidget(btnSin, 1, 0);

    auto *btnCos = createButton("cos", "btn-scientific");
    connect(btnCos, &QPushButton::clicked, this, [this]() { inputFunction("cos"); });
    buttonsGrid->addWidget(btnCos, 1, 1);

    auto *btnTan = createButton("tan", "btn-scientific");
    connect(btnTan, &QPushButton::clicked, this, [this]() { inputFunction("tan"); });
    buttonsGrid->addWidget(btnTan, 1, 2);

    auto *btnMul = createButton("×", "btn-operator");
    connect(btnMul, &QPushButton::clicked, this, [this]() { inputOperator("*"); });
    buttonsGrid->addWidget(btnMul, 1, 3);

    // 行 2: log, ln, √, -
    auto *btnLog = createButton("log", "btn-scientific");
    connect(btnLog, &QPushButton::clicked, this, [this]() { inputFunction("log"); });
    buttonsGrid->addWidget(btnLog, 2, 0);

    auto *btnLn = createButton("ln", "btn-scientific");
    connect(btnLn, &QPushButton::clicked, this, [this]() { inputFunction("ln"); });
    buttonsGrid->addWidget(btnLn, 2, 1);

    auto *btnSqrt = createButton("√", "btn-scientific");
    connect(btnSqrt, &QPushButton::clicked, this, [this]() { inputFunction("sqrt"); });
    buttonsGrid->addWidget(btnSqrt, 2, 2);

    auto *btnSub = createButton("-", "btn-operator");
    connect(btnSub, &QPushButton::clicked, this, [this]() { inputOperator("-"); });
    buttonsGrid->addWidget(btnSub, 2, 3);

    // 行 3: x^y, 7, 8, 9
    auto *btnPow = createButton("x^y", "btn-scientific");
    connect(btnPow, &QPushButton::clicked, this, [this]() { inputOperator("^"); });
    buttonsGrid->addWidget(btnPow, 3, 0);

    auto *btn7 = createButton("7", "btn-number");
    connect(btn7, &QPushButton::clicked, this, [this]() { inputNumber("7"); });
    buttonsGrid->addWidget(btn7, 3, 1);

    auto *btn8 = createButton("8", "btn-number");
    connect(btn8, &QPushButton::clicked, this, [this]() { inputNumber("8"); });
    buttonsGrid->addWidget(btn8, 3, 2);

    auto *btn9 = createButton("9", "btn-number");
    connect(btn9, &QPushButton::clicked, this, [this]() { inputNumber("9"); });
    buttonsGrid->addWidget(btn9, 3, 3);

    // 行 4: π, 4, 5, 6
    auto *btnPi = createButton("π", "btn-scientific");
    connect(btnPi, &QPushButton::clicked, this, [this]() { inputConstant("π"); });
    buttonsGrid->addWidget(btnPi, 4, 0);

    auto *btn4 = createButton("4", "btn-number");
    connect(btn4, &QPushButton::clicked, this, [this]() { inputNumber("4"); });
    buttonsGrid->addWidget(btn4, 4, 1);

    auto *btn5 = createButton("5", "btn-number");
    connect(btn5, &QPushButton::clicked, this, [this]() { inputNumber("5"); });
    buttonsGrid->addWidget(btn5, 4, 2);

    auto *btn6 = createButton("6", "btn-number");
    connect(btn6, &QPushButton::clicked, this, [this]() { inputNumber("6"); });
    buttonsGrid->addWidget(btn6, 4, 3);

    // 行 5: e, 1, 2, 3
    auto *btnE = createButton("e", "btn-scientific");
    connect(btnE, &QPushButton::clicked, this, [this]() { inputConstant("e"); });
    buttonsGrid->addWidget(btnE, 5, 0);

    auto *btn1 = createButton("1", "btn-number");
    connect(btn1, &QPushButton::clicked, this, [this]() { inputNumber("1"); });
    buttonsGrid->addWidget(btn1, 5, 1);

    auto *btn2 = createButton("2", "btn-number");
    connect(btn2, &QPushButton::clicked, this, [this]() { inputNumber("2"); });
    buttonsGrid->addWidget(btn2, 5, 2);

    auto *btn3 = createButton("3", "btn-number");
    connect(btn3, &QPushButton::clicked, this, [this]() { inputNumber("3"); });
    buttonsGrid->addWidget(btn3, 5, 3);

    // 行 6: (, ), 0, .
    auto *btnLeftParen = createButton("(", "btn-function");
    connect(btnLeftParen, &QPushButton::clicked, this, [this]() { inputOperator("("); });
    buttonsGrid->addWidget(btnLeftParen, 6, 0);

    auto *btnRightParen = createButton(")", "btn-function");
    connect(btnRightParen, &QPushButton::clicked, this, [this]() { inputOperator(")"); });
    buttonsGrid->addWidget(btnRightParen, 6, 1);

    auto *btn0 = createButton("0", "btn-number");
    connect(btn0, &QPushButton::clicked, this, [this]() { inputNumber("0"); });
    buttonsGrid->addWidget(btn0, 6, 2);

    auto *btnDot = createButton(".", "btn-number");
    connect(btnDot, &QPushButton::clicked, this, [this]() { inputNumber("."); });
    buttonsGrid->addWidget(btnDot, 6, 3);

    // 行 7: +, n!, |x|, =
    auto *btnAdd = createButton("+", "btn-operator");
    connect(btnAdd, &QPushButton::clicked, this, [this]() { inputOperator("+"); });
    buttonsGrid->addWidget(btnAdd, 7, 0);

    auto *btnFact = createButton("n!", "btn-scientific");
    connect(btnFact, &QPushButton::clicked, this, [this]() { inputFunction("factorial"); });
    buttonsGrid->addWidget(btnFact, 7, 1);

    auto *btnAbs = createButton("|x|", "btn-scientific");
    connect(btnAbs, &QPushButton::clicked, this, [this]() { inputFunction("abs"); });
    buttonsGrid->addWidget(btnAbs, 7, 2);

    auto *btnEq = createButton("=", "btn-equals");
    connect(btnEq, &QPushButton::clicked, this, &MainWindow::calculate);
    buttonsGrid->addWidget(btnEq, 7, 3);

    calcLayout->addLayout(buttonsGrid);

    // 对应 index.html 中的 <div class="mode-toggle">
    auto *modeLayout = new QHBoxLayout();
    modeLayout->setContentsMargins(0, 4, 4, 0);

    // <button class="btn btn-mode" onclick="toggleMode()">切换到弧度模式</button>
    btnMode = new QPushButton("切换到弧度模式", calculatorContainer);
    btnMode->setObjectName("btnMode");
    btnMode->setFixedHeight(46);
    btnMode->setCursor(Qt::PointingHandCursor);
    connect(btnMode, &QPushButton::clicked, this, &MainWindow::toggleMode);

    // <span class="mode-indicator" id="modeIndicator">角度模式</span>
    modeIndicator = new QLabel("角度模式", calculatorContainer);
    modeIndicator->setObjectName("modeIndicator");
    modeIndicator->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeLayout->addWidget(btnMode);
    modeLayout->addStretch();
    modeLayout->addWidget(modeIndicator);

    calcLayout->addLayout(modeLayout);
    bodyLayout->addWidget(calculatorContainer);

    setFocusPolicy(Qt::StrongFocus);
}

QPushButton* MainWindow::createButton(const QString &text, const QString &className) {
    auto *btn = new QPushButton(text, calculatorContainer);
    btn->setProperty("btnClass", className);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btn->setFixedHeight(52);
    return btn;
}

// 加载 QSS 样式表 (对应 style.css)
void MainWindow::loadStyle() {
    QFile qssFile("style.qss");
    if (qssFile.open(QFile::ReadOnly | QFile::Text)) {
        setStyleSheet(QString::fromUtf8(qssFile.readAll()));
        qssFile.close();
    } else {
        // 兜底默认样式
        setStyleSheet(R"(
            QWidget#centralWidget { background: #606fe8; font-family: 'Segoe UI', sans-serif; }
            QFrame#calculator { background: #ffffff; border-radius: 24px; }
            QFrame#display { background: #2c3748; border-radius: 16px; }
            QLabel#history { font-size: 14px; color: #a0aec0; }
            QLabel#current { font-size: 34px; font-weight: bold; color: #ffffff; }
            QLabel#current[error="true"] { color: #e74c3c; }
            QPushButton { border: none; border-radius: 12px; font-size: 16px; font-weight: bold; color: white; }
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
            QPushButton#btnMode { background: #e59838; color: white; padding: 10px 18px; border-radius: 10px; font-size: 14px; font-weight: bold; }
            QPushButton#btnMode:hover { background: #d4882b; }
            QLabel#modeIndicator { font-size: 15px; color: #2c3748; font-weight: bold; }
        )");
    }
}

// 对应 script.js 中的 updateDisplay()
void MainWindow::updateDisplay() {
    history->setText(QString::fromStdString(calculator.getHistory()));
    current->setText(QString::fromStdString(calculator.getCurrent()));

    if (calculator.getCurrent() == "错误") {
        current->setProperty("error", true);
        current->style()->unpolish(current);
        current->style()->polish(current);

        // 对应 script.js 中 setTimeout(() => this.clearAll(), 2000);
        QTimer::singleShot(2000, this, [this]() {
            if (calculator.getCurrent() == "错误") {
                clearAll();
            }
        });
    } else {
        current->setProperty("error", false);
        current->style()->unpolish(current);
        current->style()->polish(current);
    }
}

// 对应 script.js 中的 inputNumber(num)
void MainWindow::inputNumber(const QString &num) {
    calculator.inputNumber(num.toStdString());
}

// 对应 script.js 中的 inputOperator(operator)
void MainWindow::inputOperator(const QString &op) {
    calculator.inputOperator(op.toStdString());
}

// 对应 script.js 中的 inputFunction(func)
void MainWindow::inputFunction(const QString &func) {
    calculator.inputFunction(func.toStdString());
}

// 对应 script.js 中的 inputConstant(constant)
void MainWindow::inputConstant(const QString &constant) {
    calculator.inputConstant(constant.toStdString());
}

// 对应 script.js 中的 clearAll()
void MainWindow::clearAll() {
    calculator.clearAll();
}

// 对应 script.js 中的 clearEntry()
void MainWindow::clearEntry() {
    calculator.clearEntry();
}

// 对应 script.js 中的 deleteLast()
void MainWindow::deleteLast() {
    calculator.deleteLast();
}

// 对应 script.js 中的 calculate()
void MainWindow::calculate() {
    calculator.calculate();
}

// 对应 script.js 中的 toggleMode()
void MainWindow::toggleMode() {
    calculator.toggleMode();
    if (calculator.getAngleMode() == "rad") {
        btnMode->setText("切换到角度模式");
        modeIndicator->setText("弧度模式");
    } else {
        btnMode->setText("切换到弧度模式");
        modeIndicator->setText("角度模式");
    }
}

// 键盘事件处理 (对应 script.js 第 298~347 行 keydown 监听)
void MainWindow::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    QString text = event->text();

    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        inputNumber(QString::number(key - Qt::Key_0));
    } else if (text == "+") {
        inputOperator("+");
    } else if (text == "-") {
        inputOperator("-");
    } else if (text == "*") {
        inputOperator("*");
    } else if (text == "/") {
        inputOperator("/");
    } else if (text == ".") {
        inputNumber(".");
    } else if (text == "(") {
        inputOperator("(");
    } else if (text == ")") {
        inputOperator(")");
    } else if (key == Qt::Key_Equal || key == Qt::Key_Return || key == Qt::Key_Enter) {
        calculate();
    } else if (key == Qt::Key_Backspace) {
        deleteLast();
    } else if (key == Qt::Key_Escape) {
        clearAll();
    } else if (key == Qt::Key_Delete) {
        clearEntry();
    } else {
        QWidget::keyPressEvent(event);
    }
}
