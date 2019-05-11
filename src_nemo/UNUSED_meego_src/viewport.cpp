#include "viewport.h"

#include "k.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QTouchEvent>
#include <qnamespace.h>

karin::viewport::viewport(QGraphicsWidget *parent)
	: QObject(parent)
{
	reset();

	setObjectName(OBJECTNAME_VIEWPORT);
}

karin::viewport::~viewport()
{
}

void karin::viewport::reset()
{
	m_touchId = -1;
	m_pressed = false;
	m_grab = false;
	m_lastPos.setX(-1);
	m_lastPos.setY(-1);

	update();
}

bool karin::viewport::scroll(const QGraphicsSceneMouseEvent *e)
{
	if(!m_grab)
		return false;
	if(!e)
		return false;
	switch(e->type())
	{
		case QEvent::GraphicsSceneMousePress:
			{
				m_pressed = true;
				m_lastPos.setX(e->pos().x());
				m_lastPos.setY(e->pos().y());
			}
			break;
		case QEvent::GraphicsSceneMouseRelease:
			{
				m_pressed = false;
				m_lastPos.setX(0);
				m_lastPos.setY(0);
			}
			break;
		case QEvent::GraphicsSceneMouseMove:
			{
				if(m_pressed)
				{
					QPointF delta = e->pos() - m_lastPos;
					QPointF p = m_position - delta;
					updatePosition(p);
					m_lastPos = e->pos();
				}
				else
					return false;
			}
			break;
		default:
			return false;
	}
	return true;
}

bool karin::viewport::swipe(const QTouchEvent *event)
{
	const QTouchEvent::TouchPoint *e;
	if(!m_grab)
		return false;
	if(!event)
		return false;
	const QList<QTouchEvent::TouchPoint> &points = event->touchPoints();
	if(points.isEmpty())
		return false;

	switch(event->type())
	{
		case QEvent::TouchBegin:
			{
				if(m_touchId == -1)
				{
					e = &(points[0]);
					m_pressed = true;
					m_lastPos = e->pos();
					m_touchId = e->id();
					return true;
				}
			}
			break;
		case QEvent::TouchEnd:
		case QEvent::TouchCancel:
			{
				if(m_pressed)
				{
					K_FOREACH_CONST(QList<QTouchEvent::TouchPoint>, points)
					{
						if(itor->id() != m_touchId)
							continue;
						m_pressed = false;
						m_lastPos.setX(-1);
						m_lastPos.setY(-1);
						m_touchId = -1;
						return true;
					}
				}
			}
			break;
		case QEvent::TouchUpdate:
			{
				if(m_pressed)
				{
					K_FOREACH_CONST(QList<QTouchEvent::TouchPoint>, points)
					{
						if(itor->id() != m_touchId)
							continue;
						QPointF delta = itor->pos() - m_lastPos;
						QPointF p = m_position - delta;
						updatePosition(p);
						//qDebug()<<m_position<<m_range<<m_viewsize;
						m_lastPos = itor->pos();
						return true;
					}
				}
			}
			break;
		default:
			return false;
	}
	return false;
}

void karin::viewport::update()
{
	QGraphicsWidget *pwidget;
	pwidget = static_cast<QGraphicsWidget *>(parent());

	if(!pwidget)
		return;

	m_position.setX(0);
	m_position.setY(0);
	m_viewsize = pwidget->size();
	m_range.setTopLeft(QPointF(0, 0));
	m_range.setSize(QSizeF(m_viewsize.width(), 0));
}

void karin::viewport::updatePosition(const QPointF &p)
{
	float x;
	float y;

	x = p.x();
	y = p.y();

	if(x < 0)
		x = 0;
	else if(x > m_range.width())
		x = m_range.width();

	if(y < 0)
		y = 0;
	else if(y > m_range.height())
		y = m_range.height();

	if(x != m_position.x() || y != m_position.y())
	{
		m_position.setX(x);
		m_position.setY(y);
		emit positionChanged(m_position);
	}
}

void karin::viewport::setRange(const QRectF &r)
{ 
	m_range = r;
}

void karin::viewport::ungrabGesture(Qt::GestureType type, Qt::GestureFlags flags)
{
	Q_UNUSED(type);
	Q_UNUSED(flags);

	m_grab = false;
}

void karin::viewport::grabGestureWithCancelPolicy(Qt::GestureType type, Qt::GestureFlags flags)
{
	Q_UNUSED(type);
	Q_UNUSED(flags);

	m_grab = true;
}

