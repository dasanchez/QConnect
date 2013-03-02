#ifndef TERMCONFIGITEM_H
#define TERMCONFIGITEM_H

#include <QGraphicsObject>
#include <QPen>
#include <QPainter>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QGraphicsLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include "terminalarea.h"

#define ITEMWIDTH 480
#define CLOSEBTNX       5
#define CLOSEBTNY       4
#define CLOSEBTNWIDTH   20
#define CLOSEBTNHEIGHT  20
#define CONNNAMEBTNWIDTH    160
#define CONNBTNHEIGHT   20
#define CONNNAMEX       32
#define CONNNAMEY       8

#define DISPOPTX        200
#define DISPOPTWIDTH    50
#define DISPOPTHEIGHT   20
#define TERMINALYOFFSET 64

#define ECHOBTNX        280
#define ECHOBTNY        4
#define ECHOBTNWIDTH    50
#define ECHOBTNHEIGHT   20

#define PAUSEBTNX       315
#define PAUSEBTNY       4
#define PAUSEBTNWIDTH   50
#define PAUSEBTNHEIGHT  20
#define CLEARBTNX       375
#define CLEARBTNY       4
#define CLEARBTNWIDTH   50
#define CLEARBTNHEIGHT  20

#define HEXPACKETX      5
#define HEXPACKETY      32
#define HEXPACKETWIDTH  20
#define HEXPACKETHEIGHT 20

#define SENDPACKETX      320
#define SENDPACKETY      32
#define SENDPACKETWIDTH  50
#define SENDPACKETHEIGHT 20


class TermConfigItem : public QGraphicsObject
{
    Q_OBJECT
public:
    TermConfigItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void updateNameList(QList<QString>);
signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    qreal penWidth;
    qreal itemWidth;
    qreal itemHeight;
    bool viewEcho;
    bool viewAscii;
    bool viewHex;
    bool paused;
    bool hexPacket;

    QRectF connectionBox;
    QRectF viewechoBox;
    QRectF viewasciiBox;
    QRectF viewhexBox;
    QRectF pauseBox;
    QRectF clearBox;
    QRectF sendhexBox;
    QRectF sendBox;

    QComboBox *connCombo;
    QGraphicsProxyWidget *comboWidget;

    QLineEdit *packetLine;
    QGraphicsProxyWidget *packetWidget;


//    QGraphicsProxyWidget *termAsciiWidget;
//    QGraphicsProxyWidget *termHexWidget;

    TerminalArea *termAscii;
    TerminalArea *termHex;
//    QGraphicsLayoutItem *termAsciiItem;
//    QGraphicsLayoutItem *termHexItem;
//    QGraphicsLinearLayout *termLayout;
    void moveButtons(void);
    void adjustTerminals(void);

public slots:
    void resize(QSize);
    void dataRx(quint8,QByteArray);

};

#endif // TERMCONFIGITEM_H
