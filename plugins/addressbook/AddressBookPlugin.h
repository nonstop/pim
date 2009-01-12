#ifndef ADDRESSBOOKPLUGIN_H
#define ADDRESSBOOKPLUGIN_H

#include <QTranslator>

#include "PimPluginInterface.h"

class AddressBookPlugin
    : public PimPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PimPluginInterface)

public:
    virtual ~AddressBookPlugin() {}

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

#endif /* ADDRESSBOOKPLUGIN_H */

