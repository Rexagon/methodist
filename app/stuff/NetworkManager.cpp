#include "NetworkManager.h"

#include <QDomDocument>

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
    //Log::write("T:", message.toStdWString());
    
    QDomDocument document("cmd");
    document.setContent(message);
    
    Log::write(document.toString(4).toLatin1().toStdString());
        
    QDomNodeList sqlXML = document.elementsByTagName("sql_xml");
    
    Log::write(sqlXML.at(0).nodeValue().size());
    
    if (sqlXML.size() != 0) {
        QDomDocument resultXML("XML");
        resultXML.setContent(sqlXML.at(0).nodeValue());
        
        Log::write(resultXML.toString(4).toLatin1().toStdString());
    }
}
