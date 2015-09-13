#ifndef ALTERINFO_H
#define ALTERINFO_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>

namespace Ui {
class AlterInfo;
}

class AlterInfo : public QWidget {
    Q_OBJECT

public:
    explicit AlterInfo(QWidget *parent = 0);
    ~AlterInfo();

    void setInfo(QString uesrName, QTcpSocket *client);

private slots:
    void on_alterName_clicked();
    void on_alterPassWd_clicked();

private:
    QTcpSocket *client;
    QString userName;
    QString nickName;
    QString passWord;
    Ui::AlterInfo *ui;
};

#endif // ALTERINFO_H
