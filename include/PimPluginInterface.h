#ifndef PIMPLUGIN_H
#define PIMPLUGIN_H

#include <QObject>
#include <QIcon>

class PimPluginInterface;

struct PluginInfo
{
    PluginInfo()
        : plugin(NULL)
    {}
    bool operator==(const PluginInfo& pi) const
    {   return this->uid == pi.uid;   }

    ushort uid;
    PimPluginInterface* plugin;
};

class PimPluginInterface
    : public QObject
{
public:
    virtual ~PimPluginInterface() {}

    virtual void getInfo(PluginInfo& info) = 0;
    virtual void init() = 0;
    virtual void installTranslator(const QString& locale) = 0;
    virtual QIcon icon() = 0;
    virtual QString name() = 0;

    virtual bool newRecord(QWidget* parent, int id) = 0;
    virtual bool editRecord(QWidget* parent, int id) = 0;
    virtual bool deleteRecord(QWidget* parent, int id) = 0;

    virtual QString recordDisplayName(int id) = 0;
    virtual QString recordSummary(int id) = 0;
};

Q_DECLARE_INTERFACE(PimPluginInterface, "com.pim.PimPluginInterface/1.0")

#endif /* PIMPLUGIN_H */

