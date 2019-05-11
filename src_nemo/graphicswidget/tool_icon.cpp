#include "tool_icon.h"
#include "k.h"

#include <QDebug>
#include <QPainter>

karin::tool_icon::tool_icon(const QString &image, QGraphicsWidget *parent)
	: karin::button(parent),
	m_pixmap(0)
{
	setObjectName(OBJECTNAME_TOOL_ICON);
	setImage(image);
	setRadius(0);
	setBorderWidth(0);
}

// C++11
karin::tool_icon::tool_icon(QGraphicsWidget *parent)
	: karin::tool_icon(QString(), parent)
{
}

karin::tool_icon::~tool_icon()
{
	delete m_pixmap;
}

void karin::tool_icon::setImage(const QString &s)
{
	if(m_image != s)
	{
		m_image = s;
		if(!m_pixmap)
		{
			m_pixmap = new QPixmap;
			if(!m_pixmap->load(m_image))
			{
				m_image.clear();
			}
		}
		update();
	}
}

void karin::tool_icon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;
	QBrush brush;

	karin::button::paint(painter, option, widget);
	setupPainter(painter);
	if(m_pixmap && !m_pixmap->isNull())
	{
		painter->save();
		{
			painter->drawPixmap(boundingRect(), *m_pixmap, m_pixmap->rect());
		}
		painter->restore();
	}
#if 0
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
#endif
}

