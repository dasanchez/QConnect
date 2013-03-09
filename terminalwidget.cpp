#include "terminalwidget.h"

TerminalWidget::TerminalWidget(QWidget *parent) :
    QWidget(parent)
{

    delIconPixmap = QPixmap("../QConnect/delete_icon.png");
    echoOnIconPixmap = QPixmap("../QConnect/echoon_icon.png");
    echoOffIconPixmap = QPixmap("../QConnect/echooff_icon.png");
    playIconPixmap = QPixmap("../QConnect/play_icon.png");
    pauseIconPixmap = QPixmap("../QConnect/pause_icon.png");
    clearIconPixmap = QPixmap("../QConnect/clear_icon.png");
    ascIconPixmap = QPixmap("../QConnect/asc_icon.png");
    hexIconPixmap = QPixmap("../QConnect/hex_icon.png");
    sendIconPixmap = QPixmap("../QConnect/send_icon.png");

    connectionWidget = new ConnectionWidget;
    paused = false;
    echoing=false;
    hexPacket=false;

    setupUI();

    togglePacketFormat();

    packetEdit->setText("ff4d");

    QString mypacket = packetEdit->text();

    // switch to bytearray:
    //    QByteArray ba = mypacket.toLocal8Bit();
    //    QByteArray ba2 = QByteArray::fromHex(ba);
    //    ba = ba2.toHex();
    //    QString mypacket2 = QString::fromLatin1(ba2);
    //    QByteArray b3;
    //    b3.append(mypacket2);
    //    QByteArray b4 = b3.toHex();

    //    qDebug() << mypacket << ": " << ba << " -> " << ba2 << " -> " << ba << ": " << mypacket2 << " -> " << b3 << " -> " << b4;


    //    togglePacketFormat();
    //    qDebug() << packetEdit->text();


    asciiTerminal->setMaximumWidth(this->size().width()/4);
    connect(removeButton,SIGNAL(clicked()),this,SLOT(remove()));
    connect(connectionBox,SIGNAL(activated(QString)),this,SLOT(changeConnection(QString)));
    connect(asciiButton,SIGNAL(toggled(bool)),this,SLOT(asciiTermToggled(bool)));
    connect(hexButton,SIGNAL(toggled(bool)),this,SLOT(hexTermToggled(bool)));
    connect(asciiButton,SIGNAL(toggled(bool)),this,SLOT(resizeTerminals()));
    connect(hexButton,SIGNAL(toggled(bool)),this,SLOT(resizeTerminals()));
    connect(echoButton,SIGNAL(clicked()),asciiTerminal,SLOT(toggleEcho()));
    connect(echoButton,SIGNAL(clicked()),hexTerminal,SLOT(toggleEcho()));
    connect(echoButton,SIGNAL(clicked()),this,SLOT(toggleEcho()));
    connect(pauseButton,SIGNAL(clicked()),this,SLOT(togglePause()));
    connect(clearButton,SIGNAL(clicked()),asciiTerminal,SLOT(clear()));
    connect(clearButton,SIGNAL(clicked()),hexTerminal,SLOT(clear()));
    connect(hexPacketButton,SIGNAL(clicked()),this,SLOT(togglePacketFormat()));
    connect(sendButton,SIGNAL(clicked()),this,SLOT(sendPacket()));
    connect(asciiTerminal,SIGNAL(textEntered(QString,bool)),this,SLOT(textEntered(QString,bool)));
    connect(hexTerminal,SIGNAL(textEntered(QString,bool)),this,SLOT(textEntered(QString,bool)));

    // Styling
    QFile qss("../QConnect/terminalwidget.css");
    qss.open(QFile::ReadOnly);
    setStyleSheet(qss.readAll());
    qss.close();
}

TerminalWidget::~TerminalWidget()
{

}

void TerminalWidget::toggleEcho()
{
    echoing = !echoing;
    if(echoing)
    {
        echoButton->setIcon(QIcon(echoOnIconPixmap));
    }
    else
    {
        echoButton->setIcon(QIcon(echoOffIconPixmap));
    }
}

void TerminalWidget::changeConnection(QString connection)
{
    disconnect(connectionWidget,SIGNAL(dataRx(QByteArray)),this,SLOT(dataReceived(QByteArray)));
    disconnect(this,SIGNAL(sendData(QByteArray)),connectionWidget,SLOT(dataTx(QByteArray)));
    emit terminalConnectionRequest(connection);
}

