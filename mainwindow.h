#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "alterinfo.h"
#include "information.h"
#include <QMainWindow>
#include <QHostAddress>
#include <QMenu>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void Init(QString data);
    QTcpSocket *client;

private slots:
    void clientReadyRead();
    void on_UserList_customContextMenuRequested(const QPoint &pos);

    void alterInfoTriggered();
    void getInfoTriggered();
    void inviteTriggered();

    void on_BeginWithAI_clicked();
    void on_BeginWithOther_clicked();

    void on_DrawChess_clicked();
    void on_Surrender_clicked();
    void on_Retract_clicked();

private:
    bool eventFilter(QObject *obj, QEvent *event);
    void updateInfo();
    void gameBegin(bool ai, bool turnclient, int white, QString info);
    void gameEnd();

    QStringList mystatus;
    QStringList gamestatus;

    bool isAI;
    bool isWating;
    bool isGaming;
    bool isTurnClient;
    int isWhite;
    int point_x;
    int point_y;
    unsigned char chessBoard[15][15];

    Information information;
    AlterInfo alterInfo;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
