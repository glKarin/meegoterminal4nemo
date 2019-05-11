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

#ifndef _MTERMINAL_DISPLAY_
#define _MTERMINAL_DISPLAY_

#include "TerminalDisplay.h"

using namespace Konsole;

/**
 * Terminal display widget for MeeGo touch specific features.
 *
 * [1] Toggle buttons on the input method toolbar are used for modfiers. Their
 * functionality tries to emulate modifier functionality on the IM. See:
 * http://apidocs.meego.com/1.1/platform/html/meego-im-framework/internals.html
 * "Modifier Keys" sections describes this feature. Here are some excerpts:
 *
 *
 * There are three modifier key states:
 *
 * CLEAR: the modifier key is not active
 * LATCHED: the modifier key is active until other key is pressed and released
 * LOCKED: the modifier key is LATCHED until the same modifier key is pressed and released
 * The diagram below describes the transitions of the modifier key states:
 *
 *        +-------------------------------------+
 *        |                                     |
 *        |                                     ^
 *        V                                     |
 *      NORMAL +-(pressed)-> LATCHED -(non-modifier key is pressed+released)
 *        ^    |                                ^
 *        |    |                                |
 *        |    |                                |
 *        |    |                                |
 *        |    +-(pressed+released) -> LATCHED -+
 *        |                                     |
 *        |                                     V
 *        |                                     |
 *        |                      (same modifier key is pressed+released)
 *        |                                     |
 *        |                                     V
 *        |                                   LOCKED
 *        |                                     |
 *        |                                     |
 *        |                      (same modifier key is pressed+released)
 *        |                                     |
 *        |                                     |
 *        +-------------------------------------+
 *
 */

#include "overlay_toolbar.h"

#include <QAction>

namespace karin
{
	class toolbar;
};

class MTerminalDisplay : public TerminalDisplay
{
    Q_OBJECT
public:

    MTerminalDisplay(QGraphicsWidget *parent=0);
    ~MTerminalDisplay();
		/*
    Toolbars_t toolbars() const { return Toolbar::getAllToolbars(); }
		*/
    const karin::toolbar* activeToolbar() const // returns 0 if there are no toolbars
        { return m_toolbar; }
    void setNextActiveToolbar();
    void setPrevActiveToolbar();
    int setActiveToolbar(QString fileName);
    int activeToolbarIndex() const { return m_activeToolbarIndex; }

    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

    void enableSelectionMode() { setSelectionModeEnabled(true); };
    void disableSelectionMode() { setSelectionModeEnabled(false); };
		void bindToolbar(karin::toolbar *bar);

public slots:
    void setActiveToolbarIndex(int index, bool force = false);
    void updateEditorToolbarPosition();
		void recvKeySlot(unsigned code, unsigned modifier, bool pressed);
		void setGrabFocus(bool enable);
		void setInputMethodNoPredictive(bool b);

protected:
    void showEditorToolbar();
    void hideEditorToolbar();

    QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
    void inputMethodEvent (QInputMethodEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void resizeEvent(QGraphicsSceneResizeEvent* event);

    QString getCtrlifiedText(const QString& plainText);

    void updateModifierState(Qt::KeyboardModifier modifier);
    void clearLatchedModifiers();
    bool modifierIsActive(Qt::KeyboardModifier modifier) const;
    bool ctrlModifierIsActive() const
        { return modifierIsActive(Qt::ControlModifier); }
    bool altModifierIsActive() const
        { return modifierIsActive(Qt::AltModifier); }
    bool anyModifierIsActive() const
        { return m_latchedModifiers || m_lockedModifiers; }
    void toolbarBtnSetPressed(const QString &name, bool pressed) const;
    void toolbarBtnSetPressedExt(const QString &name, bool pressed) const;
    void toolbarBtnTextUnderline(const QString &name, bool underline) const;

    void mousePressEvent(QGraphicsSceneMouseEvent* ev);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* ev);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ev);

    void setSelectionModeEnabled(bool enabled)
        { m_selectionModeEnabled = enabled; }

private slots:
    void onClipboardDataChanged();
    void clearClipboardContents();
    void reAppearEditorToolbar();

private:
    Qt::KeyboardModifiers m_latchedModifiers; // see [1]
    Qt::KeyboardModifiers m_lockedModifiers;

    //Toolbars_t m_toolbars;
    int m_activeToolbarIndex; // index into m_toolbars

    // button names as specified in the input method toolbar XML file
    QString m_btnNameCtrl;
    QString m_btnNameAlt;
    QStringList m_btnNames;

    // for conversion of modifiers to button names and vice versa
    QHash<Qt::KeyboardModifier, QString> m_mod2BtnName;
    QHash<QString, Qt::KeyboardModifier> m_btnName2Mod;

    QScopedPointer<MEditorToolbar> m_editorToolbar;

		QAction *m_copyAction;
		QAction *m_pasteAction;
		QAction *m_clearAction; // clears clipboard contents

    QPointF m_mouseReleasePos;
    bool m_selectionModeEnabled;
    bool m_restoreEditorToolbar; // restore it if it was auto-hidden
		karin::toolbar *m_toolbar;

		bool m_grabFocus;
		bool m_imNoPredictive_sailfish;
};

#endif
