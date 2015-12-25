#ifndef LOGINDLG_H
#define LOGINDLG_H

#include "mainwindow.h"
#include <QDialog>

namespace Ui {
class LoginDlg;
}

typedef struct ErrorString {
    QString title;
    QString text;
} ErrStr;

class LoginDlg : public QDialog {
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

private slots:
    void on_LoginButton_clicked();
    void on_RegisterButton_clicked();
    void on_ExitButton_clicked();

private:
    void loginHandle(QString type);

    QMap<QString, ErrStr> err_map;
    QString username;
    QString password;

    MainWindow mw;

    Ui::LoginDlg *ui;
};

#endif // LOGINDLG_H
