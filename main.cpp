#include <QApplication>
#include <QWidget>
#include <QLabel>
#include "CrashDialog.h"
#include <QDebug>
#include <QCommandLineParser>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    QApplication::setApplicationName("CrashReporter");
    QApplication::setApplicationVersion("1.0");
    
    // 创建命令行解析器
    QCommandLineParser parser;
    parser.setApplicationDescription("崩溃日志报告工具");
    parser.addHelpOption();
    parser.addVersionOption();
    
    // 添加 -s 选项：直接传递崩溃日志字符串
    QCommandLineOption stringOption(QStringList() << "s" << "string",
                                    "直接传递崩溃日志字符串",
                                    "crash-log");
    parser.addOption(stringOption);
    
    // 添加 -f 选项：从文件读取崩溃日志
    QCommandLineOption fileOption(QStringList() << "f" << "file",
                                  "从文件读取崩溃日志",
                                  "log-file");
    parser.addOption(fileOption);
    
    // 解析命令行参数
    parser.process(app);
    
    // 获取崩溃日志内容
    QString crashLog;
    
    if (parser.isSet(stringOption)) {
        // 从 -s 参数获取
        crashLog = parser.value(stringOption);
    } else if (parser.isSet(fileOption)) {
        // 从 -f 参数指定的文件读取
        QString filePath = parser.value(fileOption);
        QFile file(filePath);
        
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            crashLog = in.readAll();
            file.close();
        } else {
            qDebug() << "错误: 无法打开文件" << filePath;
            crashLog = "暂无日志信息\n\n错误: 无法读取日志文件 " + filePath;
        }
    } else {
        // 没有传递任何参数
        crashLog = "暂无日志信息";
    }
    
    // 创建并显示美观的崩溃对话框
    CrashDialog *crashDialog = new CrashDialog(crashLog);
    
    // 显示对话框
    int result = crashDialog->exec();
    
    // 获取并打印结果
    QString actionResult = crashDialog->getResult();
    qDebug() << actionResult;
    
    delete crashDialog;
    
    return 0;
}
