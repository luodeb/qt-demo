#ifndef CRASHDIALOG_H
#define CRASHDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QTimer>

class CrashDialog : public QDialog {
    Q_OBJECT

public:
    enum ResultType {
        Ignore = 100,
        Upload = 101
    };
    
    explicit CrashDialog(const QString &crashInfo = "", QWidget *parent = nullptr);
    ~CrashDialog();
    
    QString getResult() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void ignoreClicked();
    void uploadClicked();

private slots:
    void onIgnoreClicked();
    void onUploadClicked();
    void toggleDetails();

private:
    void setupUI();
    void setupStyles();
    void setupAnimations();
    void applyModernShadow(QWidget *widget);

    QLabel *titleLabel;
    QLabel *iconLabel;
    QLabel *descriptionLabel;
    QPushButton *ignoreButton;
    QPushButton *uploadButton;
    QPushButton *detailsButton;
    QWidget *centralWidget;
    
    QString crashInformation;
    QPropertyAnimation *fadeInAnimation;
    QPropertyAnimation *slideAnimation;
    QString result;
    
    // 拖动相关
    bool m_isDragging = false;
    QPoint m_dragPosition;
};

#endif // CRASHDIALOG_H
