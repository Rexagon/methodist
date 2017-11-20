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
        Log::write(m_socket.errorString().toStdString());
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

void NetworkManager::sendRequest(const Request& request)
{
    Log::write(request.getData().toStdString());
    m_socket.sendTextMessage(request.getData());
}

void NetworkManager::binaryMessageHandler(const QByteArray& message)
{
    Log::write("B:", message.toStdString());
}

void NetworkManager::textMessageHandler(const QString& message)
{
    Log::write("T:", message.toStdString());
}