void TerminalWidget::remove()
{
    emit widgetRemoved();

    this->deleteLater();
}

void TerminalWidget::resizeEvent(QResizeEvent *ev)
{
    resizeTerminals();
    ev->accept();
}

void TerminalWidget::resizeTerminals()
{
    QSize newSize = this->size();
    if(asciiTerminal->isVisible())
    {
        if(hexTerminal->isVisible())
        {
            asciiTerminal->setMinimumWidth(newSize.width()/4);
            asciiTerminal->setMaximumWidth(newSize.width()/4);
        }
        else
        {
            asciiTerminal->setMaximumWidth(newSize.width());
        }
    }
    else
    {
        if(hexTerminal->isVisible())
        {
            hexTerminal->setMaximumWidth(newSize.width());
        }
        else
        {

        }
    }
}

void TerminalWidget::updateConnections(QStringList connectionNames)
{
    connectionBox->clear();
    connectionBox->addItems(connectionNames);
    int index = connectionBox->findText(connectionWidget->getName());
    if(index>=0)
        connectionBox->setCurrentIndex(index);

}

void TerminalWidget::textEntered(QString newText, bool isHex)
{
    QByteArray data;
    data.append(newText);

    if(isHex)
    {
        if(hexTerminal->willEcho())
            asciiTerminal->appendText(data,true);
    }
    else
    {
        if(asciiTerminal->willEcho())
            hexTerminal->appendText(data,true);
    }
    QByteArray output;
    output.append(data);
    emit sendData(output);

}

void TerminalWidget::asciiTermToggled(bool on)
{
    if(on)
    {
        hexButton->setDisabled(false);
        asciiTerminal->setVisible(true);
    }
    else
    {
        hexButton->setDisabled(true);
        asciiTerminal->setVisible(false);
    }
}

void TerminalWidget::hexTermToggled(bool on)
{
    if(on)
    {
        asciiButton->setDisabled(false);
        hexTerminal->setVisible(true);
    }
    else
    {
        asciiButton->setDisabled(true);
        hexTerminal->setVisible(false);
    }
}

void TerminalWidget::togglePacketFormat()
{
    hexPacket = !hexPacket;
    if(hexPacket)
    {
        packetEdit->setText(char2hex(packetEdit->text()));
        hexPacketButton->setIcon(QIcon(hexIconPixmap));
    }
    else
    {

        packetEdit->setText(hex2char(packetEdit->text()));
        hexPacketButton->setIcon(QIcon(ascIconPixmap));
    }
}


void TerminalWidget::dataReceived(QByteArray dataIn)
{
//    QByteArray data;
//    qDebug() << "received: " << dataIn.toHex();

//    foreach (char ch , dataIn){
//        if(ch!=0)
//            data.append(ch);
//    }
//        qDebug() << data;

    if(!paused)
    {
        asciiTerminal->appendText(dataIn);
        hexTerminal->appendText(dataIn);
    }
}

void TerminalWidget::togglePause()
{
    paused=!paused;
    if(paused)
    {
        pauseButton->setIcon(QIcon(pauseIconPixmap));
    }
    else
    {
        pauseButton->setIcon(QIcon(playIconPixmap));
    }
}

void TerminalWidget::sendPacket()
{
    QString displayPacket = packetEdit->text();
    QByteArray output;
    if(hexPacket)
    {
        output.append(displayPacket);
        output = QByteArray::fromHex(output);
        qDebug() << "sending: " << output.toHex();

    }
    else
    {
        output.append(displayPacket);
    }
    if(echoing)
    {
        asciiTerminal->appendText(output,true);
        hexTerminal->appendText(output,true);
    }

//    output.append(packet);
    emit sendData(output);
}

QString TerminalWidget::char2hex(QString characters)
{
    QString hexChars;
    // Switch all characters to hex (00-ff) representation
    while(characters.size()>0)
    {
        QByteArray temparray;
        temparray.append(characters.left(1));
        hexChars.append(temparray.toHex().toUpper());
        hexChars.append(" ");
        characters.remove(0,1);
    }
    return hexChars;
}

QString TerminalWidget::hex2char(QString hexChars)
{
    // Change all hex data back to ascii representation
    QByteArray characters;
    hexRegex.indexIn(hexChars);
    QString temp = hexRegex.cap();

    while(temp.size()>0)
    {
        QByteArray temparray;
        temparray.clear();
        temparray.append(temp.left(2));
        characters.append(tr(QByteArray::fromHex(temparray)));
        //        qDebug() << characters.toHex();
        temp.remove(0,2);
        if(temp.size()>0 && temp.at(0)==' ')
        {
            temp.remove(0,1);
        }
    }
    return characters;
}

