#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QVector>
#include "PimPluginInterface.h"

typedef QVector<PluginInfo> Plugins;

class PluginManager
{
public:
    static PluginManager* instance();

    const Plugins& plugins()
    {   return m_plugins;   }
    PimPluginInterface* plugin(int uid);
    void installTranslators(const QString& locale);
private:
    PluginManager();

    void loadPluginsFromDir(const QString& path);

    QString m_defaultDir;
    QVector<PluginInfo> m_plugins;
};


#endif /* PLUGINMANAGER_H */

