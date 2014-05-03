#include <QPushButton>

#include "GuiUser.h"


GuiUser::GuiUser(QObject *parent) :
    QObject(parent)
{
}

void GuiUser::postMouseClick(QPushButton *obj)
{
    QObject::connect(this, SIGNAL(mouseClick()), obj, SLOT(click()), Qt::BlockingQueuedConnection);
    emit mouseClick();
}
