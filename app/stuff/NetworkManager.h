#ifndef NETWORK_H
#define NETWORK_H

#include <QWebSocket>
#include <QEventLoop>
#include <QObject>

#include "Request.h"

#include "Log.h"

class NetworkManager : public QObject
{
    Q_OBJECT
    
public:
    class SocketErrorException : public std::exception
    {
    public:
        SocketErrorException(const QString& errorMessage, NetworkManager* manager) :
            m_errorMessage(errorMessage), m_manager(manager)
        {}

        virtual ~SocketErrorException() noexcept {}

        const char* what() const noexcept override {
            return m_errorMessage.toLatin1().data();
        }

        NetworkManager* getManager() const { return m_manager; }

    private:
        QString m_errorMessage;
        NetworkManager* m_manager;
    };

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
