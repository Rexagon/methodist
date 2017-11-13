#ifndef NETWORK_H
#define NETWORK_H

#include <QWebSocket>
#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT
    
public:
    NetworkManager(const QString& url, QObject* parent = nullptr);
    ~NetworkManager();
    
private:    
    QWebSocket m_socket;
};

#endif // NETWORK_H
