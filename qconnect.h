#ifndef QCONNECT_H
#define QCONNECT_H

#include <QtGui>
#include "connectionlistwidget.h"
#include "terminallistwidget.h"

class QConnect : public QWidget
{
    Q_OBJECT

public:
    QConnect(QWidget *parent = 0);
    ~QConnect();

signals:

private:
    ConnectionListWidget *connectionList;
    TerminalListWidget  *terminalList;
    QHBoxLayout *hl01;

private slots:
    void handleTerminalRequest(TerminalWidget*,QString);

};

#endif // QCONNECT_H
