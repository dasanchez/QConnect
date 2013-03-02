#include "connectionwidget.h"

ConnectionWidget::ConnectionWidget(QWidget *parent) :
    QWidget(parent)
{
    // Control
    removeButton = new QPushButton("X");
    removeButton->setFixedWidth(40);
    removeButton->setToolTip("Remove connection");
    nameEdit = new QLineEdit("Connection 001");
    nameEdit->setToolTip("Connection name");
    nameEdit->setMinimumWidth(200);
    connectButton = new QPushButton("Connect");
    connectButton->setFixedWidth(60);
    viewButton = new QPushButton("Less");
    viewButton->setFixedWidth(40);
    viewButton->setToolTip("Hide or show connection properties");

    // Connection
    dataConnection = new QDataConnection;
    typeLabel = new QLabel("Type");
    typeLabel->setFixedWidth(80);
    addressLabel = new QLabel("IP address");
    addressLabel->setFixedWidth(80);
    portLabel = new QLabel("Port");
    portLabel->setFixedWidth(80);
    typeComboBox = new QComboBox;
    typeComboBox->addItem("TCP");
    typeComboBox->addItem("UDP");
    typeComboBox->addItem("Serial");
    typeComboBox->setCurrentIndex(0);
    typeComboBox->setFixedWidth(120);
    addressEdit = new QLineEdit("127.0.0.1");
    addressEdit->setFixedWidth(120);
    portEdit = new QLineEdit("2222");
    portEdit->setFixedWidth(120);

    // Status
    statusBar = new QLabel("Click the Connect button");
    dataIcon = new QLabel("I/O");
    dataIcon->setAlignment(Qt::AlignHCenter);
    dataIcon->setFixedWidth(40);
    dataIconShade = 255;
    QString styleString = QString("QLabel{background-color: rgb(%1,%1,%1)}").arg(dataIconShade);
    dataIcon->setStyleSheet(styleString);
    iconTimer = new QTimer;
    iconTimer->setInterval(50);
    validName = true;

    // Layout
    topLayout = new QHBoxLayout;
    topLayout->addWidget(removeButton);
    topLayout->addWidget(nameEdit);
    topLayout->addWidget(connectButton);
    topLayout->addWidget(viewButton);

    connFrame = new QFrame;
    connFrame->setFixedHeight(100);
    controlLayout = new QGridLayout;
    controlLayout->addWidget(typeLabel,0,0);
    controlLayout->addWidget(addressLabel,1,0);
    controlLayout->addWidget(portLabel,2,0);
    controlLayout->addWidget(typeComboBox,0,1);
    controlLayout->addWidget(addressEdit,1,1);
    controlLayout->addWidget(portEdit,2,1);
    connFrame->setLayout(controlLayout);

    bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(statusBar);
    bottomLayout->addWidget(dataIcon);

    frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine | QFrame::Raised);

    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(connFrame);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addWidget(frame);

    this->setLayout(mainLayout);
    this->setFixedWidth(360);

    // UI SIGNALS
    connect(viewButton,SIGNAL(clicked()),this,SLOT(toggleView()));
    connect(iconTimer,SIGNAL(timeout()),this,SLOT(animateDataIcon()));
    connect(removeButton,SIGNAL(clicked()),this,SLOT(remove()));
    connect(nameEdit,SIGNAL(textChanged(QString)),this,SIGNAL(nameChange()));

    // Data connection Signals
    connect(typeComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(typeChanged(QString)));
    connect(addressEdit,SIGNAL(textChanged(QString)),this,SLOT(addressChanged(QString)));
    connect(portEdit,SIGNAL(textChanged(QString)),this,SLOT(portChanged(QString)));
    connect(connectButton,SIGNAL(clicked()),this,SLOT(toggleConnection()));
    connect(dataConnection,SIGNAL(connectionStatus(connectionState)),this,SLOT(connectionChanged(connectionState)));
    connect(dataConnection,SIGNAL(dataIn(QByteArray)),this,SLOT(signalData()));
    connect(dataConnection,SIGNAL(dataIn(QByteArray)),this,SIGNAL(dataRx(QByteArray)));
    connect(dataConnection,SIGNAL(overrideBaud(QString)),portEdit,SLOT(setText(QString)));
    connect(dataConnection,SIGNAL(connectionError(QString)),this,SLOT(errorReceived(QString)));
}

