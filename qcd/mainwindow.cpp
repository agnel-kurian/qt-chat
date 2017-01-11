#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtNetwork>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusLabel = new QLabel;
    ui->statusBar->addPermanentWidget(statusLabel);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &MainWindow::sessionOpened);

        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }

    connect(tcpServer, &QTcpServer::newConnection, this,
            &MainWindow::clientConnected);

}

void MainWindow::clientConnected(){
    QTcpSocket *clientCxn = tcpServer->nextPendingConnection();
    connect(clientCxn, &QAbstractSocket::disconnected,
            clientCxn, &QObject::deleteLater);

    connect(clientCxn, &QIODevice::readyRead, this, &MainWindow::authenticate);

    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(
                QAbstractSocket::SocketError);
    connect(clientCxn,
            static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, &MainWindow::handleError);
    connect(clientCxn, &QAbstractSocket::disconnected, this,
            &MainWindow::signOut);
}

void MainWindow::signOut(){
    QTcpSocket *clientCxn = dynamic_cast<QTcpSocket*>(sender());
    if(!clientCxn) return;

    //  remove clientCxn from list of clients
    int iclient = clientCxns.indexOf(clientCxn);
    Q_ASSERT(iclient != -1);
    if(iclient != -1){
        clientCxns.remove(iclient);
    }
}

void MainWindow::authenticate(){
    QTcpSocket *clientCxn = dynamic_cast<QTcpSocket*>(sender());
    if(!clientCxn) return;

    //  if credentials have not been read fully
    //      return

    //  read a login name and password

    //  authenticate
    //  write success/failure to client

    //  if authentication failed
    //      close clientCxn return

    //  disconnect authenticate from readReady signal
    disconnect(clientCxn, &QIODevice::readyRead, this,
               &MainWindow::authenticate);

    //  add clientCxn to list of connected clients

    //  connect messageReceived to readReady signal
    connect(clientCxn, &QIODevice::readyRead, this,
            &MainWindow::messageReceived);

}

void MainWindow::messageReceived(){
    QTcpSocket *clientCxn = dynamic_cast<QTcpSocket*>(sender());
    if(!clientCxn) return;

    //  loop through all clients except this one and send them the message
}

void MainWindow::handleError(QAbstractSocket::SocketError /*socketError*/){
    //socketError;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(
                        QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

//! [0] //! [1]
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }
//! [0]
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("Listening on IP: %1 Port: %2")
                         .arg(ipAddress).arg(tcpServer->serverPort()));
//! [1]
}
