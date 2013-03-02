#include "termconfigitem.h"

TermConfigItem::TermConfigItem()
{
    //    setFlag(ItemSendsGeometryChanges);
    penWidth=2;
    itemHeight=200;
    itemWidth=ITEMWIDTH;
    viewEcho = false;
    viewAscii=true;
    viewHex=false;
    paused=false;
    hexPacket=false;
    setCacheMode(DeviceCoordinateCache);


    QRectF rec = QRectF(penWidth*2,penWidth*2,itemWidth,itemHeight);

    // 5 buttons:
    connectionBox = QRectF(rec.left()+CONNNAMEX,rec.top()+CLOSEBTNY,rec.width()/2-CONNNAMEX-(2*penWidth),CLOSEBTNHEIGHT);

    viewechoBox = QRectF(rec.left()+rec.width()/2+(rec.width()/2)/6-rec.width()/4/6,rec.top()+CLOSEBTNY,(rec.width()/2)/7,DISPOPTHEIGHT);
    viewasciiBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*2-rec.width()/4/6,rec.top()+CLOSEBTNY,(rec.width()/2)/7,DISPOPTHEIGHT);
    viewhexBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*3-rec.width()/4/6,rec.top()+CLOSEBTNY,(rec.width()/2)/7,DISPOPTHEIGHT);
    pauseBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*4-rec.width()/4/6,rec.top()+PAUSEBTNY,(rec.width()/2)/7,PAUSEBTNHEIGHT);
    clearBox  = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*5-rec.width()/4/6,rec.top()+CLEARBTNY,(rec.width()/2)/7,CLEARBTNHEIGHT);

    sendhexBox = QRectF(rec.left()+HEXPACKETX,rec.top()+HEXPACKETY,HEXPACKETWIDTH,HEXPACKETHEIGHT);
    sendBox = QRectF(rec.width()-rec.width()/2/6,rec.top()+SENDPACKETY,rec.width()/3/5,SENDPACKETHEIGHT);

    connCombo = new QComboBox;
    comboWidget = new QGraphicsProxyWidget(this);
    comboWidget->setWidget(connCombo);
    comboWidget->setPos(connectionBox.x(),connectionBox.y());
    comboWidget->setZValue(2);
    connCombo->addItem("Connection_001");
    connCombo->setMinimumWidth(160);


    //  4 buttons:
    //    connectionBox = QRectF(rec.left()+CONNNAMEX,rec.top()+CLOSEBTNY,rec.width()/2-CONNNAMEX-(2*penWidth),CLOSEBTNHEIGHT);
    //    viewasciiBox = QRectF(rec.left()+rec.width()/2+(rec.width()/2)/5-rec.width()/4/5,rec.top()+CLOSEBTNY,(rec.width()/2)/6,DISPOPTHEIGHT);
    //    viewhexBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/5)*2-rec.width()/4/5,rec.top()+CLOSEBTNY,(rec.width()/2)/6,DISPOPTHEIGHT);
    //    pauseBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/5)*3-rec.width()/4/5,rec.top()+PAUSEBTNY,(rec.width()/2)/6,PAUSEBTNHEIGHT);
    //    clearBox  = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/5)*4-rec.width()/4/5,rec.top()+CLEARBTNY,(rec.width()/2)/6,CLEARBTNHEIGHT);
    //    sendhexBox = QRectF(rec.left()+HEXPACKETX,rec.top()+HEXPACKETY,HEXPACKETWIDTH,HEXPACKETHEIGHT);
    //    sendBox = QRectF(rec.width()-rec.width()/2/6,rec.top()+SENDPACKETY,rec.width()/3/5,SENDPACKETHEIGHT);

    packetLine = new QLineEdit("Enter packet here");
    packetWidget = new QGraphicsProxyWidget(this);
    packetWidget->setWidget(packetLine);
    packetWidget->setPos(HEXPACKETX+HEXPACKETWIDTH+12,HEXPACKETY+5);
    packetWidget->setZValue(1);

    termAscii = new TerminalArea(this);
    termAscii->setPos(8,62);
    //    termAscii->setPlainText("Ascii");
    termAscii->setTextWidth((itemWidth/3)-8);
    //        termAscii->setWidth(itemWidth-8);
    termAscii->setHeight(itemHeight-TERMINALYOFFSET);
    //    termAscii->adjustSize();

    termHex = new TerminalArea(this);
    //    termHex->setPlainText("Hex");
    termHex->setHex(true);
    termHex->setTextWidth(2*(itemWidth/3)-8);
    termHex->setPos((itemWidth/3)+8,62);
    termHex->setHeight(itemHeight-TERMINALYOFFSET);
    termHex->setVisible(false);

}

