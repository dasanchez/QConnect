#ifndef QTERMINALEDIT_H
#define QTERMINALEDIT_H

#include <QWidget>
#include <QPlainTextEdit>

class QTerminalEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit QTerminalEdit(QWidget *parent = 0);
    void setHex(bool);
    void appendText(QString);
    void appendText(QString, bool);
    bool willEcho(void);
signals:
    void textEntered(QString,bool);
public slots:
    void setEcho(bool);
private:
    bool echoBytes;
    bool hexMode;
    void keyPressEvent(QKeyEvent *e);

private slots:

};

#endif // QTERMINALEDIT_H

