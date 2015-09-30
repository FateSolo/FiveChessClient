#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->Input->setStyleSheet("border:1px; background:(0x00,0xff,0x00,0x00)");
    ui->UserList->setStyleSheet("border:1px; background:(0x00,0xff,0x00,0x00)");
    ui->textBrowser->setStyleSheet("border:1px; background:(0x00,0xff,0x00,0x00)");

    ui->label->installEventFilter(this);
    ui->Input->installEventFilter(this);

    setFixedSize(this->width(), this->height());

    client = new QTcpSocket(this);
    isGaming = false;
}

MainWindow::~MainWindow() {
    client->write("/Logout 1");
    client->waitForBytesWritten();

    client->disconnectFromHost();
    client->waitForDisconnected();

    delete client;
    delete ui;
}

void MainWindow::Init(QString data) {
    mystatus = data.split(" ");
    updateInfo();

    connect(client, SIGNAL(readyRead()), this, SLOT(clientReadyRead()));
    client->write("/GetUserList 1");
}

void MainWindow::clientReadyRead() {
    while(!client->atEnd()) {
        int length = client->read(4).toInt();
        QString data = client->read(length);

        QStringList msg = data.split(" ");
        int i = 1 * (msg[0] == "/UserInfo") + 2 * (msg[0] == "/NextStep") +
                3 * (msg[0] == "/AlterNameSuccess") + 4 * (msg[0] == "/AlterNameError") +
                5 * (msg[0] == "/InviteSuccess") + 6 * (msg[0] == "/InviteFailed") +
                7 * (msg[0] == "/Win") + 8 * (msg[0] == "/Lose") +
                9  * (msg[0] == "/Draw") + 10 * (msg[0] == "/NoDraw") +
                11 * (msg[0] == "/Retract") + 12 * (msg[0] == "/NoRetract") +
                13 * (msg[0] == "/AlterPasswordSuccess") + 14 * (msg[0] == "/Invited") +
                15 * (msg[0] == "/SendMsg") + 16 * (msg[0] == "/UpdateUserList") +
                17 * (msg[0] == "/IsDraw") + 18 * (msg[0] == "IsRetract");

        switch(i) {
        case 1:
            information.setInfo(msg[1], msg[2], msg[3], msg[4]);
            information.show();
            break;

        case 2:
            chessBoard[msg[2].toInt()][msg[1].toInt()] = 1 - isWhite;
            ui->label->update();
            isTurnClient = true;
            break;

        case 3:
            QMessageBox::information(&alterInfo, QStringLiteral("修改成功!"), QStringLiteral("您可以继续修改或关闭对话框  "));
            mystatus[2] = msg[1];
            updateInfo();
            break;

        case 4:
            QMessageBox::information(&alterInfo, QStringLiteral("修改失败!"), QStringLiteral("该昵称已存在, 请重新选择  "));
            break;

        case 5:
            QMessageBox::information(this, QStringLiteral("邀请成功"), QStringLiteral("对方同意了您的邀请  "));
            memset(chessBoard, 255, 225);

            isGaming = true;
            isTurnClient = true;
            isWhite = 0;
            break;

        case 6:
            if(msg[1] == "0") {
                QMessageBox::information(this, QStringLiteral("邀请失败"), QStringLiteral("对方正在游戏中  "));
            } else {
                QMessageBox::information(this, QStringLiteral("邀请失败"), QStringLiteral("对方拒绝了您的邀请  "));
            }
            break;

        case 7:
            QMessageBox::information(this, QStringLiteral("游戏结束"), QStringLiteral("恭喜你赢得了这局胜利!  "));
            mystatus[3] = QString::number(mystatus[3].toInt() + 1);
            updateInfo();

            isGaming = false;
            break;

        case 8:
            chessBoard[msg[2].toInt()][msg[1].toInt()] = 1 - isWhite;
            ui->label->update();

            QMessageBox::information(this, QStringLiteral("游戏结束"), QStringLiteral("你输了, 请继续努力!  "));
            mystatus[4] = QString::number(mystatus[4].toInt() + 1);
            updateInfo();

            isGaming = false;
            break;

        case 9:
            QMessageBox::information(this, QStringLiteral("游戏结束"), QStringLiteral("对方同意了您的和棋请求  "));
            mystatus[5] = QString::number(mystatus[5].toInt() + 1);
            updateInfo();

            isGaming = false;
            break;

        case 10:
            QMessageBox::information(this, QStringLiteral("和棋失败"), QStringLiteral("对方拒绝了您的和棋请求  "));
            isTurnClient = true;
            break;

        case 11:
            chessBoard[msg[2].toInt()][msg[1].toInt()] = 255;
            chessBoard[msg[4].toInt()][msg[3].toInt()] = 255;
            ui->label->update();

            QMessageBox::information(this, QStringLiteral("悔棋成功"), QStringLiteral("对方同意了您的悔棋请求  "));
            isTurnClient = true;
            break;

        case 12:
            QMessageBox::information(this, QStringLiteral("悔棋失败"), QStringLiteral("对方拒绝了您的悔棋请求  "));
            isTurnClient = true;
            break;

        case 13:
            QMessageBox::information(&alterInfo, QStringLiteral("修改成功!"), QStringLiteral("您可以继续修改或关闭对话框  "));
            mystatus[1] = msg[1];
            break;

        case 14:
            i = QMessageBox::information(this, QStringLiteral("邀战"), msg[1] + " " + QStringLiteral("邀请您进行双人对弈 "),
                    QStringLiteral("同意"), QStringLiteral("拒绝"));

            if(i) {
                QString data = "/InviteFailed " + msg[1];
                client->write(data.toUtf8());
            } else {
                QString data = "/InviteSuccess " + msg[1];
                client->write(data.toUtf8());
                memset(chessBoard, 255, 225);

                isGaming = true;
                isTurnClient = false;
                isWhite = 1;
            }
            client->waitForBytesWritten();
            break;

        case 15:
            data = "";
            for(i = 1; i < msg.size(); i++) {
                data += msg[i] + " ";
            }
            ui->textBrowser->append(data);
            break;

        case 16:
            ui->UserList->clear();
            msg.removeAt(0);
            ui->UserList->addItems(msg);
            break;

        case 17:
            i = QMessageBox::information(this, QStringLiteral("和棋"), QStringLiteral("对方请求和棋 "),
                    QStringLiteral("同意"), QStringLiteral("拒绝"));

            if(i) {
                QString data = "/NoDraw 0";
                client->write(data.toUtf8());
            } else {
                QString data = "/Draw 1";
                client->write(data.toUtf8());
                mystatus[5] = QString::number(mystatus[5].toInt() + 1);
                updateInfo();

                isGaming = false;
            }
            client->waitForBytesWritten();
            break;

        case 18:
            i = QMessageBox::information(this, QStringLiteral("悔棋"), QStringLiteral("对方请求悔棋 "),
                    QStringLiteral("同意"), QStringLiteral("拒绝"));

            if(i) {

            } else {

            }
            client->waitForBytesWritten();
            break;
        }
    }
}