void TermConfigItem::dataRx(quint8 index, QByteArray newdata)
{
    if(connCombo->currentIndex()==index)
    {
        QTextCursor tc = termAscii->textCursor();
         tc.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        tc.insertText(QString(newdata));
        termAscii->setTextCursor(tc);
//        termAscii->
    }
}

void TermConfigItem::updateNameList(QList<QString> newList)
{
    connCombo->clear();
    for(quint8 i=0;i<newList.size();i++)
    {
        connCombo->addItem(newList.at(i));
    }
}

QRectF TermConfigItem::boundingRect() const
{
    return QRectF(0,0,itemWidth+(2*penWidth),itemHeight+(2*penWidth));
}

void TermConfigItem::resize(QSize newSize)
{
    itemWidth=newSize.width()-10;
    itemHeight=newSize.height()-10;
    packetLine->resize(itemWidth-2*(itemWidth/4/5)-(HEXPACKETX+HEXPACKETWIDTH+12),20);
    adjustTerminals();
}

void TermConfigItem::adjustTerminals(void)
{
    if(viewAscii && !viewHex)
    {
        termAscii->setTextWidth((itemWidth)-8);
        termAscii->setHeight(itemHeight-TERMINALYOFFSET);
    }
    else if(viewAscii && viewHex)
    {
        termAscii->setTextWidth((itemWidth/3)-8);
        termAscii->setHeight(itemHeight-TERMINALYOFFSET);

        termHex->setTextWidth(2*(itemWidth/3)-8);
        termHex->setPos((itemWidth/3)+8,62);
        termHex->setHeight(itemHeight-TERMINALYOFFSET);
    }
    else
    {
        termHex->setPos(8,62);
        termHex->setTextWidth((itemWidth)-8);
        termHex->setHeight(itemHeight-TERMINALYOFFSET);
    }
    termAscii->adjustHeight();
    termHex->adjustHeight();
}

void TermConfigItem::moveButtons(void)
{

}

void TermConfigItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QRectF *termBox = new QRectF;

    termBox->setLeft(termAscii->pos().x());
    termBox->setTop(termAscii->pos().y());
    termBox->setHeight(itemHeight-TERMINALYOFFSET);
    if(viewAscii)
    {
        if(viewHex)
        {
            termBox->setWidth((itemWidth/3)-8);
        }
        else
        {
            termBox->setWidth(itemWidth-8);
        }

        if(termBox->contains(event->pos()))
            termAscii->setFocus();
    }

    // Toggle Echo
    if(viewechoBox.contains(event->pos()))
    {
        if(viewEcho)
        {
            viewEcho=!viewEcho;
            termAscii->setEcho(false);
            termHex->setEcho(false);
        }
        else
        {
            viewEcho=true;
            termAscii->setEcho(true);
            termHex->setEcho(true);
        }
    }

    // Toggle ASCII/HEX VIEW
    if(viewasciiBox.contains(event->pos()))
    {
        if(viewHex)
        {
            viewAscii=!viewAscii;
            if(viewAscii)
            {
                termAscii->setVisible(true);
            }
            else
            {
                termAscii->setVisible(false);
            }
        }
        else
        {
            viewAscii=true;
        }
        adjustTerminals();
    }
    if(viewhexBox.contains(event->pos()))
    {
        if(viewAscii)
        {
            viewHex=!viewHex;
            if(viewHex)
            {
                termHex->setVisible(true);
            }
            else
            {
                termHex->setVisible(false);
            }
        }
        else
        {
            viewHex=true;
        }
        adjustTerminals();
    }

    // Toggle active status
    if(pauseBox.contains(event->pos()))
    {
        paused=!paused;
    }

    // Clear terminals
    if(clearBox.contains(event->pos()))
    {
        termAscii->clear();
        termHex->clear();
    }
    if(sendhexBox.contains(event->pos()))
    {
        hexPacket = !hexPacket;
    }

    if(sendBox.contains(event->pos()))
    {

    }

    event->accept();
    update();
}

void TermConfigItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(widget);

    QRectF rec = QRectF(penWidth*2,penWidth*2,itemWidth,itemHeight);
    QPen rectpen(Qt::black,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);

    painter->setPen(rectpen);
    painter->setBrush(Qt::white);
    painter->fillRect(rec,Qt::white);

    // Rounded Rectangle
    painter->drawRoundedRect(rec,5,5,Qt::AbsoluteSize);

    // Delete button
    QRectF deleteButton = QRectF(rec.left()+CLOSEBTNX,rec.top()+CLOSEBTNY,CLOSEBTNWIDTH,CLOSEBTNHEIGHT);
    painter->drawRoundedRect(deleteButton,5,5,Qt::AbsoluteSize);

    // Draw X
    painter->drawLine(rec.left()+CLOSEBTNX+6,rec.top()+CLOSEBTNY+6,rec.left()+CLOSEBTNX+CLOSEBTNWIDTH-6,rec.top()+CLOSEBTNY+CLOSEBTNHEIGHT-6);
    painter->drawLine(rec.left()+CLOSEBTNX+6,rec.top()+CLOSEBTNY+CLOSEBTNHEIGHT-6,rec.left()+CLOSEBTNX+CLOSEBTNWIDTH-6,rec.top()+CLOSEBTNY+6);

    // Connection chooser
    connectionBox = QRectF(rec.left()+CONNNAMEX,rec.top()+CLOSEBTNY,rec.width()/2-CONNNAMEX-(2*penWidth),CLOSEBTNHEIGHT);
//    painter->drawRoundedRect(connectionBox,5,5,Qt::AbsoluteSize);

    // Connection Name
//    painter->drawText(rec.left()+CONNNAMEX+35, rec.top()+CLOSEBTNY+14,"Connection_001");
    painter->drawText(rec.left()+CONNNAMEX+6, rec.top()+CLOSEBTNY+14,"Connection_001");

    // Arrows
    // Data out:
    QPainterPath leftArrowPath;

//    leftArrowPath.moveTo(connectionBox.left()+4,connectionBox.top()+CONNBTNHEIGHT/2);
//    leftArrowPath.lineTo(connectionBox.left()+9,connectionBox.top()+CONNBTNHEIGHT-5);
//    leftArrowPath.lineTo(connectionBox.left()+9,connectionBox.top()+CONNBTNHEIGHT-8);
//    leftArrowPath.lineTo(connectionBox.left()+13,connectionBox.top()+CONNBTNHEIGHT-8);
//    leftArrowPath.lineTo(connectionBox.left()+13,connectionBox.top()+8);
//    leftArrowPath.lineTo(connectionBox.left()+9,connectionBox.top()+8);
//    leftArrowPath.lineTo(connectionBox.left()+9,connectionBox.top()+5);
    leftArrowPath.moveTo(connectionBox.right()-32,connectionBox.top()+CONNBTNHEIGHT/2);
    leftArrowPath.lineTo(connectionBox.right()-27,connectionBox.top()+CONNBTNHEIGHT-5);
    leftArrowPath.lineTo(connectionBox.right()-27,connectionBox.top()+CONNBTNHEIGHT-8);
    leftArrowPath.lineTo(connectionBox.right()-23,connectionBox.top()+CONNBTNHEIGHT-8);
    leftArrowPath.lineTo(connectionBox.right()-23,connectionBox.top()+8);
    leftArrowPath.lineTo(connectionBox.right()-27,connectionBox.top()+8);
    leftArrowPath.lineTo(connectionBox.right()-27,connectionBox.top()+5);
    leftArrowPath.closeSubpath();
    painter->drawPath(leftArrowPath);

    // Data in:
    QPainterPath rightArrowPath;
