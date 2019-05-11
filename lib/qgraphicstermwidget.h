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
						    

#ifndef _Q_GRAPHICS_TERM_WIDGET
#define _Q_GRAPHICS_TERM_WIDGET

#include <QtGui>
#include <QGraphicsWidget>

enum COLOR_SCHEME { COLOR_SCHEME_WHITE_ON_BLACK = 1,
                    COLOR_SCHEME_GREEN_ON_BLACK,
                    COLOR_SCHEME_GREY_ON_BLACK,
                    COLOR_SCHEME_BLACK_ON_LIGHT_YELLOW,

                    COLOR_SCHEME_FIRST = COLOR_SCHEME_WHITE_ON_BLACK,
                    COLOR_SCHEME_LAST = COLOR_SCHEME_BLACK_ON_LIGHT_YELLOW };

const QString COLOR_SCHEME_NAME[] = { "UNKNOWN",
                                      "White on Black",
                                      "Green on Black",
                                      "Grey on Black",
                                      "Black on Light Yellow" };

namespace Konsole {
    class Session;
    class TerminalDisplay;
}

using Konsole::Session;
using Konsole::TerminalDisplay;

class QGraphicsTermWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    
    enum ScrollBarPosition
    {
        /** Do not show the scroll bar. */
        NoScrollBar=0,
        /** Show the scroll bar on the left side of the display. */
        ScrollBarLeft=1,
        /** Show the scroll bar on the right side of the display. */
        ScrollBarRight=2
    };

    // default constructor, for children to customize attributes
    QGraphicsTermWidget(QGraphicsWidget *parent = 0);

    //Creation of widget
    QGraphicsTermWidget(int startnow, //start shell programm immediatelly
			QGraphicsWidget *parent = 0);
    virtual ~QGraphicsTermWidget();

    //start shell program if it was not started in constructor
    virtual void startShellProgram();
    
    //look-n-feel, if you don`t like defaults

    //	Terminal font
    // Default is application font with family Monospace, size 10
    // USE ONLY FIXED-PITCH FONT!
    // otherwise symbols' position could be incorrect
    void setTerminalFont(QFont &font); 
    
    //environment
    void setEnvironment(const QStringList& environment);

    //	Shell program, default is /bin/bash
    void setShellProgram(const QString &progname);
    
    //working directory
    void setWorkingDirectory(const QString& dir);

    // Shell program args, default is none
    void setArgs(QStringList &args);
    
    //Text codec, default is UTF-8
    void setTextCodec(QTextCodec *codec);

    //Color scheme, default is white on black
    virtual void setColorScheme(int scheme);
    
    //set size
    void setSize(int h, int v);
    
    // History size for scrolling 
    void setHistorySize(int lines); //infinite if lines < 0

    // Presence of scrollbar
    void setScrollBarPosition(ScrollBarPosition);
    
    // Send some text to terminal
    void sendText(QString &text);

    // Sets whether flow control is enabled
    void setFlowControlEnabled(bool enabled);

    // Returns whether flow control is enabled
    bool flowControlEnabled(void);

    /**
     * Sets whether the flow control warning box should be shown
     * when the flow control stop key (Ctrl+S) is pressed.
     */
    void setFlowControlWarningEnabled(bool enabled);
            
signals:
    void finished();

public slots:
    // Paste clipboard content to terminal
    void copyClipboard();
    
     // Copies selection to clipboard
    void pasteClipboard();
        
protected:
    virtual void resizeEvent(QGraphicsSceneResizeEvent* event);
    void init();    
    Session* createSession();
    virtual TerminalDisplay* createTerminalDisplay(Session *session);

    TerminalDisplay *m_terminalDisplay;
    Session *m_session;
    
protected slots:
    void sessionFinished();        
    
private:

};


//Maybe useful, maybe not

#ifdef __cplusplus
extern "C"
#endif
void *createTermWidget(int startnow, void *parent); 

#endif

