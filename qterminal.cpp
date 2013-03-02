#include "qterminal.h"

QTerminal::QTerminal(QWidget *parent):
    QWidget()
{
    termConfig = new TermConfigView;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(termConfig);

    this->setLayout(mainLayout);
    this->setMinimumHeight(160);

}