ConnectionWidget::~ConnectionWidget()
{
    dataConnection->dataDisconnect();
}

void ConnectionWidget::remove()
{
    emit widgetRemoved();
    this->deleteLater();
}

QString ConnectionWidget::getName()
{
    return nameEdit->text();
}

bool ConnectionWidget::nameIsValid(void)
{
    return validName;
}

void ConnectionWidget::toggleView()
{
    connFrame->setVisible(!connFrame->isVisible());
    connFrame->isVisible() ? viewButton->setText("Less") : viewButton->setText("More");
}

void ConnectionWidget::signalData()
{
    // Start IO animation
    dataIconShade=105;
    QString styleString = QString("QLabel{background-color: rgb(%1,%1,%1)}").arg(dataIconShade);
    dataIcon->setStyleSheet(styleString);
    iconTimer->start();
}

void ConnectionWidget::dataTx(QByteArray dataOut)
{
    if(dataConnection->getState()==ACTIVE)
    {
        dataConnection->dataOut(dataOut);
        signalData();
    }
}

void ConnectionWidget::animateDataIcon()
{
    dataIconShade+=15;
    QString styleString = QString("QLabel{background-color: rgb(%1,%1,%1)}").arg(dataIconShade);
    dataIcon->setStyleSheet(styleString);
    if(dataIconShade>=255)
    {
        iconTimer->stop();
    }
}

void ConnectionWidget::setNameValid(bool valid)
{
    if(valid)
    {
        validName = true;
        connectButton->setEnabled(true);
        statusBar->setText("Click the Connect button");
    }
    else
    {
        validName = false;
        connectButton->setEnabled(false);
        statusBar->setText("Name is not valid");
    }
}

void ConnectionWidget::setName(QString newName)
{
    disconnect(nameEdit,SIGNAL(textChanged(QString)),this,SIGNAL(nameChange()));
    nameEdit->setText(newName);
    connect(nameEdit,SIGNAL(textChanged(QString)),this,SIGNAL(nameChange()));
}

void ConnectionWidget::typeChanged(QString newType)
{
    if(newType==QString("TCP"))
    {
        dataConnection->setType(TCP);
        addressLabel->setText("IP address");
        portLabel->setText("Port");
    }
    if(newType==QString("UDP"))
    {
        dataConnection->setType(UDP);
        addressLabel->setText("IP address");
        portLabel->setText("Port");
    }
    if(newType==QString("Serial"))
    {
        dataConnection->setType(SERIAL);
        addressLabel->setText("Serial port");
        portLabel->setText("Baud rate");
    }
}

void ConnectionWidget::addressChanged(QString newAddress)
{
    dataConnection->setAddress_Port(newAddress);
}

void ConnectionWidget::portChanged(QString newPort)
{
    QString r = "\\D";
    newPort = newPort.remove(QRegExp(r));
    portEdit->setText(newPort);
    dataConnection->setPort_Baud(newPort.toLong());
}

void ConnectionWidget::toggleConnection()
{
    if(dataConnection->getState() == INACTIVE)
    {
        dataConnection->dataConnect();
    }
    else if(dataConnection->getState() == ACTIVE)
    {
        dataConnection->dataDisconnect();
        statusBar->setText("Click the Connect button");
    }
}

void ConnectionWidget::connectionChanged(connectionState newState)
{
    if(newState == ACTIVE)
    {
        connectButton->setText("Disconnect");
        connectButton->setEnabled(true);
        statusBar->setText("Connected");
        togglePropertyFields(false);
    }
    else if(newState == CONNECTING)
    {
        connectButton->setEnabled(false);
        statusBar->setText("Establishing connection");
        togglePropertyFields(false);
    }
    else
    {
        connectButton->setText("Connect");
        connectButton->setEnabled(true);
        togglePropertyFields(true);
    }
}

void ConnectionWidget::errorReceived(QString error)
{
    statusBar->setText(error);
}

void ConnectionWidget::togglePropertyFields(bool enabled)
{
    nameEdit->setEnabled(enabled);
    typeComboBox->setEnabled(enabled);
    addressEdit->setEnabled(enabled);
    portEdit->setEnabled(enabled);


}
