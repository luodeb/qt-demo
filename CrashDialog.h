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

signals:
    void ignoreClicked();
    void uploadClicked();

private slots:
    void onIgnoreClicked();
    void onUploadClicked();

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
    
private slots:
    void toggleDetails();
};

#endif // CRASHDIALOG_H
