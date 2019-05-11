/*  Copyright (C) 2008 e_k (e_k@users.sourceforge.net)

    Port to Qt Graphics View Framework. Portion Copyright (C) 2011 Nokia
    Corporation and/or its subsidiary(-ies). All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
		
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
				
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
						

#include "qgraphicstermwidget.h"

#include <QApplication>

#include "Session.h"
#include "TerminalDisplay.h"

using namespace Konsole;

void *createTermWidget(int startnow, void *parent)
{ 
    return (void*) new QGraphicsTermWidget(startnow, (QGraphicsWidget*)parent); 
}

QGraphicsTermWidget:: QGraphicsTermWidget(QGraphicsWidget *parent):
    QGraphicsWidget(parent),
    m_terminalDisplay(0),
    m_session(0)
{

}

QGraphicsTermWidget::QGraphicsTermWidget(int startnow, QGraphicsWidget *parent)
 :QGraphicsWidget(parent),
  m_terminalDisplay(0),
  m_session(0)
{
    m_session = createSession();
    m_terminalDisplay = createTerminalDisplay(m_session);

    init();

    if (startnow && m_session) {
	m_session->run();
    }

    // set focus policy so that display gets focus, default focus policy is
    // Qt::NoFocus which means ItemIsFocusable flag is not set
     this->setFocusPolicy( m_terminalDisplay->focusPolicy() ); 

    this->setFocus( Qt::OtherFocusReason );
    m_terminalDisplay->resize(this->size());
    
    this->setFocusProxy(m_terminalDisplay);
}

Session *QGraphicsTermWidget::createSession()
{
    Session *session = new Session();

    session->setTitle(Session::NameRole, "QGraphicsTermWidget");

    // if SHELL is not set fallback to sh
    QString program = qgetenv("SHELL");
    if (program.isEmpty()) {
        program = "/bin/sh";
    }

    // dash prepended to start a login shell, so that commands from the profile
    // files (for sh /etc/profile and ~/.profile) are read
    QStringList args("-" + program);

    session->setProgram(program);
    session->setArguments(args);
    session->setAutoClose(true);
		    
    session->setCodec(QTextCodec::codecForName("UTF-8"));
			
    session->setFlowControlEnabled(true);
    session->setHistoryType(HistoryTypeBuffer(1000));
    
    session->setDarkBackground(true);
	    
    session->setKeyBindings("");	    
    return session;
}

TerminalDisplay *QGraphicsTermWidget::createTerminalDisplay(Session *session)
{
    TerminalDisplay* display = new TerminalDisplay(this);

    display->setBellMode(TerminalDisplay::NotifyBell);
    display->setTerminalSizeHint(true);
    display->setTripleClickMode(TerminalDisplay::SelectWholeLine);
    display->setTerminalSizeStartup(true);

    display->setRandomSeed(session->sessionId() * 31);
    
    return display;
}

void QGraphicsTermWidget::startShellProgram()
{
    if ( m_session->isRunning() )
	return;
	
    m_session->run();
}

void QGraphicsTermWidget::init()
{    
    m_terminalDisplay->setSize(80, 40);
    
    QFont font = QApplication::font(); 
    font.setFamily("Monospace");
    font.setPointSize(10);
    font.setStyleHint(QFont::TypeWriter);
    setTerminalFont(font);
    setScrollBarPosition(NoScrollBar);    
        
    m_session->addView(m_terminalDisplay);
    
    connect(m_session, SIGNAL(finished()), this, SLOT(sessionFinished()));
}


QGraphicsTermWidget::~QGraphicsTermWidget()
{
    emit destroyed();
}


void QGraphicsTermWidget::setTerminalFont(QFont &font)
{
    if (!m_terminalDisplay)
	return;
    m_terminalDisplay->setVTFont(font);
}

void QGraphicsTermWidget::setShellProgram(const QString &progname)
{
    if (!m_session)
	return;
    m_session->setProgram(progname);	
}

void QGraphicsTermWidget::setWorkingDirectory(const QString& dir)
{
    if (!m_session)
        return;
    m_session->setInitialWorkingDirectory(dir);
}

void QGraphicsTermWidget::setArgs(QStringList &args)
{
    if (!m_session)
	return;
    m_session->setArguments(args);	
}

void QGraphicsTermWidget::setTextCodec(QTextCodec *codec)
{
    if (!m_session)
	return;
    m_session->setCodec(codec);	
}

void QGraphicsTermWidget::setColorScheme(int scheme)
{
    switch(scheme) {
	case COLOR_SCHEME_WHITE_ON_BLACK:
		m_terminalDisplay->setColorTable(whiteonblack_color_table);
		break;		
	case COLOR_SCHEME_GREEN_ON_BLACK:
		m_terminalDisplay->setColorTable(greenonblack_color_table);
		break;
	case COLOR_SCHEME_GREY_ON_BLACK:
		m_terminalDisplay->setColorTable(greyonblack_color_table);
		break;
	case COLOR_SCHEME_BLACK_ON_LIGHT_YELLOW:
		m_terminalDisplay->setColorTable(blackonlightyellow_color_table);
		break;
	default: //do nothing
	    break;
    };
}

void QGraphicsTermWidget::setSize(int h, int v)
{
    if (!m_terminalDisplay)
	return;
    m_terminalDisplay->setSize(h, v);
}

void QGraphicsTermWidget::setHistorySize(int lines)
{
    if (lines < 0)
        m_session->setHistoryType(HistoryTypeFile());
    else
	m_session->setHistoryType(HistoryTypeBuffer(lines));
}

void QGraphicsTermWidget::setScrollBarPosition(ScrollBarPosition pos)
{
    if (!m_terminalDisplay)
	return;
    m_terminalDisplay->setScrollBarPosition((TerminalDisplay::ScrollBarPosition)pos);
}

void QGraphicsTermWidget::sendText(QString &text)
{
    m_session->sendText(text); 
}

void QGraphicsTermWidget::resizeEvent(QGraphicsSceneResizeEvent*)
{
// qDebug("global window resizing...with %f %f", this->size().width(), this->size().height());
    m_terminalDisplay->resize(this->size());
}


void QGraphicsTermWidget::sessionFinished()
{
    emit finished();
}


void QGraphicsTermWidget::copyClipboard()
{
    m_terminalDisplay->copyClipboard();
}

void QGraphicsTermWidget::pasteClipboard()
{
    m_terminalDisplay->pasteClipboard();
}

void QGraphicsTermWidget::setFlowControlEnabled(bool enabled)
{
    m_session->setFlowControlEnabled(enabled);
}

bool QGraphicsTermWidget::flowControlEnabled(void)
{
    return m_session->flowControlEnabled();
}

void QGraphicsTermWidget::setFlowControlWarningEnabled(bool enabled)
{
    if(flowControlEnabled()) {
	// Do not show warning label if flow control is disabled
	m_terminalDisplay->setFlowControlWarningEnabled(enabled);
    }
}

void QGraphicsTermWidget::setEnvironment(const QStringList& environment)
{
    m_session->setEnvironment(environment);
}