//    rightArrowPath.moveTo(connectionBox.left()+26,connectionBox.top()+CONNBTNHEIGHT/2);
//    rightArrowPath.lineTo(connectionBox.left()+21,connectionBox.top()+CONNBTNHEIGHT-5);
//    rightArrowPath.lineTo(connectionBox.left()+21,connectionBox.top()+CONNBTNHEIGHT-8);
//    rightArrowPath.lineTo(connectionBox.left()+17,connectionBox.top()+CONNBTNHEIGHT-8);
//    rightArrowPath.lineTo(connectionBox.left()+17,connectionBox.top()+8);
//    rightArrowPath.lineTo(connectionBox.left()+21,connectionBox.top()+8);
//    rightArrowPath.lineTo(connectionBox.left()+21,connectionBox.top()+5);

    rightArrowPath.moveTo(connectionBox.right()-8,connectionBox.top()+CONNBTNHEIGHT/2);
        rightArrowPath.lineTo(connectionBox.right()-13,connectionBox.top()+CONNBTNHEIGHT-5);
        rightArrowPath.lineTo(connectionBox.right()-13,connectionBox.top()+CONNBTNHEIGHT-8);
        rightArrowPath.lineTo(connectionBox.right()-17,connectionBox.top()+CONNBTNHEIGHT-8);
        rightArrowPath.lineTo(connectionBox.right()-17,connectionBox.top()+8);
        rightArrowPath.lineTo(connectionBox.right()-13,connectionBox.top()+8);
        rightArrowPath.lineTo(connectionBox.right()-13,connectionBox.top()+5);
    rightArrowPath.closeSubpath();
    painter->drawPath(rightArrowPath);

    // Echo Button
    viewechoBox = QRectF(rec.left()+rec.width()/2+(rec.width()/2)/6-rec.width()/4/6,rec.top()+CLOSEBTNY,(rec.width()/2)/7,DISPOPTHEIGHT);
    if(viewEcho)
    {
        // Selected?
        painter->setBrush(QColor("#BBBBBB"));

    }
    painter->drawRoundedRect(viewechoBox,5,5,Qt::AbsoluteSize);
    painter->drawText(viewechoBox.x()+viewechoBox.width()/2-10,viewechoBox.y()+14,"Echo");
    painter->setBrush(Qt::white);

    // Ascii/Hex Buttons
    viewasciiBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*2-rec.width()/4/6,rec.top()+CLOSEBTNY,(rec.width()/2)/7,DISPOPTHEIGHT);
    //    viewasciiBox = QRectF(rec.left()+rec.width()/2+(rec.width()/2)/5-rec.width()/4/5,rec.top()+CLOSEBTNY,(rec.width()/2)/6,DISPOPTHEIGHT);
    if(viewAscii)
    {
        // Selected?
        painter->setBrush(QColor("#BBBBBB"));

    }
    painter->drawRoundedRect(viewasciiBox,5,5,Qt::AbsoluteSize);
    painter->drawText(viewasciiBox.x()+viewasciiBox.width()/2-10,viewasciiBox.y()+14,"Ascii");
    painter->setBrush(Qt::white);

    viewhexBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*3-rec.width()/4/6,rec.top()+CLOSEBTNY,(rec.width()/2)/7,DISPOPTHEIGHT);
    //    viewhexBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/5)*2-rec.width()/4/5,rec.top()+CLOSEBTNY,(rec.width()/2)/6,DISPOPTHEIGHT);
    if(viewHex)
    {
        painter->setBrush(QColor("#BBBBBB"));
    }
    painter->drawRoundedRect(viewhexBox,5,5,Qt::AbsoluteSize);
    painter->drawText(viewhexBox.x()+viewhexBox.width()/2-9,viewhexBox.y()+14,"Hex");
    painter->setBrush(Qt::white);

    // Clear/pause buttons
    pauseBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*4-rec.width()/4/6,rec.top()+PAUSEBTNY,(rec.width()/2)/7,PAUSEBTNHEIGHT);
    //    pauseBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/5)*3-rec.width()/4/5,rec.top()+PAUSEBTNY,(rec.width()/2)/6,PAUSEBTNHEIGHT);
    if(paused)
    {
        painter->setBrush(QColor("#BBBBBB"));
    }
    painter->drawRoundedRect(pauseBox,5,5,Qt::AbsoluteSize);
    painter->drawText(pauseBox.x()+pauseBox.width()/2-13,pauseBox.y()+14,"Pause");
    painter->setBrush(Qt::white);

    clearBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/6)*5-rec.width()/4/6,rec.top()+CLEARBTNY,(rec.width()/2)/7,CLEARBTNHEIGHT);
    //    clearBox = QRectF(rec.left()+rec.width()/2+((rec.width()/2)/5)*4-rec.width()/4/5,rec.top()+CLEARBTNY,(rec.width()/2)/6,CLEARBTNHEIGHT);
    painter->drawRoundedRect(clearBox,5,5,Qt::AbsoluteSize);
    painter->drawText(clearBox.x()+clearBox.width()/2-12,clearBox.y()+14,"Clear");

    // Packet
    sendhexBox = QRectF(rec.left()+HEXPACKETX,rec.top()+HEXPACKETY,HEXPACKETWIDTH,HEXPACKETHEIGHT);
    if(hexPacket)
    {
        painter->setBrush(QColor("#BBBBBB"));
    }
    painter->drawRoundedRect(sendhexBox,5,5,Qt::AbsoluteSize);
    painter->drawText(sendhexBox.x()+4,sendhexBox.y()+14,"0x");
    painter->setBrush(Qt::white);

    // Send Button
    sendBox = QRectF(rec.width()-rec.width()/2/6,rec.top()+SENDPACKETY,rec.width()/3/5,SENDPACKETHEIGHT);
    painter->drawRoundedRect(sendBox,5,5,Qt::AbsoluteSize);
    painter->drawText(sendBox.x()+sendBox.width()/2-11,sendBox.y()+14,"Send");

    // Terminal dividers
    painter->drawLine(rec.left(),rec.top()+60,rec.right(),rec.top()+60);
    if(viewAscii && viewHex)
    {
        painter->drawLine(rec.left()+rec.width()/3,rec.top()+60,rec.left()+rec.width()/3,rec.bottom());
    }
}
