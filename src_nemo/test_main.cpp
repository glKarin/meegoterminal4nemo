/*
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

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <iostream>

#include "MTermWidget.h"

using std::cout;

struct Options
{
    Options():
        cmd(),    // empty
        cmdArgs() // empty
    {}

    QString cmd;         // command specified with the -e option
    QStringList cmdArgs; // arguments to pass to the cmd
};

void printHelp(QString appName)
{
    cout << "Usage: " << qPrintable(appName) << " [options] [args]\n\n";
    cout << "Terminal emulator\n\n";
    cout << "Options:\n";
    cout << "  -h, --help\t Show this help message and exit\n";
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
            //return;
            exit(0);
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
 * Currently, the sole purpose of this class is to provide context menu to the
 * terminal widget.
 */
class MyTermWidget : public MTermWidget
{
    // Q_OBJECT
public:
    MyTermWidget(int startnow = 1, QGraphicsWidget *parent = 0):
        MTermWidget(startnow, parent)
    {
        m_contextMenu = new QMenu;
        m_schemeActionGroup = new QActionGroup(this);

        for (int i = COLOR_SCHEME_FIRST; i <= COLOR_SCHEME_LAST; ++i) {
            QAction *act = new QAction(COLOR_SCHEME_NAME[i], this);
            act->setCheckable(true);
            act->setData(i);
            m_schemeActionGroup->addAction(act);

            // default scheme: display sets this in its constructor
            if (i == COLOR_SCHEME_WHITE_ON_BLACK)
                act->setChecked(true);
        }

        m_contextMenu->addActions(m_schemeActionGroup->actions());
    }

    ~MyTermWidget()
    {
        delete m_contextMenu;
    }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
    {
        QAction *selectedAction = m_contextMenu->exec(event->screenPos());
        if (!selectedAction)
            return;

        if (selectedAction->actionGroup() == m_schemeActionGroup) {
            setColorScheme(selectedAction->data().toInt());
        }
    }

private:
    QMenu *m_contextMenu;
    QActionGroup *m_schemeActionGroup;
};

int main(int argc, char *argv[])
{
    int w = 700, h = 600;
    QApplication app(argc, argv);

    Options options;
    parseArgsList(app.arguments(), options);

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setBackgroundBrush(Qt::blue);

    // QGraphicsTermWidget *console = new QGraphicsTermWidget(false, 0);
    MTermWidget *console = new MyTermWidget(false, 0);
    scene.addItem(console);

    QFont font = QApplication::font();
    font.setFamily("Monospace");
    font.setPointSize(12);
    console->setTerminalFont(font);

    if (!options.cmd.isEmpty()) {
        console->setShellProgram(options.cmd);
        console->setArgs(options.cmdArgs);
    }
    console->startShellProgram();

    console->resize(w, h);

    view.resize(720, 1280);
		view.setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QObject::connect(console, SIGNAL(finished()), &app, SLOT(quit()));

    view.show();

    return app.exec();
} 