void MainWindow::on_UserList_customContextMenuRequested(const QPoint &pos) {
    if(ui->UserList->itemAt(pos)) {
        QMenu *popMenu = new QMenu(ui->UserList);

        if(ui->UserList->itemAt(pos)->text() == mystatus[2]) {
            QAction *alterInfo = new QAction(QStringLiteral("修改我的信息"), ui->UserList);
            connect(alterInfo, SIGNAL(triggered()), this, SLOT(alterInfoTriggered()));

            popMenu->addAction(alterInfo);
            popMenu->exec(QCursor::pos());

            delete alterInfo;
        } else {
            QAction *getInfo = new QAction(QStringLiteral("查看详细信息"), ui->UserList);
            QAction *invite = new QAction(QStringLiteral("邀请对方下棋"), ui->UserList);

            connect(getInfo, SIGNAL(triggered()), this, SLOT(getInfoTriggered()));
            connect(invite, SIGNAL(triggered()), this, SLOT(inviteTriggered()));

            popMenu->addAction(getInfo);
            popMenu->addAction(invite);
            popMenu->exec(QCursor::pos());

            delete getInfo;
            delete invite;
        }
        delete popMenu;
    }
}

void MainWindow::alterInfoTriggered() {
    alterInfo.setInfo(mystatus[0], client);
    alterInfo.show();
}

void MainWindow::getInfoTriggered() {
    QString data = "/GetUserInfo " + ui->UserList->currentItem()->text();
    client->write(data.toUtf8());
    client->waitForBytesWritten();
}

