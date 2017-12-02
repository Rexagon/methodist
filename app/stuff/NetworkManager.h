#ifndef NETWORK_H
#define NETWORK_H

#include <QWebSocket>
#include <QEventLoop>
#include <QObject>

#include <map>

#include "Request.h"
#include "Response.h"

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
    
    void send(const Request& request);
    void send(const Request& request, std::function<void(const Response&)> f);
    
private:
    void binaryMessageHandler(const QByteArray& message);
    void textMessageHandler(const QString& message);
    
    QWebSocket m_socket;
    QEventLoop m_synchronizationLoop;
    
    std::map<size_t, std::function<void(const Response&)>> m_responseHandlers;
};

#endif // NETWORK_H
