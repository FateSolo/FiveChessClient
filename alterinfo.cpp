#include "alterinfo.h"
#include "ui_alterinfo.h"

AlterInfo::AlterInfo(QWidget *parent) :
    QWidget(parent), ui(new Ui::AlterInfo) {
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

AlterInfo::~AlterInfo() {
    delete ui;
}

void AlterInfo::setInfo(QString userName, QTcpSocket *client) {
    this->userName = userName;
    this->client = client;
    ui->Username->setText(userName);
}

void AlterInfo::on_alterName_clicked() {
    nickName = ui->InputName->text();

    if(!StringUtil::stringTest(4, 10, nickName)) {
        QMessageBox::information(this, QStringLiteral("修改失败!"), QStringLiteral("请输入长度在4-10位之间的字母与数字组合 "));
        return;
    }

    nickName = "/AlterName " + nickName;
    client->write(nickName.toUtf8());
    client->waitForBytesWritten();
}

void AlterInfo::on_alterPassWd_clicked() {
    passWord = ui->InputPassWd->text();

    if(!StringUtil::stringTest(6, 10, passWord)) {
        QMessageBox::information(this, QStringLiteral("修改失败!"), QStringLiteral("请输入长度在6-10位之间的字母与数字组合 "));
        return;
    }

    passWord = "/AlterPassword " + passWord;
    client->write(passWord.toUtf8());
    client->waitForBytesWritten();
}
