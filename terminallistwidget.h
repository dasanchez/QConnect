#ifndef TERMINALLISTWIDGET_H
#define TERMINALLISTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSplitter>
#include <QList>

#include "terminalwidget.h"
#include "livelistwidget.h"

class TerminalListWidget : public QWidget
{
    Q_OBJECT
public:
    TerminalListWidget(QWidget *parent = 0);
    ~TerminalListWidget();
    QList<TerminalWidget*> terminalList;
signals:
    void terminalRequest(TerminalWidget*,QString);
public slots:
    void updateConnections(QStringList);

private:
    QVBoxLayout *mainLayout;
    QPushButton *newTerminalButton;
    QHBoxLayout *topLayout;
    QLabel *bottomLabel;
    QLabel *widgetNameLabel;

    QSplitter *splitter;
    QScrollArea *scrollArea;
    QVBoxLayout *scrollAreaVLayout;
    QWidget *saWidgetContents;

    QStringList nameList;

private slots:
    void newTerminal(void);
    void terminalRemoved(void);
    void terminalConnectionRequest(QString);
};

#endif // TERMINALLISTWIDGET_H
