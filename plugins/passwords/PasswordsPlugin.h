#ifndef PASSWORDS_H
#define PASSWORDS_H

#include <QTranslator>

#include "PimPluginInterface.h"

class PasswordsPlugin
    : public PimPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PimPluginInterface)

public:
    virtual ~PasswordsPlugin() {}

    virtual void getInfo(PluginInfo& info);
    virtual void init();
    virtual void installTranslator(const QString& locale);
    virtual QIcon icon();
    virtual QString name();

    virtual bool newRecord(QWidget* parent, int id);
    virtual bool editRecord(QWidget* parent, int id);
    virtual bool deleteRecord(QWidget* parent, int id);

    virtual QString recordDisplayName(int id);
    virtual QString recordSummary(int id);
private:
    QTranslator m_tr;
};


#endif /* PASSWORDS_H */

