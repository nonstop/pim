#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>

#include "config.h"

#include "PluginManager.h"

#include "PimMainWindow.h"

int main(int ac, char* av[])
{
    QApplication app(ac, av);

    QString locale = QLocale::system().name();

    QTranslator translator;
    QString trFile(PIM_TRANSLATIONS_DIR);
    trFile += "/pim_" + locale;
    if (!translator.load(trFile))
        qDebug() << "failed loading translations from " << trFile;
    else
        qDebug() << "translations loaded from " << trFile;
    QApplication::installTranslator(&translator);
    
    PluginManager::instance()->installTranslators(locale);

    PimMainWindow wnd;
    wnd.show();

    return app.exec();
}

