#include "NetworkManager.h"

#include <QObject>

#include "Log.h"

QEventLoop NetworkManager::m_synchronizationLoop;
std::unique_ptr<QWebSocket> NetworkManager::m_socket = nullptr;
std::map<size_t, std::function<void(const Response&)>> NetworkManager::m_responseHandlers;

void NetworkManager::init(const QString& url)
{
    m_socket = std::make_unique<QWebSocket>();
    
    QObject::connect(m_socket.get(), &QWebSocket::connected, []() {
        if (m_synchronizationLoop.isRunning()) {
            m_synchronizationLoop.exit();
        }
        Log::write("Successfully connected to server");
    });
    
    QObject::connect(m_socket.get(), static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError error)>(&QWebSocket::error), 
            [](QAbstractSocket::SocketError error)
    {
        throw SocketErrorException(m_socket->errorString());
    });
    
    QObject::connect(m_socket.get(), &QWebSocket::disconnected, []() {
        if (m_synchronizationLoop.isRunning()) {
            m_synchronizationLoop.exit();
        }
        Log::write("Disconnected from server");
    });
    
    QObject::connect(m_socket.get(), &QWebSocket::binaryMessageReceived, &NetworkManager::binaryMessageHandler);
    
    QObject::connect(m_socket.get(), &QWebSocket::textMessageReceived, &NetworkManager::textMessageHandler);
    
    m_socket->open(QUrl(url));
    Log::write("Connecting to server...");
    
    m_synchronizationLoop.exec();
}

void NetworkManager::close()
{
    Log::write("Closing connection...");
    m_socket->close();
    Log::write("Connection closed...");
    m_socket.reset();
}

void NetworkManager::send(const Request& request)
{
    Log::write("Send request to server [id:", request.getTaskId(), "]");
    m_socket->sendTextMessage(request.getData());
}

void NetworkManager::send(const Request& request, std::function<void (const Response&)> f)
{
    Log::write("Send request to server [id:", request.getTaskId(), "]");
    m_socket->sendTextMessage(request.getData());
    
    m_responseHandlers[request.getTaskId()] = f;
}

QString NetworkManager::escape(const QString& string)
{    
    QString result = string;
    return result
            //.replace("&", "&amp;")
            .replace("<", "&lt;")
            .replace(">", "&gt;")
            .replace("\"", "&quot;")
            .replace("'", "&#39;")
            .replace("/", "&&#x2F;")
            .replace("`", "&#x60;")
            .replace("=", "&#x3D;");
}

void NetworkManager::binaryMessageHandler(const QByteArray& message)
{
    //TODO: handle binary messages
}

void NetworkManager::textMessageHandler(const QString& message)
{
    //qDebug() << message;
    
    Response response(message);
    Log::write("Got response from server. [id:", response.getTaskId(), "]");
    
    auto it = m_responseHandlers.find(response.getTaskId());
    if (it != m_responseHandlers.end()) {
        it->second(response);
        m_responseHandlers.erase(it);
    }
}
