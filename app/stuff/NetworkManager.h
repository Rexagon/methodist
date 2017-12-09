#ifndef NETWORK_H
#define NETWORK_H

#include <QWebSocket>
#include <QEventLoop>

#include <memory>
#include <map>

#include "Async.h"
#include "Request.h"
#include "Response.h"

class NetworkManager
{
public:
    class SocketErrorException : public std::exception
    {
    public:
        SocketErrorException(const QString& errorMessage) :
            m_errorMessage(errorMessage)
        {}

        virtual ~SocketErrorException() noexcept {}

        const char* what() const noexcept override {
            return m_errorMessage.toLatin1().data();
        }

    private:
        QString m_errorMessage;
    };

    static void init(const QString& url);
    static void close();
    
    static void send(const Request& request);
    static void send(const Request& request, std::function<void(const Response&)> f);
    
private:
    static void binaryMessageHandler(const QByteArray& message);
    static void textMessageHandler(const QString& message);
    
    static QEventLoop m_synchronizationLoop;
    static std::unique_ptr<QWebSocket> m_socket;
    
    static std::map<size_t, std::function<void(const Response&)>> m_responseHandlers;
};

#endif // NETWORK_H
