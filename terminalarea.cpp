#include "terminalarea.h"

TerminalArea::TerminalArea(QGraphicsItem* parent) :
    QGraphicsTextItem(parent)
{
    echo = false;
    hex = false;
    this->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(ItemSendsGeometryChanges);
    QFont font;
    font.setFamily("Consolas");
    font.setPointSize(10);
    this->setFont(font);

    //   setFlag(QGraphicsItem::ItemUsesExtendedStyleOption);
    //    bar->set


    //    QTextDocument *doc = new QTextDocument;

    //    doc = this->document();
    //    doc->setPageSize(QSize(40,40));
    //    doc->setMaximumBlockCount(1);
    //    this->setDocument(doc);

    //this->set

}

TerminalArea::TerminalArea(const QString& text, QGraphicsItem* parent) :
    QGraphicsTextItem(text, parent)
{
    echo = false;
    hex = false;

}

//void TerminalArea::textChanged()

void TerminalArea::clear()
{
    QTextCursor tc = this->textCursor();
    tc.select(QTextCursor::Document);
    tc.removeSelectedText();
    this->setTextCursor(tc);
}

void TerminalArea::adjustHeight(void)
{
    QTextCursor tc = this->textCursor();
        while(boundingRect().height()>itemHeight)
        {
            tc.movePosition(tc.Start);
            tc.deleteChar();
        }
        setTextCursor(tc);
}



void TerminalArea::keyPressEvent( QKeyEvent *e)
{
    // Push everything back if the length of the text document exceeds the viewing window
    // Get length of window
    // Items to experiment with:
//    QFontMetrics, QTextDocument, boundingRect


    if(echo & !hex)
    {
        QTextCursor tc = this->textCursor();
        tc.movePosition(tc.End);
        tc.insertText(e->text());
        while(boundingRect().height()>itemHeight)
        {
            tc.movePosition(tc.Start);
            tc.deleteChar();
        }
        this->setTextCursor(tc);

    }
    e->accept();

}

//QRectF TerminalArea::boundingRect() const
//{
//    return QRectF(0,0,100,100);
//}

void TerminalArea::setEcho(bool newEcho)
{
    if(newEcho)
    {
        echo=true;
    }
    else
    {
        echo=false;
    }
}

void TerminalArea::setHex(bool newHex)
{
    if(newHex)
    {
        hex=true;
    }
    else
    {
        hex=false;
    }
}

//   void TerminalArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
//   {
//       if(boundingRect().contains(event->pos()))
//       {
//           qDebug("click detected");
////           QTextCursor *tc = new QTextCursor(this->textCursor());
////           tc->setPosition;
//       }
//       event->accept();
//   }

bool TerminalArea::contains(const QPointF &point) const
{
    return QGraphicsItem::contains(point);
}

QPainterPath TerminalArea::shape() const
{
    QPainterPath painterPath;

    QRectF rect = boundingRect();
    rect.setHeight(itemHeight);
    painterPath.addRect(rect);

//    painterPath.addRect(boundingRect());
    return painterPath;
}

//QRectF TerminalArea::boundingRect() const
//{
//    QRectF rect = QGraphicsTextItem::boundingRect();
//    rect.setHeight(itemHeight);
//    return rect;
//}

void TerminalArea::setHeight(qreal newHeight)
{
    itemHeight=newHeight;
}

void TerminalArea::setWidth(qreal newWidth)
{
    itemWidth=newWidth;
}

void TerminalArea::paint(QPainter *p, const QStyleOptionGraphicsItem *co, QWidget *w)
{
    QStyleOptionGraphicsItem *o = const_cast<QStyleOptionGraphicsItem*>(co);
    o->state &= ~QStyle::State_Selected;
    o->state &= ~QStyle::State_HasFocus;

    //    QStyleOptionSlider option;
    //    QStyle::drawComplexControl(QStyle::CC_ScrollBar, &option,p);

//    p->setPen(Qt::black);
//    p->drawRect(boundingRect());
    QGraphicsTextItem::paint(p,o,w);
}
