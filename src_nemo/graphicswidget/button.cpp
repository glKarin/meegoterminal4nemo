#include "button.h"
#include "k.h"

#include <QDebug>
#include <QPainter>

karin::button::button(const QString &text, QGraphicsWidget *parent)
	: karin::rect(parent),
	m_text(text),
	m_checked(false),
	m_checkable(true),
	m_autoRelease(true),
	m_clickable(true)
{
	setObjectName(OBJECTNAME_BUTTON);
	setNeedToPaint(false);
	setRadius(karin::button::M_Radius);
}

// C++11
karin::button::button(QGraphicsWidget *parent)
	: karin::button(QString(), parent)
{
}

karin::button::~button()
{
}

void karin::button::setText(const QString &s)
{
	if(m_text != s)
	{
		m_text = s;
		update();
	}
}

void karin::button::setCheckable(bool b)
{
	if(m_checkable != b)
	{
		m_checkable = b;
		update();
	}
}

void karin::button::cancel()
{
	if(m_autoRelease)
		m_checked = false;
	update();
}

void karin::button::reset()
{
	m_checked = false;
	update();
}

void karin::button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;
	QBrush brush;

	Q_UNUSED(widget);

	setupPainter(painter);
	if(m_checkable)
	{
		if(m_checked)
		{
			painter->save();
			{
				brush = painter->brush();
				brush.setStyle(Qt::SolidPattern);
				brush.setColor(karin::Button_Bg_Color_1_ub);
				painter->setBrush(brush);
				painter->fillPath(m_innerPath, brush);
			}
			painter->restore();

			painter->save();
			{
				pen = painter->pen();
				pen.setWidth(m_borderWidth);
				pen.setColor(karin::Button_Border_Color_1_ub);
				painter->setPen(pen);
				//painter->drawRoundedRect(m_outerRect, m_radius, m_radius);
				painter->drawPath(m_innerPath);
			}
			painter->restore();

			painter->save();
			{
				pen = painter->pen();
				pen.setColor(karin::Button_Text_Color_1_ub);
				painter->setPen(pen);
				QFont f = painter->font();
				f.setPixelSize(font().pixelSize());
				f.setBold(true);
				painter->setFont(f);
				painter->drawText(m_center - Font_GetStringCenter(m_text, f), m_text);
			}
			painter->restore();
		}
		else
		{
			painter->save();
			{
				brush = painter->brush();
				brush.setStyle(Qt::SolidPattern);
				brush.setColor(karin::Button_Bg_Color_0_ub);
				painter->setBrush(brush);
				painter->fillPath(m_innerPath, brush);
			}
			painter->restore();

			painter->save();
			{
				pen = painter->pen();
				pen.setWidth(m_borderWidth);
				pen.setColor(karin::Button_Border_Color_0_ub);
				painter->setPen(pen);
				//painter->drawRoundedRect(m_outerRect, m_radius, m_radius);
				painter->drawPath(m_innerPath);
			}
			painter->restore();

			painter->save();
			{
				pen = painter->pen();
				pen.setColor(karin::Button_Text_Color_0_ub);
				painter->setPen(pen);
				QFont f = painter->font();
				f.setPixelSize(font().pixelSize());
				painter->setFont(f);
				painter->drawText(m_center - Font_GetStringCenter(m_text, f), m_text);
			}
			painter->restore();
		}
	}
	else
	{
		painter->save();
		{
			brush = painter->brush();
			brush.setStyle(Qt::SolidPattern);
			brush.setColor(karin::Button_Bg_Color_2_ub);
			painter->setBrush(brush);
			painter->fillPath(m_innerPath, brush);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_borderWidth);
			pen.setColor(karin::Button_Border_Color_2_ub);
			painter->setPen(pen);
			//painter->drawRoundedRect(m_outerRect, m_radius, m_radius);
			painter->drawPath(m_innerPath);
		}
		painter->restore();

		painter->save();
		{
			pen = painter->pen();
			pen.setColor(karin::Button_Text_Color_2_ub);
			painter->setPen(pen);
			QFont f = painter->font();
			f.setPixelSize(font().pixelSize());
			f.setBold(true);
			painter->setFont(f);
			painter->drawText(m_center - Font_GetStringCenter(m_text, f), m_text);
		}
		painter->restore();
	}
}

void karin::button::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if(m_clickable)
		touchEvent(true);
	karin::rect::mousePressEvent(e);
}

void karin::button::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	touchEvent(false);
	karin::rect::mouseReleaseEvent(e);
}

void karin::button::setChecked(bool b)
{
	if(!m_checkable)
		return;
	if(m_checked == b)
		return;
	m_checked = b;
	emit toggled(m_checked);
	update();
}

void karin::button::touchEvent(bool arg)
{
	bool b;

	if(!m_checkable)
		return;
	if(!m_autoRelease && arg)
		return;

	b = m_autoRelease ? arg : !m_checked;
	setChecked(b);
	emit b ? pressed() : released();
	emit clicked(b);
}

void karin::button::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	karin::rect::resizeEvent(event);
}

void karin::button::setAutoRelease(bool b)
{
	if(m_autoRelease == b)
		return;
	m_autoRelease = b;
	reset();
}

void karin::button::setClickable(bool b)
{
	if(m_clickable == b)
		return;
	m_clickable = b;
	reset();
}
