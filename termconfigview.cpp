#include "termconfigview.h"
//#include "terminalarea.h"

TermConfigView::TermConfigView(QWidget *parent) :
    QGraphicsView(parent)
{
    configItem = new TermConfigItem;
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::white);
    setRenderHint(QPainter::Antialiasing);
    setScene(scene);

    setAlignment(Qt::AlignTop | Qt::AlignLeft);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setTransformationAnchor(NoAnchor);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    scene->addItem(configItem);
    setMinimumWidth(500);
}

void TermConfigView::resizeEvent(QResizeEvent *event)
{
    configItem->resize(event->size());
    QRectF rect = scene->itemsBoundingRect();
    if(rect.isNull())
    {
        scene->setSceneRect(QRectF(0,0,1,1));
    }
    else
    {
        scene->setSceneRect(rect);
    }

    QGraphicsView::resizeEvent(event);
}
