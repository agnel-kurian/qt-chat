#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    QNetworkSession *networkSession;
    QTcpServer *tcpServer;
    QLabel *statusLabel;

};

#endif // MAINWINDOW_H
