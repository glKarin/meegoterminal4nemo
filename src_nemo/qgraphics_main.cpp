/*
    This file is part of meego-terminal

    Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).

    Contact: Ruslan Mstoi <ruslan.mstoi@nokia.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#include <MApplication>
#include <MApplicationService>
#include <MApplicationWindow>
#include <MComponentCache>
#include <QLocale>
#include <QTranslator>
#include <QScopedPointer>
#include <MTheme>

#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include <iostream>

//#include "qgraphicstermwidget.h"
#include "terminal.h"
#include "MTerminalDisplay.h"

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

/*
 * To enable multiple instances.
 */
class MyApplicationService: public MApplicationService
{
public:
    MyApplicationService(const QString &binName, QObject *parent = 0) :
        MApplicationService("com.karin." + binName, parent)
    {
    }

    void launch()
    {
        launchAnotherWithQProcess();
    }

    void handleServiceRegistrationFailure()
    {
        qDebug() << "MyApplicationService::handleServiceRegistrationFailure()";

        incrementAndRegister();
    }
};

int main(int argc, char *argv[])
{
    Options options;
    parseArgv(argc, argv, options);

    MApplicationService *service = 0;
    if (options.createNewInstance) {
        service = new MyApplicationService(QFileInfo(argv[0]).fileName());
    }

		MApplication *a = MComponentCache::mApplication(argc, argv, DEVELOPER, service);
		QScopedPointer<MApplication> app(a);
		app -> setApplicationName(APP_NAME_SETTINGS);
		app -> setOrganizationName(DEVELOPER);
		app -> setApplicationVersion(VERSION);

#ifdef _KARIN_LOCAL_
		if(MTheme::instance() -> loadCSS(QString(_KARIN_PREFIX_) + "/src_meegotouch/style/karin_mstyle.css"))
#else
		if(MTheme::instance() -> loadCSS(QString(_KARIN_PREFIX_) + "/style/karin_mstyle.css"))
#endif
			qDebug()<<"load stylesheet -> karin_mstyle.css";
		else
			qWarning()<<"stylesheet missing -> karin_mstyle.css";
		QTranslator translator;
#ifdef _KARIN_LOCAL_
		if(translator.load(QString("karin-console.zh_CN.qm"), QString(_KARIN_PREFIX_) + "/src_meegotouch/i18n/"))
		{
			qDebug()<<"Load i18n -> \"karin-console.zh_CN.qm\"";
#else
			QString locale = QLocale::system().name();

			if(translator.load(QString("karin-console.") + locale, QString(_KARIN_PREFIX_) + "/i18n/"))
			{
				qDebug()<<(QString("Load i18n -> \"karin-console.") + locale + ".qm\"");
#endif
				app -> installTranslator(&translator);
			}

			MApplicationWindow window;
			karin::terminal console;
			console.appear(&window);
			console.init(options.cmd, options.cmdArgs);

#ifdef QT_DEBUG
			// disable meegotouch handlers
			qInstallMsgHandler(0);
#endif

			// QGraphicsTermWidget *console = new QGraphicsTermWidget(1, 0);

			QObject::connect(&console, SIGNAL(finished()), a, SLOT(quit()));

			window.show();

			return app -> exec();
		}

