#include "label.h"
#include "k.h"

#include <QDebug>
#include <QPainter>

karin::label::label(QGraphicsWidget *parent)
	: karin::rect(parent),
	m_text(""),
	m_color(karin::label::M_Color)
{
	setObjectName(OBJECTNAME_LABEL);
	setBorderWidth(0);
}

karin::label::~label()
{
}

void karin::label::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;
	QBrush brush;

	setupPainter(painter);

	karin::rect::paint(painter, option, widget);

	painter->save();
	{
		pen = painter->pen();
		pen.setColor(m_color);
		painter->setPen(pen);
		QFont f = painter->font();
		f.setPixelSize(font().pixelSize());
		painter->setFont(f);
		painter->drawText(m_center - Font_GetStringCenter(m_text, f), m_text);
	}
	painter->restore();
}

void karin::label::setColor(const QColor &c)
{
	if(m_color != c)
	{
		m_color = c;
		update();
	}
}

void karin::label::setText(const QString &s)
{
	if(m_text != s)
	{
		m_text = s; 
		update();
	}
}
