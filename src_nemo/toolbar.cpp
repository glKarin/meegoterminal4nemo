#include "toolbar.h"

#include "key.h"
#include "k.h"

#include "MTerminalDisplay.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsScene>
#include <QTimer>


karin::toolbar::toolbar(QGraphicsWidget *parent)
	: karin::list_view(parent),
	m_terminalDisplay(0),
	m_activeToolbarIndex(-1)
{
	setObjectName(OBJECTNAME_TOOLBAR);
	setBgColor(QColor(255, 255, 255));
	setAutoFixedSize(true);
	setFixedHeight(M_FixedHeight);

	updateToolbarDirectory();
}

karin::toolbar::~toolbar()
{
}

void karin::toolbar::addAction(karin::base_key *action, int i)
{
	if(!action)
		return;

	addItem(action, i);
	action->setFixedWidth(64);
	QFont f = font();
	f.setPixelSize(24);
	action->setFont(f);
	connect(action, SIGNAL(sendKeySignal(unsigned, unsigned, bool)), this, SLOT(sendKeySlot(unsigned, unsigned, bool)));
}

void karin::toolbar::setFixedHeight(int h)
{
	m_fixedHeight = h;
	setMinimumHeight(m_fixedHeight);
	setMaximumHeight(m_fixedHeight);
	setPreferredHeight(m_fixedHeight);
	K_SET_HEIGHT(this, m_fixedHeight);
}

karin::base_key * karin::toolbar::getAction(const QString &name)
{
	karin::base_key *key;
	QList<QGraphicsWidget *> actions = items();
	K_FOREACH_CONST(QList<QGraphicsWidget *>, actions)
	{
		key = static_cast<base_key *>(*itor);
		if(key->name() == name)
			return key;
	}
	return 0;
}

void karin::toolbar::sendKeySlot(unsigned key, unsigned modifier, bool pressed)
{
	if(m_terminalDisplay)
	{
		//QKeyEvent key_event(pressed ? QEvent::KeyPress : QEvent::KeyRelease, key, static_cast<Qt::KeyboardModifier>(modifier), QString(key)/*, const QString &text = QString(), bool autorep = false, ushort count = 1*/);
		//QApplication::sendEvent(m_terminalDisplay, &key_event);
		//m_terminalDisplay->setFocus(Qt::MouseFocusReason);
		(static_cast<MTerminalDisplay *>(m_terminalDisplay))->recvKeySlot(key, modifier, pressed);
	}
	emit sendKeySignal(key, modifier, pressed);
}

void karin::toolbar::bindTerminalDisplay(QGraphicsWidget *term)
{
	if(m_terminalDisplay != term)
	{
		m_terminalDisplay = term;
		reset();
	}
	/*
		 if(m_terminalDisplay)
		 connect(this, SIGNAL(sendKeySignal(unsigned, unsigned, bool)), m_terminalDisplay, SLOT(recvKeySlot(unsigned, unsigned, bool)));
		 */
}

void karin::toolbar::setToolKeyState(const QString &name, unsigned state)
{
	base_key *key;

	if(m_activeToolbarIndex == -1)
		return;

	key = getAction(name);
	if(!key)
		return;

	key->setState(static_cast<karin::base_key::Key_State>(state));
}

void karin::toolbar::reset()
{
	karin::base_key *key;
	QList<QGraphicsWidget *> actions = items();
	K_FOREACH_CONST(QList<QGraphicsWidget *>, actions)
	{
		key = static_cast<base_key *>(*itor);
		key->reset();
	}
}
