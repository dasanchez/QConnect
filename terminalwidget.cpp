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

    connectionWidget = new ConnectionWidget;
    paused = false;
    echoing=false;

    asciiTerminal = new QTerminalEdit;
    hexTerminal = new QTerminalEdit;
    hexTerminal->setHex(true);
    hexRegex.setPattern(QString("([A-Fa-f0-9]{2}( )?)+"));

    // UI controls

    connectionBox = new QComboBox;
    connectionBox->setFixedHeight(24);
    connectionBox->setMinimumWidth(100);

    asciiButton = new QPushButton("Ascii");
    asciiButton->setCheckable(true);
    asciiButton->setChecked(true);
    asciiButton->setFixedHeight(24);
    hexButton = new QPushButton("Hex");
    hexButton->setCheckable(true);
    hexButton->setChecked(true);
    hexButton->setFixedHeight(24);
    echoButton = new QPushButton;
    echoButton->setIcon(QIcon(echoOffIconPixmap));
    echoButton->setFixedHeight(24);
    echoButton->setFixedWidth(24);
    pauseButton = new QPushButton;
    pauseButton->setFixedHeight(24);
    pauseButton->setFixedWidth(24);
    pauseButton->setIcon(QIcon(playIconPixmap));
    clearButton = new QPushButton;
    clearButton->setIcon(QIcon(clearIconPixmap));
    clearButton->setFixedHeight(24);
    clearButton->setFixedWidth(24);
    removeButton = new QPushButton;
    removeButton->setIcon(QIcon(delIconPixmap));
    removeButton->setFixedHeight(24);
    removeButton->setFixedWidth(24);

    hexPacketButton = new QPushButton("Hex");
    hexPacketButton->setCheckable(true);
    hexPacketButton->setChecked(false);
    hexPacketButton->setFixedHeight(24);
    packetEdit = new QLineEdit;
    packetEdit->setFixedHeight(24);
    sendButton = new QPushButton("Send");
    sendButton->setFixedHeight(24);

    // Layout:
    controlLayout = new QHBoxLayout;

    controlLayout->addWidget(connectionBox);

    controlLayout->addWidget(asciiButton);
    controlLayout->addWidget(hexButton);
        controlLayout->addWidget(echoButton);
    controlLayout->addWidget(pauseButton);
    controlLayout->addWidget(clearButton);
    controlLayout->addWidget(removeButton);

    packetLayout = new QHBoxLayout;
    packetLayout->addWidget(hexPacketButton);
    packetLayout->addWidget(packetEdit);
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
    connect(hexPacketButton,SIGNAL(toggled(bool)),this,SLOT(togglePacketFormat(bool)));
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
    if(isHex)
    {
        if(hexTerminal->willEcho())
            asciiTerminal->appendText(newText,true);
    }
    else
    {
        if(asciiTerminal->willEcho())
            hexTerminal->appendText(newText,true);
    }
    QByteArray output;
    output.append(newText);
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

void TerminalWidget::togglePacketFormat(bool hex)
{
    if(hex)
    {
        packetEdit->setText(char2hex(packetEdit->text()));
    }
    else
    {

        packetEdit->setText(hex2char(packetEdit->text()));
    }
}


void TerminalWidget::dataReceived(QByteArray dataIn)
{
    QString data;

    foreach (char ch , dataIn){
        if(ch!=0)
            data.append(ch);
    }
    //    qDebug() << mydata;

    if(!paused)
    {
        asciiTerminal->appendText(data);
        hexTerminal->appendText(data);
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
    QString packet = packetEdit->text();
    if(hexPacketButton->isChecked())
    {
        packet = hex2char(packet);

    }
    if(echoing)
    {
        asciiTerminal->appendText(packet,true);
        hexTerminal->appendText(packet,true);
    }
    QByteArray output;
    output.append(packet);
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
        temparray.append(temp.left(2));
        characters.append(QByteArray::fromHex(temparray));
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

