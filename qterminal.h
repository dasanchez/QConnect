#ifndef QTERMINAL_H
#define QTERMINAL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include "termconfigview.h"
#include "terminalarea.h"

class QTerminal : public QWidget
{
    Q_OBJECT
public:
    QTerminal(QWidget *parent = 0);
    TermConfigView *termConfig;

private:
    QVBoxLayout *mainLayout;

signals:
    
public slots:

};

#endif // QTERMINAL_H
