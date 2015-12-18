#include "logindlg.h"
#include "ui_logindlg.h"

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent), ui(new Ui::LoginDlg) {
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());

    err_map["/UsernameNotExist"] = {QStringLiteral("用户名不存在!"), QStringLiteral("请输入正确的用户名  ")};
    err_map["/PasswordError"] = {QStringLiteral("密码错误!"), QStringLiteral("请输入正确的密码  ")};
    err_map["/UserHasLogged"] = {QStringLiteral("用户已经登陆!"), QStringLiteral("不允许账号重复登陆  ")};
    err_map["/UsernameExist"] = {QStringLiteral("用户名已存在!"), QStringLiteral("请重新选择用户名  ")};
}

LoginDlg::~LoginDlg() {
    delete ui;
}

void LoginDlg::on_LoginButton_clicked() {
    loginHandle("/Login ");
}

void LoginDlg::on_RegisterButton_clicked() {
    loginHandle("/Register ");
}

void LoginDlg::on_ExitButton_clicked() {
    this->close();
}

void LoginDlg::loginHandle(QString s) {
    username = ui->UsernameEdit->text();
    password = ui->PasswordEdit->text();

    if(!stringTest(4, 10, username)) {
        QMessageBox::information(this, QStringLiteral("用户名无效!"), QStringLiteral("请输入长度在4-10位之间的字母与数字组合 "));
        return;
    }

    if(!stringTest(6, 10, password)) {
        QMessageBox::information(this, QStringLiteral("密码无效!"), QStringLiteral("请输入长度在6-10位之间的字母与数字组合 "));
        return;
    }

    QString data = s + username + " " + password;

    QSettings *configIniRead = new QSettings("config.ini", QSettings::IniFormat);

    QString ipResult = configIniRead->value("/ip/value").toString();
    int portResult = configIniRead->value("/port/value").toInt();

    delete configIniRead;

    mw.client->connectToHost(QHostAddress(ipResult), portResult);
    if(!mw.client->waitForConnected(3000)) {
        QMessageBox::information(this, QStringLiteral("无法连接至服务器!"), QStringLiteral("请检查您的网络配置  "));
        return;
    }

    mw.client->write(data.toUtf8());
    if(mw.client->waitForReadyRead()) {
        mw.client->read(4);
        data = mw.client->read(1024);
    }

    if(err_map.contains(data)) {
        QMessageBox::information(this, err_map[data].title, err_map[data].text);
        mw.client->disconnectFromHost();
        mw.client->waitForDisconnected();
        return;
    }

    this->close();
    mw.show();
    mw.Init(data);
}

bool LoginDlg::stringTest(int min, int max, QString s) {
    QString pattern = QString("^[a-z0-9A-Z]{%1,%2}$").arg(min).arg(max);
    QRegExp rx(pattern);

    return rx.exactMatch(s);
}
