#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QFrame>
#include <QPalette>
#include <QTimer>

#include "qdataconnection.h"
#include "terminalwidget.h"

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    ConnectionWidget(QWidget *parent = 0);
        ~ConnectionWidget();
    QString getName(void);
    void setName(QString);
    bool nameIsValid(void);

signals:
    void nameChange(void);
    void widgetRemoved(void);
    void dataRx(QByteArray);
    
public slots:
    void setNameValid(bool);
    void dataTx(QByteArray);
    
private:
    // Control
    QPushButton *removeButton;
    QLineEdit *nameEdit;
    QString connectionName;
    QPushButton *connectButton;
    QPushButton *viewButton;

    // Connection
    QDataConnection *dataConnection;
    QLabel *typeLabel;
    QLabel *addressLabel;
    QLabel *portLabel;
    QComboBox *typeComboBox;
    QLineEdit *addressEdit;
    QLineEdit *portEdit;
    QFrame *connFrame;
    bool validName;

    // Status
    QLabel *statusBar;
    QLabel *dataIcon;
    quint16 dataIconShade;
    QTimer *iconTimer;

    // Layout
    QHBoxLayout *topLayout;
    QHBoxLayout *bottomLayout;
    QGridLayout *controlLayout;
    QVBoxLayout *mainLayout;
    QFrame *frame;

private slots:
    void toggleView(void);
    void signalData(void);
    void animateDataIcon(void);
    void remove();
    void togglePropertyFields(bool);
        // Data connection
    void typeChanged(QString newType);
    void addressChanged(QString);
    void portChanged(QString);
    void toggleConnection(void);
    void connectionChanged(connectionState);
    void errorReceived(QString);
};

#endif // CONNECTIONWIDGET_H
