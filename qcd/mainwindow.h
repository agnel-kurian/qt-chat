#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QVector>

#include "clientcxn.h"

namespace Ui {
class MainWindow;
}

class QNetworkSession;
class QTcpServer;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sessionOpened();
    void clientConnected();
    void authenticate();
    void messageReceived();
    void signOut();
    void handleError(QAbstractSocket::SocketError socketError);

private:
    Ui::MainWindow *ui;
    QNetworkSession *networkSession;
    QTcpServer *tcpServer;
    QLabel *statusLabel;
    QDataStream in;


    QMap<QTcpSocket*, ClientCxn*> clientCxns;

};

#endif // MAINWINDOW_H
