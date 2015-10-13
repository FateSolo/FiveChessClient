#include "logindlg.h"
#include "ui_logindlg.h"

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent), ui(new Ui::LoginDlg) {
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

LoginDlg::~LoginDlg() {
    delete ui;
}

void LoginDlg::on_LoginButton_clicked() {
    username = ui->UsernameEdit->text();
    password = ui->PasswordEdit->text();

    if(username.length() < 4) {
        QMessageBox::information(this, QStringLiteral("用户名无效!"), QStringLiteral("长度应在4-10位之间  "));
        return;
    }

    if(username.contains(" ")) {
        QMessageBox::information(this, QStringLiteral("用户名无效!"), QStringLiteral("用户名不可以包含空格  "));
        return;
    }

    if(password.length() < 6) {
        QMessageBox::information(this, QStringLiteral("密码无效!"), QStringLiteral("长度应在6-10位之间  "));
        return;
    }

    QString data = "/Login " + username + " " + password;

    mw.client->connectToHost(QHostAddress("192.168.132.140"), 7110);
    if(!mw.client->waitForConnected(3000)) {
        QMessageBox::information(this, QStringLiteral("无法连接至服务器!"), QStringLiteral("请检查您的网络配置  "));
        return;
    }

    mw.client->write(data.toUtf8());
    if(mw.client->waitForReadyRead()) {
        mw.client->read(4);
        data = mw.client->read(1024);
    }

    if(data == "/UsernameNotExist") {
        QMessageBox::information(this, QStringLiteral("用户名不存在!"), QStringLiteral("请重新输入正确的用户名  "));
        mw.client->disconnectFromHost();
        mw.client->waitForDisconnected();
        return;
    }

    if(data == "/PasswordError") {
        QMessageBox::information(this, QStringLiteral("密码错误!"), QStringLiteral("请重新输入正确的密码  "));
        mw.client->disconnectFromHost();
        mw.client->waitForDisconnected();
        return;
    }

    if(data == "/UserHasLogged") {
        QMessageBox::information(this, QStringLiteral("用户已经登陆!"), QStringLiteral("不允许账号重复登陆  "));
        mw.client->disconnectFromHost();
        mw.client->waitForDisconnected();
        return;
    }

    this->close();
    mw.show();
    mw.Init(data);
}

void LoginDlg::on_RegisterButton_clicked() {
    username = ui->UsernameEdit->text();
    password = ui->PasswordEdit->text();

    if(username.length() < 4) {
        QMessageBox::information(this, QStringLiteral("用户名无效!"), QStringLiteral("长度应在4-10位之间  "));
        return;
    }

    if(username.contains(" ")) {
        QMessageBox::information(this, QStringLiteral("用户名无效!"), QStringLiteral("用户名不可以包含空格  "));
        return;
    }

    if(password.length() < 6) {
        QMessageBox::information(this, QStringLiteral("密码无效!"), QStringLiteral("长度应在6-10位之间  "));
        return;
    }

    QString data = "/Register " + username + " " + password;

    mw.client->connectToHost(QHostAddress("192.168.132.140"), 7110);
    if(!mw.client->waitForConnected(3000)) {
        QMessageBox::information(this, QStringLiteral("无法连接至服务器!"), QStringLiteral("请检查您的网络配置  "));
        return;
    }

    mw.client->write(data.toUtf8());
    if(mw.client->waitForReadyRead()) {
        mw.client->read(4);
        data = mw.client->read(1024);
    }

    if(data == "/UsernameExist") {
        QMessageBox::information(this, QStringLiteral("用户名已存在!"), QStringLiteral("请重新选择用户名  "));
        mw.client->disconnectFromHost();
        mw.client->waitForDisconnected();
        return;
    }

    this->close();
    mw.show();
    mw.Init(data);
}

void LoginDlg::on_ExitButton_clicked() {
    this->close();
}
