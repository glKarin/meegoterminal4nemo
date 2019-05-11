#include "list_view.h"
#include "k.h"

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QDebug>

// #define LIST_VIEW_LAYOUT() (static_cast<list_view_layout *>(m_content))

// list_view_layout

	karin::list_view_layout::list_view_layout(QGraphicsWidget *parent)
: QGraphicsWidget(parent),
	m_layout(new QGraphicsLinearLayout(this))
{
	setObjectName(OBJECTNAME_LIST_VIEW_LAYOUT);
	m_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(m_layout);

}

karin::list_view_layout::~list_view_layout()
{
}

bool karin::list_view_layout::isExists(const QGraphicsWidget *action) const
{
	if(!action)
		return true;

	K_FOREACH_CONST(QList<QGraphicsWidget *>, m_actions)
	{
		if(*itor == action)
			return true;
	}
	return false;
}

QRectF karin::list_view_layout::boundingRect() const
{
	QRectF r = caleBoundingRect();
	if(r != m_lastBoundingRect)
	{
		karin::list_view_layout *_self = const_cast<karin::list_view_layout *>(this);
		_self->m_lastBoundingRect = r;
		emit _self->boundingRectChanged(m_lastBoundingRect);
	}
	return m_lastBoundingRect;
}

QRectF karin::list_view_layout::caleBoundingRect() const
{
	QRectF r;

	K_FOREACH_CONST(QList<QGraphicsWidget *>, m_actions)
	{
		//if((*itor)->isVisible())
			// r |= (*itor)->geometry();
			r |= (*itor)->boundingRect().translated((*itor)->pos());
	}
	return r;
}

void karin::list_view_layout::addItem(QGraphicsWidget *action, int i)
{
	if(!action)
		return;
	if(isExists(action))
		return;
	action->setParent(this);
	action->setParentItem(this);
	m_layout->insertItem(i, action);
	m_actions.push_back(action);

	boundingRect();
}

QGraphicsWidget * karin::list_view_layout::takeItem(int i)
{
	QGraphicsWidget *k;

	k = static_cast<QGraphicsWidget *>(m_layout->itemAt(i));
	if(k)
	{
		k->setParent(0);
		k->setParentItem(0);
		m_actions.removeOne(k);
		m_layout->removeItem(k);

		boundingRect();
	}

	return k;
}

QGraphicsWidget * karin::list_view_layout::takeItem(QGraphicsWidget *w)
{
	if(!isExists(w))
		return 0;

	w->setParent(0);
	w->setParentItem(0);
	m_actions.removeOne(w);
	m_layout->removeItem(w);

	boundingRect();

	return w;
}

void karin::list_view_layout::removeItem(int i)
{
	QGraphicsWidget *k;

	k = takeItem(i);
	if(k)
		k->deleteLater();
}

void karin::list_view_layout::removeItem(QGraphicsWidget *w)
{
	QGraphicsWidget *k;
	if(!w)
		return;

	k = takeItem(w);
	if(k)
		k->deleteLater();
}

void karin::list_view_layout::clear()
{
	K_FOREACH(QList<QGraphicsWidget *>, m_actions)
	{
		m_layout->removeItem(*itor);
		(*itor)->deleteLater();
	}
	m_actions.clear();

	boundingRect();
	//k reset();
}

const QGraphicsWidget * karin::list_view_layout::getItem(int i) const
{
	return const_cast<list_view_layout *>(this)->operator[](i);
}

const QGraphicsWidget * karin::list_view_layout::getItem(const QString &name) const
{
	return const_cast<list_view_layout *>(this)->operator[](name);
}

QGraphicsWidget * karin::list_view_layout::operator[] (int i)
{
	return static_cast<QGraphicsWidget *>(m_layout->itemAt(i));
}

QGraphicsWidget * karin::list_view_layout::operator[] (const QString &name)
{
	QGraphicsWidget *k;

	K_FOREACH(QList<QGraphicsWidget *>, m_actions)
	{
		k = static_cast<QGraphicsWidget *>(*itor);
		if(k && k->objectName() == name)
			return k;
	}
	return 0;
}

karin::list_view_layout & karin::list_view_layout::operator<< (QGraphicsWidget *action)
{
	addItem(action);
	return *this;
}

void karin::list_view_layout::setOrientation(Qt::Orientation o)
{
	m_layout->setOrientation(o);
}

Qt::Orientation karin::list_view_layout::orientation() const
{
	return m_layout->orientation();
}

int karin::list_view_layout::count() const 
{
	return m_layout->count(); 
	// return m_actions.size(); 
}

QList<QGraphicsWidget *> karin::list_view_layout::items(bool b)
{
	if(!b)
		return m_actions;
	else
	{
		QList<QGraphicsWidget *> r;
		K_FOREACH(QList<QGraphicsWidget *>, m_actions)
		{
			if((*itor)->isVisible())
				r.push_back(*itor);
		}
		return r;
	}
}

