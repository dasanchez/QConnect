#ifndef TERMINALAREA_H
#define TERMINALAREA_H

#include <QGraphicsTextItem>
#include <QGraphicsLayoutItem>
#include <QGraphicsItem>
#include <QTextDocument>
#include <QStyle>
#include <QPainter>
#include <QTextCursor>
#include <QKeyEvent>
#include <QDebug>
#include <QTextBlock>
#include <QScrollBar>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

class TerminalArea : public QGraphicsTextItem
{
    Q_OBJECT
public:
//    TerminalArea(QGraphicsItem *parent = 0,QGraphicsScene *scene=0);
    TerminalArea(QGraphicsItem* parent=0);
    TerminalArea(const QString& text, QGraphicsItem* parent=0);
    void setEcho(bool newEcho);
    void setHex(bool newHex);
    void setHeight(qreal);
    void setWidth(qreal);
    void clear(void);
    void adjustHeight(void);

protected:
//QRectF boundingRect() const;
//void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *);
void paint(QPainter *p, const QStyleOptionGraphicsItem *co, QWidget *w);
bool contains(const QPointF &point) const;
QPainterPath shape() const;

private:
    bool echo;
    bool hex;
    qreal itemHeight;
    qreal itemWidth;
signals:
    
public slots:
    
};

#endif // TERMINALAREA_H
