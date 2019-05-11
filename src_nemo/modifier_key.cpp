#include "key.h"
#include "k.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QBrush>

// general key

karin::modifier_key::modifier_key(QGraphicsWidget *parent)
	: base_key(parent)
{
	setObjectName(OBJECTNAME_MODIFIER_KEY);
	setBorderWidth(karin::base_key::M_Border);
	setKeyType(karin::base_key::Key_Modifier);
}

karin::modifier_key::~modifier_key()
{
}

void karin::modifier_key::reset()
{
	setState(karin::base_key::Key_Released);
	update();
}

void karin::modifier_key::switch2()
{
}

void karin::modifier_key::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
	else if(m_state == karin::base_key::Key_Pressed)
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
	else if(m_state == karin::base_key::Key_Long_Pressed)
	{
		painter->save();
		{
			brush = painter->brush();
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(karin::Key_Bg_Color_2_ub);
			painter->setBrush(brush);
			painter->fillPath(m_innerPath, brush);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_borderWidth);
			pen.setColor(karin::Key_Border_Color_2_ub);
			painter->setPen(pen);
			//painter->drawRoundedRect(m_outerRect, m_radius, m_radius);
			painter->drawPath(m_innerPath);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setColor(karin::Key_Text_Color_2_ub);
			painter->setPen(pen);
			QFont f = painter->font();
			f.setPixelSize(font().pixelSize());
			f.setBold(true);
			f.setUnderline(true);
			painter->setFont(f);
			painter->drawText(m_center - Font_GetStringCenter(m_label, f), m_label);
		}
		painter->restore();
	}
}

void karin::modifier_key::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if(!isValid())
		return;
	touchEvent(true);
	karin::base_key::mousePressEvent(e);
}

void karin::modifier_key::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	if(!isValid())
		return;
	touchEvent(false);
	karin::base_key::mouseReleaseEvent(e);
}

void karin::modifier_key::setState(karin::base_key::Key_State state)
{
	if(m_state == state)
		return;
	switch(state)
	{
		case karin::base_key::Key_Released:
			{
				setBorderWidth(karin::base_key::M_Border);
				m_state = state;
			}
			break;
		case karin::base_key::Key_Pressed:
			{
				setBorderWidth(karin::base_key::M_Border * 1.5);
				m_state = state;
			}
			break;
		case karin::base_key::Key_Long_Pressed:
			{
				setBorderWidth(karin::base_key::M_Border * 2);
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

void karin::modifier_key::touchEvent(bool b)
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
			//this->setState(karin::base_key::Key_Released);
			emit sendKeySignal(m_sym, m_modifier, false);
			update();
		}
	}
}

