#include "menu_item.h"

#include "k.h"
#include "button.h"

#include <QDebug>
#include <QPainter>
#include <QGraphicsAnchorLayout>

// abstract_menu_item
karin::abstract_menu_item::abstract_menu_item(QGraphicsWidget *parent)
	: karin::rect(parent)
{
	setObjectName(OBJECTNAME_ABSTRACT_MENU_ITEM);
	setNeedToPaint(false);
}

karin::abstract_menu_item::~abstract_menu_item()
{
}



// menu_item

karin::menu_item::menu_item(QGraphicsWidget *parent)
	: karin::abstract_menu_item(parent),
	m_btn(new karin::button(this))
{
	setObjectName(OBJECTNAME_MENU_ITEM);
	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addAnchor(m_btn, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout->addAnchor(m_btn, Qt::AnchorRight, layout, Qt::AnchorRight);
	layout->addAnchor(m_btn, Qt::AnchorTop, layout, Qt::AnchorTop);
	layout->addAnchor(m_btn, Qt::AnchorBottom, layout, Qt::AnchorBottom);
	setLayout(layout);
	setFixedHeight(80);
	connect(m_btn, SIGNAL(clicked(bool)), this, SLOT(clickedSlot(bool)));

}

karin::menu_item::~menu_item()
{
}

void karin::menu_item::clickedSlot(bool b)
{
	emit clicked(b);
	if(m_btn->autoRelease())
	{
		if(!b)
		emit triggered(m_name, m_value);
	}
	else
	{
		if(b)
			emit triggered(m_name, m_value);
	}
}

void karin::menu_item::setText(const QString &text)
{
	m_btn->setText(text);
}

bool karin::menu_item::isOn() const
{
	return m_btn->isChecked();
}

void karin::menu_item::setOn(bool b)
{
	m_btn->setChecked(b);
}

void karin::menu_item::reset()
{
	m_btn->reset();
}

void karin::menu_item::touchEvent(bool on)
{
	m_btn->touchEvent(on);
}

void karin::menu_item::cancel()
{
	m_btn->cancel();
}

void karin::menu_item::setAutoRelease(bool b)
{
	m_btn->setAutoRelease(b);
}

// menu_item_switcher

karin::menu_item_switcher::menu_item_switcher(QGraphicsWidget *parent)
	: karin::menu_item(parent),
	m_on(false),
	m_ellipseBorderWidth(karin::menu_item_switcher::M_EllipseBorderWidth),
	m_ellipseColor(QColor::fromRgbF(0.0, 0.0, 1.0))
{
	setObjectName(OBJECTNAME_MENU_ITEM_SWITCHER);
	connect(this, SIGNAL(clicked(bool)), this, SLOT(clickedSlot(bool)));
	resizeEvent(0);

}

karin::menu_item_switcher::~menu_item_switcher()
{
}

void karin::menu_item_switcher::clickedSlot(bool b)
{
	if(!b)
		setOn(!m_on);
}

void karin::menu_item_switcher::setOn(bool on)
{
	if(m_on != on)
	{
		m_on = on;
		setValue(m_on);
		emit triggered(m_name, m_value);
		emit valueChanged(m_on);
		update();
	}
}

void karin::menu_item_switcher::reset()
{
	m_on = false;
	menu_item::reset();
}

void karin::menu_item_switcher::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	menu_item::paint(painter, option, widget);

	QPen pen;
	QBrush brush;

	setupPainter(painter);
	if(m_on)
	{
		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_ellipseBorderWidth);
			pen.setColor(m_ellipseColor);
			painter->setPen(pen);
			painter->drawEllipse(m_ellipseCenter, m_ellipseRadius, m_ellipseRadius);
		}
		painter->restore();

		painter->save();
		{
			QPainterPath path;
			int w = m_ellipseRadius - m_ellipseBorderWidth;
			path.addEllipse(m_ellipseCenter, w, w);
			brush = painter->brush();
			brush.setColor(m_ellipseColor);
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			pen = painter->pen();
			pen.setColor(m_ellipseColor);
			painter->drawPath(path);
		}
		painter->restore();
	}
	else
	{
		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_ellipseBorderWidth);
			pen.setColor(m_ellipseColor);
			painter->setPen(pen);
			painter->drawEllipse(m_ellipseCenter, m_ellipseRadius, m_ellipseRadius);
		}
		painter->restore();
	}
}

void karin::menu_item_switcher::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	karin::menu_item::resizeEvent(event);
	QSizeF sizef = boundingRect().size();
	m_ellipseRadius = sizef.height() / 2 / 2;
	m_ellipseCenter.setX(sizef.width() - m_ellipseRadius * 2 - m_ellipseBorderWidth);
	m_ellipseCenter.setY(sizef.height() / 2);
}


// menu_item_checkbox

karin::menu_item_checkbox::menu_item_checkbox(QGraphicsWidget *parent)
	: karin::menu_item(parent),
	m_on(false),
	m_ellipseBorderWidth(karin::menu_item_checkbox::M_EllipseBorderWidth),
	m_ellipseColor(QColor::fromRgbF(0.0, 0.0, 1.0))
{
	setObjectName(OBJECTNAME_MENU_ITEM_CHECKBOX);
	connect(this, SIGNAL(clicked(bool)), this, SLOT(clickedSlot(bool)));
	resizeEvent(0);

}

karin::menu_item_checkbox::~menu_item_checkbox()
{
}

void karin::menu_item_checkbox::clickedSlot(bool b)
{
	if(!b)
		setOn(!m_on);
}

void karin::menu_item_checkbox::setOn(bool on)
{
	if(m_on != on)
	{
		m_on = on;
		setValue(m_on);
		emit triggered(m_name, m_value);
		emit valueChanged(m_on);
		update();
	}
}

void karin::menu_item_checkbox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	menu_item::paint(painter, option, widget);

	QPen pen;
	QBrush brush;

	setupPainter(painter);
	if(m_on)
	{
		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_ellipseBorderWidth);
			pen.setColor(m_ellipseColor);
			painter->setPen(pen);
			painter->drawEllipse(m_ellipseCenter, m_ellipseRadius, m_ellipseRadius);
		}
		painter->restore();

		painter->save();
		{
			QPainterPath path;
			int w = m_ellipseRadius - m_ellipseBorderWidth;
			path.addEllipse(m_ellipseCenter, w, w);
			brush = painter->brush();
			brush.setColor(m_ellipseColor);
			brush.setStyle(Qt::SolidPattern);
			painter->setBrush(brush);
			pen = painter->pen();
			pen.setColor(m_ellipseColor);
			painter->drawPath(path);
		}
		painter->restore();
	}
	else
	{
		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_ellipseBorderWidth);
			pen.setColor(m_ellipseColor);
			painter->setPen(pen);
			painter->drawEllipse(m_ellipseCenter, m_ellipseRadius, m_ellipseRadius);
		}
		painter->restore();
	}
}

void karin::menu_item_checkbox::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	karin::menu_item::resizeEvent(event);
	QSizeF sizef = boundingRect().size();
	m_ellipseRadius = sizef.height() / 2 / 2;
	m_ellipseCenter.setX(m_ellipseRadius + m_ellipseBorderWidth);
	m_ellipseCenter.setY(sizef.height() / 2);
}

void karin::menu_item_checkbox::reset()
{
	m_on = false;
	menu_item::reset();
}
