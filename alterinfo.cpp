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
    if(nickName.length() < 4) {
        QMessageBox::information(this, QStringLiteral("修改失败!"), QStringLiteral("长度应在4-10位之间  "));
        return;
    }
    if(nickName.contains(" ")) {
        QMessageBox::information(this, QStringLiteral("修改失败!"), QStringLiteral("昵称不可以包含空格  "));
        return;
    }
    nickName = "/AlterName " + nickName;
    client->write(nickName.toUtf8());
    client->waitForBytesWritten();
}

void AlterInfo::on_alterPassWd_clicked() {
    passWord = ui->InputPassWd->text();
    if(passWord.length() < 6) {
        QMessageBox::information(this, QStringLiteral("修改失败!"), QStringLiteral("长度应在6-10位之间  "));
        return;
    }
    passWord = "/AlterPassword " + passWord;
    client->write(passWord.toUtf8());
    client->waitForBytesWritten();
}
