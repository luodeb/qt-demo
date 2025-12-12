#include "CrashDetailsDialog.h"
#include <QScreen>
#include <QApplication>
#include <QClipboard>

CrashDetailsDialog::CrashDetailsDialog(const QString &crashInfo, QWidget *parent)
    : QDialog(parent), crashInformation(crashInfo) {
    setWindowTitle("崩溃详情");
    setModal(false);
    resize(700, 500);
    
    setupUI();
    setupStyles();
    
    // 居中显示
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
}

CrashDetailsDialog::~CrashDetailsDialog() {
}

void CrashDetailsDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);
    
    // 标题
    QLabel *titleLabel = new QLabel("崩溃详细信息", this);
    titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(titleLabel);
    
    // 详情文本区域
    crashInfoText = new QTextEdit(this);
    crashInfoText->setObjectName("crashInfoText");
    crashInfoText->setReadOnly(true);
    crashInfoText->setPlainText(crashInformation.isEmpty() ? 
        "Segmentation fault (core dumped)\n"
        "Signal: SIGSEGV\n"
        "Address: 0x00000000\n"
        "Timestamp: 2025-12-12 14:23:45\n"
        "Module: kernel.sys\n"
        "Thread ID: 12345\n"
        "Process ID: 67890\n\n"
        "Stack trace:\n"
        "  #0  0x7fff8a2e4560 in function_a() at module_a.cpp:123\n"
        "  #1  0x7fff8a2e4780 in function_b() at module_b.cpp:456\n"
        "  #2  0x7fff8a2e4890 in function_c() at module_c.cpp:789\n"
        "  #3  0x7fff8a2e4a20 in main() at main.cpp:42\n\n"
        "Register dump:\n"
        "  RAX: 0x0000000000000000  RBX: 0x00007fff8a2e5000\n"
        "  RCX: 0x00007fff8a2e4560  RDX: 0x0000000000000001\n"
        "  RSI: 0x00007fff8a2e6000  RDI: 0x00007fff8a2e7000\n"
        "  RBP: 0x00007fff8a2e8000  RSP: 0x00007fff8a2e9000\n\n"
        "Memory map:\n"
        "  0x00400000-0x00600000 r-xp  /usr/bin/application\n"
        "  0x00800000-0x00900000 rw-p  [heap]\n"
        "  0x7fff8a000000-0x7fff8b000000 r-xp  /lib/x86_64-linux-gnu/libc.so.6"
        : crashInformation);
    mainLayout->addWidget(crashInfoText);
    
    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    copyButton = new QPushButton("复制到剪贴板", this);
    copyButton->setObjectName("copyButton");
    copyButton->setFixedHeight(40);
    copyButton->setCursor(Qt::PointingHandCursor);
    connect(copyButton, &QPushButton::clicked, this, [this]() {
        QApplication::clipboard()->setText(crashInfoText->toPlainText());
        copyButton->setText("已复制 ✓");
        QTimer::singleShot(2000, this, [this]() {
            copyButton->setText("复制到剪贴板");
        });
    });
    buttonLayout->addWidget(copyButton);
    
    buttonLayout->addStretch();
    
    closeButton = new QPushButton("关闭", this);
    closeButton->setObjectName("closeButton");
    closeButton->setFixedSize(100, 40);
    closeButton->setCursor(Qt::PointingHandCursor);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeButton);
    
    mainLayout->addLayout(buttonLayout);
}

void CrashDetailsDialog::setupStyles() {
    QString styleSheet = R"(
        QDialog {
            background-color: #ffffff;
        }
        
        #titleLabel {
            color: #2c3e50;
            font-size: 18px;
            font-weight: bold;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
            padding-bottom: 5px;
        }
        
        #crashInfoText {
            background-color: #f8fafc;
            border: 1px solid #dfe6ec;
            border-radius: 8px;
            padding: 15px;
            color: #34495e;
            font-family: "Consolas", "Courier New", monospace;
            font-size: 12px;
            selection-background-color: #3498db;
            selection-color: white;
            line-height: 1.5;
        }
        
        #copyButton {
            background-color: #ecf0f1;
            color: #5a6c7d;
            border: 1px solid #d5dbdf;
            border-radius: 8px;
            font-size: 13px;
            font-weight: 500;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
            padding: 8px 16px;
        }
        
        #copyButton:hover {
            background-color: #d5dbdf;
            border: 1px solid #bdc3c7;
        }
        
        #copyButton:pressed {
            background-color: #bdc3c7;
        }
        
        #closeButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3498db, stop:1 #2980b9);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 13px;
            font-weight: 600;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
            padding: 8px;
        }
        
        #closeButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5dade2, stop:1 #3498db);
        }
        
        #closeButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2980b9, stop:1 #21618c);
        }
    )";
    
    setStyleSheet(styleSheet);
}
