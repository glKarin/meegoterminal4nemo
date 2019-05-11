#include "button_group.h"
#include "list_view.h"
#include "button.h"
#include "k.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsSceneResizeEvent>
#include <QDebug>

karin::button_group::button_group(QGraphicsWidget *parent)
	: karin::rect(parent),
	m_layout(new karin::list_view_layout(this)),
	m_itemWidth(karin::button_group::M_ItemWidth),
	m_itemHeight(karin::button_group::M_ItemHeight),
	m_exclusive(true)
{
	setObjectName(OBJECTNAME_BUTTON_GROUP);

	setNeedToPaint(false);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addAnchor(m_layout, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout->addAnchor(m_layout, Qt::AnchorRight, layout, Qt::AnchorRight);
	layout->addAnchor(m_layout, Qt::AnchorTop, layout, Qt::AnchorTop);
	layout->addAnchor(m_layout, Qt::AnchorBottom, layout, Qt::AnchorBottom);
	
	setLayout(layout);

	m_layout->setSpacing(0);
	relayout();

	connect(m_layout, SIGNAL(boundingRectChanged(const QRectF &)), this, SLOT(buttonBoundingRectChangeSlot(const QRectF &)));
}

karin::button_group::~button_group()
{
}

void karin::button_group::clear()
{
	m_layout->clear();

	relayout();
}

void karin::button_group::addButton(karin::button *b, int id)
{
	if(!b)
		return;
	QFont f = font();
	b->setFixedWidth(m_layout->orientation() == Qt::Horizontal ? m_itemWidth : width());
	b->setFixedHeight(m_layout->orientation() == Qt::Horizontal ? height() : m_itemHeight);
	b->setFont(f);
	m_layout->addItem(b, id);

	relayout();
}

karin::button * karin::button_group::button(int id) const
{
	return static_cast<karin::button *>(m_layout->operator[](id));
}

QList<karin::button *> karin::button_group::buttons() const
{
	QList<karin::button *> r;
	QList<QGraphicsWidget *> items = m_layout->items();
	K_FOREACH_CONST(QList<QGraphicsWidget *>, items)
	{
		r.push_back(static_cast<karin::button *>(*itor));
	}
	return r;
}

int karin::button_group::id(karin::button *b) const
{
	if(!b)
		return -1;

	return m_layout->indexOf(b);
}

void karin::button_group::removeButton(karin::button *b)
{
	if(!b)
		return;

	m_layout->removeItem(b);

	relayout();
}

void karin::button_group::removeButton(int index)
{
	m_layout->removeItem(index);

	relayout();
}

karin::button * karin::button_group::takeButton(int index)
{
	QGraphicsWidget *w;

	w = m_layout->takeItem(index);

	relayout();

	return static_cast<karin::button *>(w);
}

void karin::button_group::setItemWidth(int w)
{
	if(m_itemWidth == w)
		return;

	m_itemWidth = w;

	if(m_layout->orientation() == Qt::Horizontal)
	{
		QList<QGraphicsWidget *> actions = m_layout->items();
		K_FOREACH_CONST(QList<QGraphicsWidget *>, actions)
		{
			Widget_SetFixedWidth(*itor, m_itemWidth);
		}
	}
}

void karin::button_group::setItemHeight(int h)
{
	if(m_itemHeight == h)
		return;

	m_itemHeight = h;

	if(m_layout->orientation() == Qt::Vertical)
	{
		QList<QGraphicsWidget *> actions = m_layout->items();
		K_FOREACH_CONST(QList<QGraphicsWidget *>, actions)
		{
			Widget_SetFixedHeight(*itor, m_itemHeight);
		}
	}
}

void karin::button_group::relayout()
{
	karin::button *b;
	int len;

	if(m_exclusive)
	{
		QList<QGraphicsWidget *> actions = m_layout->items(true);
		if(actions.size() == 1)
		{
			b = static_cast<karin::button *>(actions[0]);
			b->setPositionType(karin::rect::Rect_In_Border);
		}
		else if(actions.size() > 1)
		{
			len = actions.size() - 1;
			b = static_cast<karin::button *>(actions[0]);
			b->setPositionType(m_layout->orientation() == Qt::Horizontal ? karin::rect::Rect_In_TL | karin::rect::Rect_In_BL : karin::rect::Rect_In_TL | karin::rect::Rect_In_TR);
			b = static_cast<karin::button *>(actions[len]);
			b->setPositionType(m_layout->orientation() == Qt::Horizontal ? karin::rect::Rect_In_TR | karin::rect::Rect_In_BR : karin::rect::Rect_In_BL | karin::rect::Rect_In_BR);
			for(int i = 1; i < len; i++)
			{
				b = static_cast<karin::button *>(actions[i]);
				b->setPositionType(karin::rect::Rect_in_Center);
			}
		}
		//setFixedSize(m_layout->boundingRect().size().toSize());
	}
	m_layout->invalidate();
}

void karin::button_group::setExclusive(bool b)
{
	if(m_exclusive == b)
		return;
	m_exclusive = b;
	if(m_exclusive)
	{
		m_layout->setSpacing(0);
	}
	relayout();
}

void karin::button_group::resizeEvent(QGraphicsSceneResizeEvent * event)
{
	QSizeF size = event->newSize();
	m_layout->resizeItems(size);

	karin::rect::resizeEvent(event);
}

int karin::button_group::count() const
{
#if 0
	int r;

	r = 0;
	QList<QGraphicsWidget *> items = m_layout->items();
	K_FOREACH_CONST(QList<QGraphicsWidget *>, items)
	{
		if((*itor)->isVisible())
			r++;
	}
	return r;
#else
	return m_layout->count();
#endif
}

void karin::button_group::buttonBoundingRectChangeSlot(const QRectF &r)
{
	int h = height();
	QSize s = r.size().toSize();
	setFixedSize(s.width(), qMax(h, s.height()));
}
