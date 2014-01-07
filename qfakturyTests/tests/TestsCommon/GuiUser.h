#ifndef GUIUSER_H
#define GUIUSER_H

#include <QObject>

class QPushButton;


class GuiUser : public QObject
{
    Q_OBJECT
public:
    explicit GuiUser(QObject *parent = 0);
    virtual ~GuiUser() {}
    
signals:
    void finished();
    void mouseClick();

public slots:
    virtual void process() = 0;

protected:
    void postMouseClick(QPushButton *obj);
};

#endif // GUIUSER_H