void MainWindow::inviteTriggered() {
    if(isGaming) {
        QMessageBox::information(this, QStringLiteral("邀请失败"), QStringLiteral("您正在游戏中  "));
    } else {
        QString data = "/Invite " + ui->UserList->currentItem()->text();
        client->write(data.toUtf8());
        client->waitForBytesWritten();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(obj == ui->label && isGaming) {
        if(event->type() == QEvent::Paint) {
            QPainter painter(ui->label);
            QPixmap black, white;
            black.load(":/new/prefix1/src/Black.png");
            white.load(":/new/prefix1/src/White.png");

            for(int i = 0; i < 15; i++) {
                for(int j = 0; j < 15; j++) {
                    if(chessBoard[j][i] == 0) {
                        painter.drawPixmap(203 + i * 31, 40 + j * 31, 30, 30, black);
                    }
                    if(chessBoard[j][i] == 1) {
                        painter.drawPixmap(203 + i * 31, 40 + j * 31, 30, 30, white);
                    }
                }
            }
        } else if(event->type() == QEvent::MouseButtonPress) {
            point_x = ui->label->mapFromGlobal(QCursor::pos()).x();
            point_y = ui->label->mapFromGlobal(QCursor::pos()).y();
        } else if(event->type() == QEvent::MouseButtonRelease && isTurnClient) {
            int x = ui->label->mapFromGlobal(QCursor::pos()).x();
            int y = ui->label->mapFromGlobal(QCursor::pos()).y();

            if(qAbs(x - point_x) < 15 && qAbs(y - point_y) < 15) {
                int i = (x - 203) / 31;
                int j = (y - 40) / 31;

                if(i >= 0 && i <= 15 && j >= 0 && j <= 15 && chessBoard[j][i] == 255) {
                    QString data = "/NextStep " + QString::number(i) + " " + QString::number(j) +
                            " " + QString::number(isWhite);
                    chessBoard[j][i] = isWhite;
                    ui->label->update();

                    client->write(data.toUtf8());
                    client->waitForBytesWritten();
                    isTurnClient = false;
                }
            }
        }
    } else if(obj == ui->Input && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if(keyEvent->key() == Qt::Key_Return && (keyEvent->modifiers() & Qt::ControlModifier)) {
            if(ui->Input->toPlainText() == "") {
                QMessageBox::information(this, QStringLiteral("无法发送"), QStringLiteral("消息内容为空  "));
            } else {
                QString data = "/SendMsg " + mystatus[2] + ":" + ui->Input->toPlainText();
                ui->Input->setText("");

                client->write(data.toUtf8());
                client->waitForBytesWritten();
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::updateInfo() {
    QString data;
    data = mystatus[2] + QStringLiteral("\n") + mystatus[3] + QStringLiteral("胜 ")
            + mystatus[4] + QStringLiteral("负 ") + mystatus[5] + QStringLiteral("和");
    ui->MyStatus->setText(data);
}

void MainWindow::on_BeginWithAI_clicked() {
    if(isGaming) {
        QMessageBox::information(this, QStringLiteral("无法开始"), QStringLiteral("您正在游戏中  "));
    } else {
        client->write("/BeginWithAI 1");
        client->waitForBytesWritten();
        memset(chessBoard, 255, 225);

        isGaming = true;
        isTurnClient = false;
        isWhite = 1;
    }
}

void MainWindow::on_BeginWithOther_clicked() {
    if(isGaming) {
        QMessageBox::information(this, QStringLiteral("无法开始"), QStringLiteral("您正在游戏中  "));
    } else {

    }
}

void MainWindow::on_DrawChess_clicked() {
    if(isGaming) {
        if(isTurnClient) {
            client->write("/DrawChess 1");
            client->waitForBytesWritten();
            isTurnClient = false;
        } else {
            QMessageBox::information(this, QStringLiteral("和棋无效"), QStringLiteral("请先等待对方落子  "));
        }
    } else {
        QMessageBox::information(this, QStringLiteral("和棋无效"), QStringLiteral("请先进入游戏  "));
    }
}

void MainWindow::on_Surrender_clicked() {
    if(isGaming) {
        if(isTurnClient) {
            client->write("/Surrender 1");
            client->waitForBytesWritten();
            QMessageBox::information(this, QStringLiteral("游戏结束"), QStringLiteral("你输了, 请继续努力!  "));
            isGaming = false;
            mystatus[4] = QString::number(mystatus[4].toInt() + 1);
            updateInfo();
        } else {
            QMessageBox::information(this, QStringLiteral("认输无效"), QStringLiteral("请先等待对方落子  "));
        }
    } else {
        QMessageBox::information(this, QStringLiteral("认输无效"), QStringLiteral("请先进入游戏  "));
    }
}

void MainWindow::on_Retract_clicked() {
    if(isGaming) {
        if(isTurnClient) {
            client->write("/Retract 1");
            client->waitForBytesWritten();
            isTurnClient = false;
        } else {
            QMessageBox::information(this, QStringLiteral("悔棋无效"), QStringLiteral("请先等待对方落子  "));
        }
    } else {
        QMessageBox::information(this, QStringLiteral("悔棋无效"), QStringLiteral("请先进入游戏  "));
    }
}
