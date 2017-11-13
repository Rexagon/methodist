#include "NetworkManager.h"

#include <iostream>

NetworkManager::NetworkManager(const QString& url, QObject* parent) :
    QObject(parent)
{
    connect(&m_socket, &QWebSocket::connected, this, [this]() {
       std::cout << "Connected" << std::endl;
    });
    
    connect(&m_socket, &QWebSocket::disconnected, this, [this]() {
        std::cout << "Disconnected" << std::endl;
    });
    
    m_socket.open(QUrl(url));
    std::cout << "Opening" << std::endl;
}

NetworkManager::~NetworkManager()
{
    m_socket.close();
}
