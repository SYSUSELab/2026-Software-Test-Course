import javax.swing.*;

/**
 * 科学计算器程序入口
 */
public class Main {
    public static void main(String[] args) {
        // 设置全局异常捕获，避免静默失败
        Thread.setDefaultUncaughtExceptionHandler((thread, throwable) -> {
            throwable.printStackTrace();
            JOptionPane.showMessageDialog(
                    null,
                    "计算器运行异常:\n" + throwable.getMessage(),
                    "错误",
                    JOptionPane.ERROR_MESSAGE
            );
        });

        // 尝试设置系统原生外观风格
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception ignored) {
        }

        // 在 Swing 事件分发线程中创建并显示主界面
        SwingUtilities.invokeLater(() -> {
            try {
                MainWindow window = new MainWindow();
                window.setVisible(true);
            } catch (Throwable t) {
                t.printStackTrace();
                JOptionPane.showMessageDialog(
                        null,
                        "计算器界面初始化失败:\n" + t.getMessage(),
                        "启动错误",
                        JOptionPane.ERROR_MESSAGE
                );
            }
        });
    }
}
