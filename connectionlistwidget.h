#ifndef CONNECTIONLISTWIDGET_H
#define CONNECTIONLISTWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QList>
#include "connectionwidget.h"

//class QVBoxLayout;


class ConnectionListWidget : public QWidget
{
    Q_OBJECT

public:
    ConnectionListWidget(QWidget *parent = 0);
    ~ConnectionListWidget();
    QList<ConnectionWidget*> connectionList;
    QStringList nameList;
    void sendData(quint8, QByteArray);
public slots:
        void newConnection(void);
signals:
    void connectionListChanged(QStringList);
    void dataReceived(quint8, QByteArray);

private:
    QVBoxLayout *mainLayout;
    QPushButton *newConnBtn;
    QHBoxLayout *topLayout;
    QLabel *bottomLabel;
    QLabel *widgetNameLabel;

    QScrollArea *scrollArea;
    QVBoxLayout *scrollAreaVLayout;
    QWidget *saWidgetContents;

    QString newConnectionName(void);
    void checkAllNames(void);
    void updateList(void);

private slots:
    void nameChanged(void);
    void connectionRemoved(void);
};

#endif // CONNECTIONWIDGET_H
