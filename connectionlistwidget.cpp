#include "connectionlistwidget.h"


ConnectionListWidget::ConnectionListWidget(QWidget *parent)
    : QWidget(parent)
{

    widgetNameLabel = new QLabel("Connection List");
    newConnBtn = new QPushButton("New Connection");

    topLayout = new QHBoxLayout;
    topLayout->addWidget(widgetNameLabel);
    topLayout->addWidget(newConnBtn);

    scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollAreaVLayout = new QVBoxLayout;
    saWidgetContents = new QWidget();

    saWidgetContents->setLayout(scrollAreaVLayout);
    scrollAreaVLayout->setSizeConstraint(QLayout::SetFixedSize);
    scrollArea->setWidget(saWidgetContents);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(scrollArea);

    this->setLayout(mainLayout);
    setFixedWidth(410);
    connect(newConnBtn,SIGNAL(clicked()),this,SLOT(newConnection()));
}

ConnectionListWidget::~ConnectionListWidget()
{

}

void ConnectionListWidget::newConnection()
{
    ConnectionWidget *connWidget = new ConnectionWidget;
    connWidget->setName(newConnectionName());
    connectionList.append(connWidget);
    scrollAreaVLayout->addWidget(connWidget);
    connect(connWidget,SIGNAL(nameChange()),this,SLOT(nameChanged()));
    connect(connWidget,SIGNAL(widgetRemoved()),this,SLOT(connectionRemoved()));
    updateList();
}

void ConnectionListWidget::updateList()
{
    nameList.clear();
    foreach(ConnectionWidget *connection,connectionList)
    {
        nameList.append(connection->getName());
    }
    emit connectionListChanged(nameList);
}

void ConnectionListWidget::connectionRemoved()
{
    ConnectionWidget* conn = qobject_cast<ConnectionWidget *>(QObject::sender());
    foreach(ConnectionWidget *widget,connectionList)
    {
        if(widget==conn)
        {
            connectionList.removeAt(connectionList.indexOf(widget));
        }
    }
    // Check the remaining connection names
    checkAllNames();
    updateList();
}

QString ConnectionListWidget::newConnectionName()
{
    QString newName;
    quint16 nameCounter=1;
    bool match;
    if(connectionList.size()>0)
    {
        while(nameCounter<999)
        {
            match=false;
            newName=QString("Connection %1").arg(nameCounter);
            foreach(ConnectionWidget *conn,connectionList)
            {
                if(newName==conn->getName())
                {
                    match=true;
                    nameCounter++;
                    break;
                }
            }
            if(!match)
            {
                return newName;
            }
        }
        return QString("");

    }
    return QString("Connection 1");
}

void ConnectionListWidget::nameChanged()
{
    ConnectionWidget* conn = qobject_cast<ConnectionWidget *>(QObject::sender());
    // Compare sender string against other connections
    quint16 index = connectionList.indexOf(conn);
    for(quint16 i=0;i<connectionList.size();i++)
    {
        if(i!=index)
        {
            if(conn->getName() == connectionList.at(i)->getName())
            {
                conn->setNameValid(false);
                return;
            }
        }
    }
    conn->setNameValid(true);

    checkAllNames();
    updateList();
}

void ConnectionListWidget::checkAllNames()
{
    bool valid;
    // Compare all connections to see if there are any we can validate
    for(quint16 i=0;i<connectionList.size();i++)
    {
        valid=true;
        if(!connectionList.at(i)->nameIsValid())
        {
            for(quint16 j=0;j<connectionList.size();j++)
            {
                if(j==i) break;
                if(connectionList.at(i)->getName() == connectionList.at(j)->getName())
                {
                    valid=false;
                }
            }
            connectionList.at(i)->setNameValid(valid);
        }
    }
}


