#ifndef CLIENTCXN_H
#define CLIENTCXN_H

class QTcpSocket;

#include <QDataStream>

class ClientCxn
{
    QTcpSocket *socket;
    QDataStream in;

public:
    ClientCxn(QTcpSocket *socket = Q_NULLPTR);
};

#endif // CLIENTCXN_H
