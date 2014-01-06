#ifndef GUIUSER_H
#define GUIUSER_H

#include <QObject>

class GuiUser : public QObject
{
    Q_OBJECT
public:
    explicit GuiUser(QObject *parent = 0);
    virtual ~GuiUser() {}
    
signals:
    void finished();
public slots:
    virtual void process() = 0;
};

#endif // GUIUSER_H