void karin::list_view_layout::invalidate()
{
	m_layout->invalidate();
}

void karin::list_view_layout::setSpacing(int s)
{
	m_layout->setSpacing(s);
}

int karin::list_view_layout::spacing() const
{
	return m_layout->spacing();
}

void karin::list_view_layout::resizeItems(const QSizeF &size)
{
	resize(size);
	
	K_FOREACH_CONST(QList<QGraphicsWidget *>, m_actions)
	{
		if(orientation() == Qt::Horizontal)
		{
			Widget_SetFixedHeight(*itor, size.height());
		}
		else
		{
			Widget_SetFixedWidth(*itor, size.width());
		}
	}
}

int karin::list_view_layout::indexOf(const QGraphicsWidget *w) const
{
	if(!w)
		return -1;
	for(int i = 0; i < m_actions.size(); i++)
	{
		if(m_actions.at(i) == w)
			return i;
	}
	return -1;
}



// list_view

	karin::list_view::list_view(QGraphicsWidget *parent)
: karin::pannable(parent),
	m_content(new list_view_layout(this)),
	m_bgColor(Qt::white),
	m_autoFixedSize(false)
{
	setObjectName(OBJECTNAME_LIST_VIEW);
	setPannableWidget(m_content);
	setOrientation(Qt::Horizontal);
}

karin::list_view::~list_view()
{
}

void karin::list_view::addItem(QGraphicsWidget *action, int i)
{
	if(!action)
		return;
	m_content->addItem(action, i);
	if(m_autoFixedSize)
	{
		QSizeF size = boundingRect().size(); //
		if(orientation() == Qt::Horizontal)
		{
			Widget_SetFixedHeight(action, size.height());
		}
		else
		{
			Widget_SetFixedWidth(action, size.width());
		}
		action->update();
	}
}

QGraphicsWidget * karin::list_view::takeItem(int i)
{
	return m_content->takeItem(i);
}

QGraphicsWidget * karin::list_view::takeItem(QGraphicsWidget *w)
{
	if(!w)
		return 0;

	return m_content->takeItem(w);
}

void karin::list_view::removeItem(int i)
{
	m_content->removeItem(i);
}

void karin::list_view::clear()
{
	m_content->clear();
	reset();
}

const QGraphicsWidget * karin::list_view::getItem(int i) const
{
	return const_cast<list_view *>(this)->operator[](i);
}

const QGraphicsWidget * karin::list_view::getItem(const QString &name) const
{
	return const_cast<list_view *>(this)->operator[](name);
}

QGraphicsWidget * karin::list_view::operator[] (int i)
{
	return m_content->operator[](i);
}

QGraphicsWidget * karin::list_view::operator[] (const QString &name)
{
	return m_content->operator[](name);
}

karin::list_view & karin::list_view::operator<< (QGraphicsWidget *action)
{
	addItem(action);
	return *this;
}

void karin::list_view::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;

	Q_UNUSED(widget);

	setupPainter(painter);
	painter->save();
	{
		painter->fillRect(boundingRect(), m_bgColor);
		pen.setColor(m_borderColor);
		pen.setWidth(1);
		painter->setPen(pen);
		painter->drawRect(boundingRect());
	}
	painter->restore();
}

void karin::list_view::setOrientation(Qt::Orientation o)
{
	m_content->setOrientation(o);
	resizeEvent(0);
}

Qt::Orientation karin::list_view::orientation() const
{
	return m_content->orientation();
}

int karin::list_view::count() const 
{
	return m_content->count(); 
}

QList<QGraphicsWidget *> karin::list_view::items(bool b)
{
	return m_content->items(b);
}

void karin::list_view::invalidate()
{
	m_content->invalidate();
}

void karin::list_view::setSpacing(int s)
{
	m_content->setSpacing(s);
}

int karin::list_view::spacing() const
{
	return m_content->spacing();
}

void karin::list_view::setAutoFixedSize(bool b)
{
	if(m_autoFixedSize == b)
		return;
	m_autoFixedSize = b;
	resizeEvent(0);
}

void karin::list_view::resizeEvent(QGraphicsSceneResizeEvent * event)
{
	QSizeF size = boundingRect().size(); //
	/*
		 if(orientation() == Qt::Horizontal)
		 {
		 Widget_SetFixedHeight(m_content, size.height());
		 }
		 else
		 {
		 Widget_SetFixedWidth(m_content, size.width());
		 }
		 */

	if(m_autoFixedSize)
	{
		m_content->resizeItems(size);
	}

	karin::pannable::resizeEvent(event);
}

int karin::list_view::indexOf(const QGraphicsWidget *w) const
{
	return m_content->indexOf(w);
}

void karin::list_view::setColor(const QColor &color)
{
	if(m_bgColor != color)
	{
		m_bgColor = color;
		update();
	}
}

void karin::list_view::removeItem(QGraphicsWidget *w)
{
	if(!w)
		return;

	m_content->removeItem(w);
}
