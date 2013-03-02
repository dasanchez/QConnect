#include "qconnect.h"

QConnect::QConnect(QWidget *parent)
    : QWidget(parent)
{
    connectionList = new ConnectionListWidget();
    terminalList  = new TerminalListWidget();
    hl01 = new QHBoxLayout;
    hl01->addWidget(connectionList);
    hl01->addWidget(terminalList);
    //  hl01->setAlignment(Qt::AlignLeft);
    this->setLayout(hl01);

    connect(connectionList,SIGNAL(connectionListChanged(QStringList)),terminalList,SLOT(updateConnections(QStringList)));
    connect(terminalList,SIGNAL(terminalRequest(TerminalWidget*,QString)),this,SLOT(handleTerminalRequest(TerminalWidget*,QString)));

    connectionList->newConnection();
    terminalList->terminalList.at(0)->assignConnection(connectionList->connectionList.at(0));
}

QConnect::~QConnect()
{

}

void QConnect::handleTerminalRequest(TerminalWidget *terminal,QString name)
{
    foreach(ConnectionWidget *connection,connectionList->connectionList)
    {
        if(connection->getName()==name)
        {
            terminal->assignConnection(connection);
        }
    }
}

