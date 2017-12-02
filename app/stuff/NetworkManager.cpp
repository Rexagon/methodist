#include "NetworkManager.h"

#include "Log.h"

NetworkManager::NetworkManager(const QString& url, QObject* parent) :
    QObject(parent)
{
    connect(&m_socket, &QWebSocket::connected, this, [this]() {
        if (m_synchronizationLoop.isRunning()) {
            m_synchronizationLoop.exit();
        }
        Log::write("Successfully connected to server");
    });
    
    connect(&m_socket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError error)>(&QWebSocket::error), this, 
            [this](QAbstractSocket::SocketError error)
    {
        throw SocketErrorException(m_socket.errorString(), this);
    });
    
    connect(&m_socket, &QWebSocket::disconnected, this, [this]() {
        if (m_synchronizationLoop.isRunning()) {
            m_synchronizationLoop.exit();;
        }
        Log::write("Disconnected from server");
    });
    
    connect(&m_socket, &QWebSocket::binaryMessageReceived, this, &NetworkManager::binaryMessageHandler);
    
    connect(&m_socket, &QWebSocket::textMessageReceived, this, &NetworkManager::textMessageHandler);
    
    m_socket.open(QUrl(url));
    Log::write("Connecting to server...");
    
    m_synchronizationLoop.exec();
}

NetworkManager::~NetworkManager()
{
    m_socket.close();
}

void NetworkManager::send(const Request& request)
{
    Log::write("Send request to server. [id:", request.getTaskId(), "]");
    m_socket.sendTextMessage(request.getData());
}

void NetworkManager::send(const Request& request, std::function<void (const Response&)> f)
{
    Log::write("Send request to server. [id:", request.getTaskId(), "]");
    m_socket.sendTextMessage(request.getData());
    
    m_responseHandlers[request.getTaskId()] = f;
}

void NetworkManager::binaryMessageHandler(const QByteArray& message)
{
    //TODO: handle binary messages
}

void NetworkManager::textMessageHandler(const QString& message)
{
    Response response(message);
    Log::write("Got response from server. [id:", response.getTaskId(), "]");
    
    auto it = m_responseHandlers.find(response.getTaskId());
    if (it != m_responseHandlers.end()) {
        it->second(response);
        m_responseHandlers.erase(it);
    }
}
