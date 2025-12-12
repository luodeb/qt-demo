#include "CrashDialog.h"
#include "CrashDetailsDialog.h"
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QEasingCurve>
#include <QScreen>
#include <QApplication>
#include <QClipboard>

CrashDialog::CrashDialog(const QString &crashInfo, QWidget *parent)
    : QDialog(parent), crashInformation(crashInfo) {
    setWindowTitle("应用程序崩溃报告");
    setModal(true);
    setFixedSize(550, 280);
    
    // 移除默认边框，创建自定义外观
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setupUI();
    setupStyles();
    setupAnimations();
    
    // 居中显示
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
}

CrashDialog::~CrashDialog() {
}

void CrashDialog::setupUI() {
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 创建中心容器（带圆角和阴影）
    centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    
    QVBoxLayout *contentLayout = new QVBoxLayout(centralWidget);
    contentLayout->setContentsMargins(40, 35, 40, 35);
    contentLayout->setSpacing(20);
    
    // 顶部区域：图标和标题
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(20);
    
    // 崩溃图标
    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(64, 64);
    iconLabel->setText("⚠️");
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setStyleSheet("font-size: 48px;");
    headerLayout->addWidget(iconLabel);
    
    // 标题和副标题
    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(5);
    
    titleLabel = new QLabel("检测到内核崩溃日志", this);
    titleLabel->setObjectName("titleLabel");
    titleLayout->addWidget(titleLabel);
    
    // 描述文字和显示详情按钮在同一行
    QHBoxLayout *descLayout = new QHBoxLayout();
    descLayout->setSpacing(0);
    
    descriptionLabel = new QLabel("我们检测到一个内核崩溃日志，您可以选择忽略或上传崩溃报告帮助我们改进", this);
    descriptionLabel->setObjectName("descriptionLabel");
    descriptionLabel->setWordWrap(true);
    descLayout->addWidget(descriptionLabel);
    
    // 不显眼的"显示详情"链接按钮
    detailsButton = new QPushButton("显示详情", this);
    detailsButton->setObjectName("detailsButton");
    detailsButton->setCursor(Qt::PointingHandCursor);
    detailsButton->setFlat(true);
    connect(detailsButton, &QPushButton::clicked, this, &CrashDialog::toggleDetails);
    descLayout->addWidget(detailsButton);
    descLayout->addStretch();
    
    titleLayout->addLayout(descLayout);
    
    headerLayout->addLayout(titleLayout, 1);
    contentLayout->addLayout(headerLayout);
    
    // 添加弹性空间
    contentLayout->addStretch();
    
    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    buttonLayout->addStretch();
    
    // 忽略按钮
    ignoreButton = new QPushButton("忽略", this);
    ignoreButton->setObjectName("ignoreButton");
    ignoreButton->setFixedSize(140, 45);
    ignoreButton->setCursor(Qt::PointingHandCursor);
    connect(ignoreButton, &QPushButton::clicked, this, &CrashDialog::onIgnoreClicked);
    buttonLayout->addWidget(ignoreButton);
    
    // 上传按钮
    uploadButton = new QPushButton("上传报告", this);
    uploadButton->setObjectName("uploadButton");
    uploadButton->setFixedSize(140, 45);
    uploadButton->setCursor(Qt::PointingHandCursor);
    connect(uploadButton, &QPushButton::clicked, this, &CrashDialog::onUploadClicked);
    buttonLayout->addWidget(uploadButton);
    
    contentLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(centralWidget);
}

void CrashDialog::setupStyles() {
    QString styleSheet = R"(
        #centralWidget {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff, stop:1 #f5f7fa);
            border-radius: 16px;
            border: 2px solid #e1e8ed;
        }
        
        QDialog {
            background: transparent;
        }
        
        #titleLabel {
            color: #2c3e50;
            font-size: 24px;
            font-weight: bold;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
        }
        
        #descriptionLabel {
            color: #657786;
            font-size: 13px;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
        }
        
        #infoLabel {
            color: #2c3e50;
            font-size: 14px;
            font-weight: 600;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
        }
        
        #divider {
            color: #e1e8ed;
            background-color: #e1e8ed;
            max-height: 1px;
            border: none;
        }
        
        #detailsButton {
            color: #95a5a6;
            font-size: 12px;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
            padding: 3px 8px;
            margin-left: 5px;
            border: none;
            background: transparent;
            text-decoration: none;
        }
        
        #detailsButton:hover {
            color: #3498db;
            text-decoration: underline;
        }
        
        #ignoreButton {
            background-color: #ecf0f1;
            color: #5a6c7d;
            border: 1px solid #d5dbdf;
            border-radius: 10px;
            font-size: 14px;
            font-weight: 600;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
            padding: 8px;
        }
        
        #ignoreButton:hover {
            background-color: #d5dbdf;
            border: 1px solid #bdc3c7;
        }
        
        #ignoreButton:pressed {
            background-color: #bdc3c7;
        }
        
        #uploadButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3498db, stop:1 #2980b9);
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 14px;
            font-weight: 600;
            font-family: "Segoe UI", "Microsoft YaHei", Arial;
            padding: 8px;
        }
        
        #uploadButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #5dade2, stop:1 #3498db);
        }
        
        #uploadButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2980b9, stop:1 #21618c);
        }
    )";
    
    setStyleSheet(styleSheet);
}

void CrashDialog::setupAnimations() {
    // 使用窗口透明度动画替代图形效果
    setWindowOpacity(0.0);
    
    fadeInAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    fadeInAnimation->setDuration(400);
    fadeInAnimation->setStartValue(0.0);
    fadeInAnimation->setEndValue(1.0);
    fadeInAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // 延迟启动动画
    QTimer::singleShot(50, this, [this]() {
        fadeInAnimation->start();
    });
}

void CrashDialog::applyModernShadow(QWidget *widget) {
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(30);
    shadow->setXOffset(0);
    shadow->setYOffset(8);
    shadow->setColor(QColor(0, 0, 0, 60));
    widget->setGraphicsEffect(shadow);
}

void CrashDialog::toggleDetails() {
    // 弹出详情窗口
    CrashDetailsDialog *detailsDialog = new CrashDetailsDialog(crashInformation, this);
    detailsDialog->exec();
    delete detailsDialog;
}

QString CrashDialog::getResult() const {
    return result;
}

void CrashDialog::onIgnoreClicked() {
    result = "IGNORE";
    
    // 添加按钮点击动画
    QPropertyAnimation *scaleAnim = new QPropertyAnimation(this, "windowOpacity", this);
    scaleAnim->setDuration(200);
    scaleAnim->setStartValue(1.0);
    scaleAnim->setEndValue(0.0);
    scaleAnim->setEasingCurve(QEasingCurve::InCubic);
    
    connect(scaleAnim, &QPropertyAnimation::finished, this, [this]() {
        emit ignoreClicked();
        done(ResultType::Ignore);
    });
    
    scaleAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void CrashDialog::onUploadClicked() {
    result = "UPLOAD";
    
    // 添加按钮点击动画
    QPropertyAnimation *scaleAnim = new QPropertyAnimation(this, "windowOpacity", this);
    scaleAnim->setDuration(200);
    scaleAnim->setStartValue(1.0);
    scaleAnim->setEndValue(0.0);
    scaleAnim->setEasingCurve(QEasingCurve::InCubic);
    
    connect(scaleAnim, &QPropertyAnimation::finished, this, [this]() {
        emit uploadClicked();
        done(ResultType::Upload);
    });
    
    scaleAnim->start(QAbstractAnimation::DeleteWhenStopped);
}
