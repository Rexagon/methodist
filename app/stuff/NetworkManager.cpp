#include "NetworkManager.h"

#include <QObject>

#include "Log.h"

std::unique_ptr<QWebSocket> NetworkManager::m_socket = nullptr;
std::unique_ptr<QEventLoop> NetworkManager::m_synchronizationLoop = nullptr;
std::map<size_t, std::function<void(const Response&)>> NetworkManager::m_responseHandlers;

void NetworkManager::init(const QString& url)
{
    m_socket = std::make_unique<QWebSocket>();
    m_synchronizationLoop = std::make_unique<QEventLoop>();
    
    QObject::connect(m_socket.get(), &QWebSocket::connected, []() {
        exitSynchronizationLoop();
        Log::write("Successfully connected to server");
    });
    
    QObject::connect(m_socket.get(), static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError error)>(&QWebSocket::error), 
            [](QAbstractSocket::SocketError error)
    {
        throw SocketErrorException(m_socket->errorString());
    });
    
    QObject::connect(m_socket.get(), &QWebSocket::disconnected, []() {
        exitSynchronizationLoop();
        Log::write("Disconnected from server");
    });
    
    QObject::connect(m_socket.get(), &QWebSocket::binaryMessageReceived, &NetworkManager::binaryMessageHandler);
    
    QObject::connect(m_socket.get(), &QWebSocket::textMessageReceived, &NetworkManager::textMessageHandler);
    
    m_socket->open(QUrl(url));
    Log::write("Connecting to server...");
    
    m_synchronizationLoop->exec();
}

void NetworkManager::close()
{
    Log::write("Closing connection...");
    m_socket->close();
    Log::write("Connection closed...");
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

void NetworkManager::exitSynchronizationLoop()
{
    if (m_synchronizationLoop != nullptr && m_synchronizationLoop->isRunning()) {
        m_synchronizationLoop->exit();
        m_synchronizationLoop.reset();
    }
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