void TerminalWidget::assignConnection(ConnectionWidget *connWidget)
{
    connectionWidget=connWidget;
    connect(connectionWidget,SIGNAL(dataRx(QByteArray)),this,SLOT(dataReceived(QByteArray)));
    connect(this,SIGNAL(sendData(QByteArray)),connectionWidget,SLOT(dataTx(QByteArray)));
    connect(connectionWidget,SIGNAL(widgetRemoved()),this,SLOT(detachConnection()));
}

void TerminalWidget::detachConnection()
{
    connectionWidget = new ConnectionWidget;
}

void TerminalWidget::setupUI()
{
    asciiTerminal = new QTerminalEdit;
    hexTerminal = new QTerminalEdit;
    hexTerminal->setHex(true);
    hexRegex.setPattern(QString("([A-Fa-f0-9]{2}( )?)+"));

    // UI controls

    connectionBox = new QComboBox;
    connectionBox->setToolTip("Select a connection");
    connectionBox->setFixedHeight(24);
    connectionBox->setMinimumWidth(100);

    asciiButton = new QPushButton("Ascii");
    asciiButton->setToolTip("Show or hide the ASCII terminal");
    asciiButton->setCheckable(true);
    asciiButton->setChecked(true);
    asciiButton->setFixedHeight(24);
    asciiButton->setObjectName("asciiButton");
    hexButton = new QPushButton("Hex");
    hexButton->setToolTip("Show or hide the hex terminal");
    hexButton->setCheckable(true);
    hexButton->setChecked(true);
    hexButton->setFixedHeight(24);
    hexButton->setObjectName("hexButton");
    echoButton = new QPushButton;
    echoButton->setToolTip("Toggle echo");
    echoButton->setIcon(QIcon(echoOffIconPixmap));
    echoButton->setFixedHeight(24);
    echoButton->setFixedWidth(24);
    pauseButton = new QPushButton;
    pauseButton->setToolTip("Pause terminal display");
    pauseButton->setFixedHeight(24);
    pauseButton->setFixedWidth(24);
    pauseButton->setIcon(QIcon(playIconPixmap));
    clearButton = new QPushButton;
    clearButton->setToolTip("Clear terminal views");
    clearButton->setIcon(QIcon(clearIconPixmap));
    clearButton->setFixedHeight(24);
    clearButton->setFixedWidth(24);
    removeButton = new QPushButton;
    removeButton->setToolTip("Remove terminal");
    removeButton->setIcon(QIcon(delIconPixmap));
    removeButton->setFixedHeight(24);
    removeButton->setFixedWidth(24);

    packetEdit = new QLineEdit;
    packetEdit->setToolTip("Enter packet to transmit");
    packetEdit->setFixedHeight(24);
    hexPacketButton = new QPushButton;
    hexPacketButton->setIcon(QIcon(ascIconPixmap));
    hexPacketButton->setFixedHeight(24);
    hexPacketButton->setFixedWidth(24);
    hexPacketButton->setToolTip("Toggle hex display");
    sendButton = new QPushButton;
    sendButton->setToolTip("Transmit packet");
    sendButton->setIcon(QIcon(sendIconPixmap));
    sendButton->setFixedHeight(24);
    sendButton->setFixedWidth(24);

    // Layout:
    controlLayout = new QHBoxLayout;
    controlLayout->addWidget(connectionBox);

    aschexLayout = new QHBoxLayout;
    aschexLayout->addWidget(asciiButton);
    aschexLayout->addWidget(hexButton);
    aschexLayout->setSpacing(0);

    controlLayout->addLayout(aschexLayout);
    controlLayout->addWidget(echoButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(removeButton);

    packetLayout = new QHBoxLayout;
    packetLayout->addWidget(packetEdit);
    packetLayout->addWidget(hexPacketButton);
    packetLayout->addWidget(sendButton);

    terminalLayout = new QHBoxLayout;
    terminalLayout->addWidget(asciiTerminal);
    terminalLayout->addWidget(hexTerminal);

    frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine | QFrame::Raised);

    mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(packetLayout);
    mainLayout->addLayout(terminalLayout);
    mainLayout->addWidget(frame);
    this->setLayout(mainLayout);
}
