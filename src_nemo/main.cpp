#include "main_window.h"
#include "MTermWidget.h"
#include "key.h"
#include "toolbar.h"
#include "terminal.h"
#include "menu.h"

#include <QDebug>
#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <iostream>

using namespace karin;

using std::cout;

struct Options
{
    Options():
        cmd(),    // empty
        cmdArgs(),// empty
        createNewInstance(false)
    {}

    QString cmd;         // command specified with the -e option
    QStringList cmdArgs; // arguments to pass to the cmd
    bool createNewInstance;
};

void printHelp(QString appName)
{
    cout << "Usage: " << qPrintable(appName) << " [options] [args]\n\n";
    cout << "Terminal emulator\n\n";
    cout << "Options:\n";
    cout << "  -h, --help\t Show this help message and exit\n";
    cout << "  -n\t\t Create new instance\n";
    cout << "  -e CMD\t Command to execute, options and arguments after "
        "the CMD will be passed to the CMD\n\n";
}

/**
 * Parses arguments string list, like one returned by
 * QCoreApplication::arguments()
 */
void parseArgsList(QStringList args, Options &options)
{
    for (int i = 0; i < args.size(); ++i) {
        QString s = args.at(i);
        
        if (s == "-h" || s.startsWith("-help") || s.startsWith("--help")) {
            printHelp(QFileInfo(args.at(0)).fileName());
            // let MComponentDataPrivate::parseArguments print its help and exit
            return;
            // exit(0);
        }

        else if (s == "-n") {
            options.createNewInstance = true;
        }

        else if (s == "-e") {
            if (i == args.size() - 1) { // error: -e is last, cmd not specified
                cout << "Please specify command to execute!\n";
                exit(1);
            }

            QString cmd = args.at(i + 1);
            if (cmd.startsWith(QLatin1String("./"))) {
                cmd = QDir::currentPath() + cmd.mid(1);
            }

            QStringList cmdArgs = args.mid(i + 1); // cmd is 1st element

            options.cmd = cmd;
            options.cmdArgs = cmdArgs;

            // -e is last option, everything after it is passed to the command
            break;
        }
    }
}

/**
 * This wrapper exists to do meegotouch specific hacks to the command line
 * arguments handling. The problem is that
 * MComponentDataPrivate::parseArguments calls exit(0) in case one of -h, -help
 * or --help is specified. This it does during MApplication creation. So this
 * function intercepts that, to provide the user with some help text of this
 * app before meegotouch prints its help and exits.
 *
 * [1] If -e option is used, this function will remove it and everything after
 * it from argv and argc. This is because the user might specify some options
 * to the command (say -h), which should not be handled by meegotouch.
 *
 */
void parseArgv(int &argc, char *argv[], Options &options)
{
    QStringList list;

    char ** const av = argv;
    for (int a = 0; a < argc; ++a) {
        list << QString::fromLocal8Bit(av[a]);
    }

    parseArgsList(list, options);

    // see [1]
    int i = list.indexOf("-e");
    if (i != -1) { // list contains -e
        argv[i] = 0;
        argc = i;
    }
}

int main(int argc, char *argv[])
{
#define _LOC_SEP "."
#define _I18N_DIR "i18n"

    Options options;
		QTranslator translator;
		QString qm;
		QString locale;
		QString i18nDir;

		// args
    parseArgv(argc, argv, options);

		// qApp
    QApplication app(argc, argv);
		app.setApplicationName(APP_NAME_SETTINGS);
		app.setOrganizationName(DEVELOPER);
		app.setApplicationVersion(VERSION);

		// i18n
		i18nDir = QString("%1/%2/").arg(_KARIN_PREFIX_).arg(_I18N_DIR);
#ifdef _KARIN_LOCAL_
		locale = "zh_CN";
#else
		locale = QLocale::system().name();
#endif
		qm = QString(APP_NAME_SETTINGS) + _LOC_SEP + locale + ".qm" ;
		if(translator.load(qm, i18nDir))
		{
			qDebug() << QString("Load i18n -> %1%2").arg(i18nDir).arg(qm);
			app.installTranslator(&translator);
		}
		else
			qWarning() << QString("Can not load i18n -> %1%2").arg(i18nDir).arg(qm);

		// gui
		main_window view;
		karin::terminal console;
		QObject::connect(&console, SIGNAL(finished()), &app, SLOT(quit()));
		console.init(options.cmd, options.cmdArgs);
		view.setCentralWidget(&console);
    view.show();

		// mainloop
    return app.exec();
} 

