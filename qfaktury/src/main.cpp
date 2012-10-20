#include <QApplication>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QSplashScreen>

#include "dialogs/MainWindow.h"
#include "SettingsGlobal.h"


int main(int argc, char **argv)
{
	QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings (QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForLocale (QTextCodec::codecForName ("UTF-8"));
    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));

    a.setApplicationName("QFaktury");
    a.setOrganizationName("www.e-linux.pl");
    a.setOrganizationDomain("www.e-linux.pl");
    a.setApplicationVersion(APP_VERSION);

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

	return a.exec();
}
