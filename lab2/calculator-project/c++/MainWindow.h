#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include "Calculator.h"

/**
 * @brief Qt 科学计算器主窗口
 * 对应 index.html 的 DOM 结构与 script.js / style.css 的全部前端逻辑与样式
 */
class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

protected:
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    // 对应 script.js 中的全局函数与 HTML onclick 调用
    void inputNumber(const QString &num);
    void inputOperator(const QString &op);
    void inputFunction(const QString &func);
    void inputConstant(const QString &constant);
    void clearAll();
    void clearEntry();
    void deleteLast();
    void calculate();
    void toggleMode();
    void updateDisplay();

private:
    void initUI();
    void loadStyle();
    QPushButton* createButton(const QString &text, const QString &className);

    // 业务计算核心对象 (包含原脚本全部逻辑与未修复的 Bug)
    Calculator calculator;

    // DOM 元素对应的 Qt 控件 (与 index.html 中的 id 对应)
    QFrame *calculatorContainer;
    QFrame *displayContainer;
    QLabel *history;       // id="history"
    QLabel *current;       // id="current"
    QPushButton *btnMode;  // class="btn btn-mode"
    QLabel *modeIndicator; // id="modeIndicator"
};

#endif // MAINWINDOW_H
