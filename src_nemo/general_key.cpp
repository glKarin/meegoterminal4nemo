#include "key.h"

#include "k.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QBrush>

// general key

karin::general_key::general_key(QGraphicsWidget *parent)
	: base_key(parent),
	m_repeat(false),
	m_timer(0),
	m_interval(karin::general_key::M_Repeat_Interval_MS),
	m_delay(karin::general_key::M_Repeat_Delay_MS)
{
	setObjectName(OBJECTNAME_GENERAL_KEY);
	setBorderWidth(karin::base_key::M_Border);
	setKeyType(karin::base_key::Key_General);
}

karin::general_key::~general_key()
{
	if(m_timer)
		m_timer->stop();
}

void karin::general_key::reset()
{
	setState(karin::base_key::Key_Released);
	update();
	karin::base_key::reset();
}

void karin::general_key::switch2()
{
}

void karin::general_key::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;
	QBrush brush;

	Q_UNUSED(widget);

	setupPainter(painter);
	if(m_state == karin::base_key::Key_Released)
	{
		painter->save();
		{
			brush = painter->brush();
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(karin::Key_Bg_Color_0_ub);
			painter->setBrush(brush);
			painter->fillPath(m_innerPath, brush);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_borderWidth);
			pen.setColor(karin::Key_Border_Color_0_ub);
			painter->setPen(pen);
			//painter->drawRoundedRect(m_outerRect, m_radius, m_radius);
			painter->drawPath(m_innerPath);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setColor(karin::Key_Text_Color_0_ub);
			painter->setPen(pen);
			QFont f = painter->font();
			f.setPixelSize(font().pixelSize());
			painter->setFont(f);
			painter->drawText(m_center - Font_GetStringCenter(m_label, f), m_label);
		}
		painter->restore();
	}
	else
	{
		painter->save();
		{
			brush = painter->brush();
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(karin::Key_Bg_Color_1_ub);
			painter->setBrush(brush);
			painter->fillPath(m_innerPath, brush);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_borderWidth);
			pen.setColor(karin::Key_Border_Color_1_ub);
			painter->setPen(pen);
			//painter->drawRoundedRect(m_outerRect, m_radius, m_radius);
			painter->drawPath(m_innerPath);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setColor(karin::Key_Text_Color_1_ub);
			painter->setPen(pen);
			QFont f = painter->font();
			f.setPixelSize(font().pixelSize());
			f.setBold(true);
			painter->setFont(f);
			painter->drawText(m_center - Font_GetStringCenter(m_label, f), m_label);
		}
		painter->restore();
	}
}

void karin::general_key::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if(!isValid())
		return;
	touchEvent(true);
	karin::base_key::mousePressEvent(e);
}

void karin::general_key::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	if(!isValid())
		return;
	if(!m_pressed)
		return;
	touchEvent(false);
	karin::base_key::mouseReleaseEvent(e);
}

void karin::general_key::setRepeat(bool b)
{
	if(m_repeat == b)
		return;

	m_repeat = b;
	if(m_repeat)
	{
		if(!m_timer)
			m_timer = new QTimer(this);
		m_timer->setSingleShot(false);
		m_timer->setInterval(m_interval);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
	}
	else
	{
		if(m_timer)
		{
			disconnect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
			m_timer->stop();
			m_timer->deleteLater();
			m_timer = 0;
		}
	}
}

void karin::general_key::timeoutSlot()
{
	//emit sendKeySignal(m_sym, m_modifier, false);
	emit sendKeySignal(m_sym, m_modifier, true);
}

void karin::general_key::setState(karin::base_key::Key_State state)
{
	if(m_state == state)
		return;

	switch(state)
	{
		case karin::base_key::Key_Released:
			{
				if(m_repeat)
				{
					m_timer->stop();
				}
				setBorderWidth(karin::base_key::M_Border);
				m_state = state;
			}
			break;
		case karin::base_key::Key_Pressed:
			{
				setBorderWidth(karin::base_key::M_Border * 2);
				if(m_repeat)
				{
					m_timer->start(m_delay);
				}
				m_state = state;
			}
			break;
		default:
			{
				setBorderWidth(karin::base_key::M_Border);
				m_state = karin::base_key::Key_Unused_State;
			}
			break;
	}
	update();
}

void karin::general_key::touchEvent(bool b)
{
	if(b)
	{
		m_pressed = true;
		this->setState(karin::base_key::Key_Pressed);
		emit sendKeySignal(m_sym, m_modifier, true);
	}
	else
	{
		if(m_pressed)
		{
			m_pressed = false;
			this->setState(karin::base_key::Key_Released);
			emit sendKeySignal(m_sym, m_modifier, false);
		}
	}
}

