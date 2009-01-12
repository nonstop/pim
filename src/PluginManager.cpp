#include <QDir>
#include <QPluginLoader>
#include <QDebug>

#include "config.h"

#include "PluginManager.h"

#define DBG qDebug() << __FILE__ << ":" << __LINE__ << " "

PluginManager* PluginManager::instance()
{
    static PluginManager* pm = NULL;
    if (!pm)
        pm = new PluginManager;
    return pm;
}

PluginManager::PluginManager()
    : m_defaultDir(PIM_PLUGINS_DIR)
{
    loadPluginsFromDir(m_defaultDir);
}

void PluginManager::loadPluginsFromDir(const QString& path)
{
    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files, QDir::Name);
    for (int i=0; i<files.size(); i++)
    {
        DBG << "loading " << dir.absoluteFilePath(files[i]);
        QPluginLoader pl(dir.absoluteFilePath(files[i]));
        pl.load();
        if (!pl.isLoaded())
        {
            DBG << "loading failed";
            continue;
        }
        PimPluginInterface *plugin = qobject_cast<PimPluginInterface*>(pl.instance());
        if (!plugin)
        {
            pl.unload();
            DBG << "qobject_cast failed";
            continue;
        }
        PluginInfo pi;
        plugin->getInfo(pi);
        DBG << "uid [" << pi.uid << "] found";
        if (m_plugins.indexOf(pi) >= 0)
        {
            DBG << "uid [" << pi.uid << "] already used. unloading";
            pl.unload();
            continue;
        }
        pi.plugin = plugin;
        m_plugins.push_back(pi);
        DBG << "uid [" << pi.uid << "] loaded";
    }
}

PimPluginInterface* PluginManager::plugin(int uid)
{
    for (int i=0; i<m_plugins.size(); ++i)
    {
        if (m_plugins[i].uid == uid)
            return m_plugins[i].plugin;
    }
    DBG << "plugin with uid[" << uid << "] not found";
    return NULL;
}

void PluginManager::installTranslators(const QString& locale)
{
    for (int i=0; i<m_plugins.size(); ++i)
        m_plugins[i].plugin->installTranslator(locale);
}

