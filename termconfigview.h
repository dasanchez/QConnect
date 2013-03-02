#ifndef TERMCONFIGVIEW_H
#define TERMCONFIGVIEW_H

#include <QGraphicsView>
#include "termconfigitem.h"

class TermConfigView : public QGraphicsView
{
    Q_OBJECT
public:
    TermConfigView(QWidget *parent = 0);
    TermConfigItem *configItem;
signals:

private:
      QGraphicsScene *scene;

    void resizeEvent( QResizeEvent *event );

public slots:

};

#endif // TERMCONFIGVIEW_H
