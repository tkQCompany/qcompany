#include <QApplication>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QSplashScreen>
#include <QTranslator>
#include <QLibraryInfo>

#include "MainWindow.h"
#include "SettingsGlobal.h"

//#ifndef MAIN_CPP
//#define MAIN_CPP

int main(int argc, char **argv)
{
	QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("UTF-8"));
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName ("UTF-8"));
#endif

    a.setApplicationName("QFaktury");
    a.setOrganizationName("www.e-linux.pl");
    a.setOrganizationDomain("www.e-linux.pl");
    a.setApplicationVersion(APP_VERSION);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

	QRect screen = QApplication::desktop()->screenGeometry();
    MainWindow w;
	w.move(screen.center() - QPoint(w.width() / 2, w.height() / 2));

    if (a.arguments().contains("--nosplash"))
    {
		w.show();
    }
    else
    {
        QSplashScreen splash(QPixmap(":/res/icons/splash.png"));
		splash.show();
        splash.showMessage(QObject::trUtf8("Uruchamianie programu..."), Qt::AlignBottom | Qt::AlignLeft, Qt::white);
        w.show();
        splash.finish(&w);
	}

    SettingsGlobal s;
    if(s.value(s.FIRST_RUN, true).toBool())//if the app is run first time
    {
        s.setFirstRun(false);
        w.editMyCompanyInfo();
    }

	return a.exec();
}

//#endif
