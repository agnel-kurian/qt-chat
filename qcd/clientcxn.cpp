#include "clientcxn.h"

#include <QTcpSocket>

ClientCxn::ClientCxn(QTcpSocket *socket) : socket(socket), in(socket)
{
    in.setVersion(QDataStream::Qt_5_7);
}
