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

#ifndef _MTERM_WIDGET
#define _MTERM_WIDGET

#include "qgraphicstermwidget.h"

class QSwipeGesture;
class QPinchGesture;
class QGestureEvent;

class MTerminalDisplay;

namespace karin
{
	class scroll_view;
	class tab_group;
	class terminal;
}

/**
 * MeeGo touch terminal widget.
 *
 * This widget adds meegotouch based input method, copy/paste, panning,
 * support to the terminal widget.
 */
class MTermWidget : public QGraphicsTermWidget
{
    Q_OBJECT
public:
    MTermWidget(int startnow = 1, //start shell programm immediatelly
                QGraphicsWidget *parent = 0, qint64 id = -1);

    ~MTermWidget();

    void setColorScheme(int scheme);
    void startShellProgram();
		void setTranslucency(bool b);
		void setSuppression(bool b);
		void clearScreen();
		void resetScreen();
		MTerminalDisplay * terminalDisplay();

public Q_SLOTS:
    void toggleSelectionMode(bool selection);
    void toggleSelectionModeWithBanner(bool selection, qint64 id);
		void doTitleChanged();
    void onInputMethodAreaChanged();

Q_SIGNALS:
		void selectionModeChanged(bool enabled, qint64 id);
		void titleChanged(const QString &title, qint64 id);
		void sessionFinished(qint64 id);

protected:
    TerminalDisplay* createTerminalDisplay(Session *session);
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    bool event(QEvent *event);

private:
    void showBanner(const QString &title) const;
    bool gestureEvent(QGestureEvent *event);
    void pinchTriggered(QPinchGesture*);
    void swipeTriggered(QSwipeGesture*);

    void construct(int startnow);

    void readSettings();

    //void setupMenu();
    void enableGestures();
    void disableGestures();

    /* Last scroll of display history set by this widget due to viewport
     panning. It is used to avoid the rounding error when converting from lines
     into the viewport position.
     
     Details: viewportPositionChanged is called by viewport whenever position
     changes and it can set position to a number which is not a multiplier of
     PANM. But since scroll can be set only in integer lines, division will
     result in a decimal number with a fraction, which will be rounded
     off. Call to viewportPositionChanged will eventually result in a call
     updateViewport will be called with the same currentLine.  This in turn
     will set position to a number that is a full integer multiplier of
     PANM. So this will result in viewport position bouncing back and forth: a
     very weird panning artifacts. */
    int m_lastSetCurrentLine;

    int m_colorScheme; // used color scheme

    // with default theme navigation bar is hidden by the VKB so custom
    // button is used to invoke the menu
    bool m_selectionButton;

    MTerminalDisplay* m_display; // casted m_terminalDisplay
		qint64 mwId;
		bool autoSelectionEnabled;
		bool userDisabledAutoSelection;
		qreal prevPosY;
		qreal lastTotalScaleFactor;

		karin::scroll_view *m_viewport;

		friend class karin::terminal;
		friend class karin::tab_group;

private slots:
    void updateViewport(int currentLine, int maxLines);
    void viewportPositionChanged(const QPointF &position);
    //void onInputMethodAreaChanged(const QRect &);
    void displayFontChanged() const;
		void doFinished();

};

#endif
