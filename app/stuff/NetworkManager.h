#ifndef NETWORK_H
#define NETWORK_H

#include <QWebSocket>
#include <QEventLoop>
#include <QObject>

#include "Request.h"

class NetworkManager : public QObject
{
    Q_OBJECT
    
public:
    NetworkManager(const QString& url, QObject* parent = nullptr);
    ~NetworkManager();
    
    void sendRequest(const Request& request);
    
private:
    void binaryMessageHandler(const QByteArray& message);
    void textMessageHandler(const QString& message);
    
    QWebSocket m_socket;
    QEventLoop m_synchronizationLoop;
};

#endif // NETWORK_H
