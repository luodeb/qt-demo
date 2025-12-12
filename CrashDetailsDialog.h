#ifndef CRASHDETAILSDIALOG_H
#define CRASHDETAILSDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

class CrashDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit CrashDetailsDialog(const QString &crashInfo, QWidget *parent = nullptr);
    ~CrashDetailsDialog();

private:
    void setupUI();
    void setupStyles();

    QTextEdit *crashInfoText;
    QPushButton *closeButton;
    QPushButton *copyButton;
    QString crashInformation;
};

#endif // CRASHDETAILSDIALOG_H
