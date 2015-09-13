#include "information.h"
#include "ui_information.h"

Information::Information(QWidget *parent) :
    QWidget(parent), ui(new Ui::Information) {
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

Information::~Information() {
    delete ui;
}

void Information::setInfo(QString name, QString win, QString lose, QString draw) {
    ui->Name->setText(name);
    ui->WinNum->setText(win);
    ui->LoseNum->setText(lose);
    ui->DrawNum->setText(draw);
}

void Information::on_ok_clicked() {
    this->hide();
}
