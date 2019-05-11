#include "scroll_view.h"

#include "k.h"

#include <QGraphicsSceneMouseEvent>
#include <QTouchEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>

// scroll_bar

karin::scroll_bar::scroll_bar(QGraphicsWidget *parent)
	: karin::rect(parent),
	m_orientation(Qt::Vertical),
	m_per(1.0f),
	m_fullLength(1.0),
	m_curLength(1.0),
	m_lineVisible(karin::scroll_bar::M_Line_Visible),
	m_lineWidth(karin::scroll_bar::M_Line_Width),
	m_lineColor(static_cast<Qt::GlobalColor>(karin::scroll_bar::M_Line_Color)),
	m_barRadius(0),
	m_barWidth(karin::scroll_bar::M_Bar_Width),
	m_barColor(static_cast<Qt::GlobalColor>(karin::scroll_bar::M_Bar_Color))
{
	setObjectName(OBJECTNAME_SCROLL_BAR);
	setNeedToPaint(false);

	setupLine();
	setupBar();
}

karin::scroll_bar::~scroll_bar()
{
}

void karin::scroll_bar::setBarColor(const QColor &c)
{
	if(m_barColor == c)
		return;
	m_barColor = c;
	update();
}

void karin::scroll_bar::setLineColor(const QColor &c)
{
	if(m_lineColor == c)
		return;
	m_lineColor = c;
	update();
}

void karin::scroll_bar::setLineVisible(bool b)
{
	if(m_lineVisible == b)
		return;
	m_lineVisible = b;
	update();
}

void karin::scroll_bar::setFullLength(qreal len)
{
	if(m_fullLength == len)
		return;
	m_fullLength = len;
	setupBar();
}

void karin::scroll_bar::setCurLength(qreal len)
{
	if(m_curLength == len)
		return;
	m_curLength = len;
	setupBar();
}

void karin::scroll_bar::setupLine()
{
	QSizeF s = size();
	if(m_orientation == Qt::Horizontal)
	{
		m_line.setP1(QPoint(0, s.height() / 2));
		m_line.setP2(QPoint(s.width(), s.height() / 2));

		m_barRadius = s.height() / 2;
	}
	else
	{
		m_line.setP1(QPoint(s.width() / 2, 0));
		m_line.setP2(QPoint(s.width() / 2, s.height()));

		m_barRadius = s.width() / 2;
	}
	update();
}

void karin::scroll_bar::setupBar()
{
	qreal len;
	QSizeF s = size();

	if(m_orientation == Qt::Horizontal)
	{
		len = m_curLength * s.width() / m_fullLength;
		m_barRect.setTopLeft(QPointF(qMax(qreal(0), m_per * (s.width() - len)), 0));
		m_barRect.setWidth(qMax(len, qreal(m_barWidth)));
		m_barRect.setHeight(s.height());
	}
	else
	{
		len = m_curLength * s.height() / m_fullLength;
		m_barRect.setTopLeft(QPointF(0, qMax(qreal(0), m_per * (s.height() - len))));
		m_barRect.setWidth(s.width());
		m_barRect.setHeight(qMax(len, qreal(m_barWidth)));
	}

	QPainterPath path;

	path.moveTo(m_barRect.topRight() - QPointF(m_barRadius, 0));
	path.lineTo(m_barRect.topLeft() + QPointF(m_barRadius, 0));
	path.quadTo(m_barRect.topLeft(), m_barRect.topLeft() + QPointF(0, m_barRadius));
	path.lineTo(m_barRect.bottomLeft() + QPointF(0, -m_barRadius));
	path.quadTo(m_barRect.bottomLeft(), m_barRect.bottomLeft() + QPointF(m_barRadius, 0));
	path.lineTo(m_barRect.bottomRight() - QPointF(m_barRadius, 0));
	path.quadTo(m_barRect.bottomRight(), m_barRect.bottomRight() + QPointF(0, -m_barRadius));
	path.lineTo(m_barRect.topRight() + QPointF(0, m_barRadius));
	path.quadTo(m_barRect.topRight(), m_barRect.topRight() + QPointF(-m_barRadius, -0));

	m_barPath = path;

	update();
}

void karin::scroll_bar::setRange(qreal full, qreal cur)
{
	if(m_fullLength == full && m_curLength == cur)
		return;
	m_fullLength = full;
	m_curLength = cur;
	setupBar();
}

void karin::scroll_bar::setPer(float p)
{
	if(m_per == p)
		return;
	m_per = p;
	setupBar();
}

void karin::scroll_bar::setOrientation(Qt::Orientation o)
{
	if(m_orientation == o)
		return;
	m_orientation = o;
	setupLine();
	setupBar();
}

void karin::scroll_bar::setLineWidth(int w)
{
	if(m_barWidth == w)
		return;
	m_barWidth = w;
	setupLine();
	setupBar();
}

void karin::scroll_bar::setBarWidth(int w)
{
	if(m_lineWidth == w)
		return;
	m_lineWidth = w;
	setupLine();
	setupBar();
}

void karin::scroll_bar::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	karin::rect::resizeEvent(event);
	setupLine();
	setupBar();
}

void karin::scroll_bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;
	QBrush brush;

	Q_UNUSED(widget);

	setupPainter(painter);
	
	if(m_lineVisible)
	{
		painter->save();
		{
			pen = painter->pen();
			pen.setWidth(m_lineWidth);
			pen.setColor(m_lineColor);
			painter->setPen(pen);
			painter->drawLine(m_line);
		}
		painter->restore();
	}

	painter->save();
	{
		brush = painter->brush();
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(m_barColor);
		painter->setBrush(brush);
		painter->fillPath(m_barPath, brush);
	}
	painter->restore();
}



// scroll_view
	karin::scroll_view::scroll_view(QGraphicsWidget *parent)
: karin::rect(parent),
	m_verticalBar(new karin::scroll_bar(this)),
	m_horizontalBar(new karin::scroll_bar(this)),
	m_timer(new QTimer(this))
{
	setObjectName(OBJECTNAME_SCROLL_VIEW);
	setHandleMouseEvent(false);
	m_horizontalBar->setOrientation(Qt::Horizontal);
	m_verticalBar->setOpacity(karin::scroll_view::M_Scroll_Bar_Opacity);
	m_horizontalBar->setOpacity(karin::scroll_view::M_Scroll_Bar_Opacity);

	m_timer->setInterval(karin::scroll_view::M_Scroll_Bar_Hide_Interval);
	m_timer->setSingleShot(true);
	setNeedToPaint(false);
	reset();

  setFlag(QGraphicsItem::ItemHasNoContents, true);
	setFocusPolicy(Qt::NoFocus);
	// setAcceptDrops(false);
	// setAcceptHoverEvents(false);

	relayout();

	connect(m_timer, SIGNAL(timeout()), this, SLOT(hideScrollerBar()));
}

karin::scroll_view::~scroll_view()
{
}

void karin::scroll_view::reset()
{
	m_touchId = -1;
	m_pressed = false;
	m_grab = false;
	m_lastPos.setX(-1);
	m_lastPos.setY(-1);

	updateViewport();
}

bool karin::scroll_view::handleScroll(const QGraphicsSceneMouseEvent *event)
{
	if(!m_grab)
		return false;
	if(!event)
		return false;

	switch(event->type())
	{
		case QEvent::GraphicsSceneMousePress:
			{
				m_pressed = true;
				m_lastPos = event->pos();
				return true;
			}
		case QEvent::GraphicsSceneMouseRelease:
			{
				if(m_pressed)
				{
					m_pressed = false;
					m_lastPos.setX(-1);
					m_lastPos.setY(-1);
					return true;
				}
			}
			break;
		case QEvent::GraphicsSceneMouseMove:
			{
				if(m_pressed)
				{
					QPointF delta = event->pos() - m_lastPos;
					QPointF p = m_position - delta;
					updatePosition(p);
					//qDebug()<<m_position<<m_range<<m_viewsize;
					m_lastPos = event->pos();
					return true;
				}
			}
			break;
		default:
			return false;
	}
	return false;
}

bool karin::scroll_view::swipe(const QTouchEvent *event)
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

void karin::scroll_view::updateViewport()
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

void karin::scroll_view::updatePosition(const QPointF &p)
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

		setupScrollBar();
		emit positionChanged(m_position);
	}
}

void karin::scroll_view::setRange(const QRectF &r)
{
	if(m_range == r)
		return;
	m_range = r;
	setupScrollBar();
}

void karin::scroll_view::setPosition(const QPointF &p)
{
	updatePosition(p);
}

void karin::scroll_view::setupScrollBar()
{
	QSizeF s = size();
	m_horizontalBar->setRange(m_range.width() + s.width(), s.width());
	if(Math_IsZero(m_range.width()))
		m_horizontalBar->setPer(1.0);
	else
		m_horizontalBar->setPer(m_position.x() / m_range.width());

	m_verticalBar->setRange(m_range.height() + s.height(), s.height());
	if(Math_IsZero(m_range.height()))
		m_verticalBar->setPer(1.0);
	else
		m_verticalBar->setPer(m_position.y() / m_range.height());

	//k m_horizontalBar->setVisible(!Math_IsZero(m_range.width()));
	//k m_verticalBar->setVisible(!Math_IsZero(m_range.height()));

	showScrollerBar();
}

void karin::scroll_view::ungrabGesture(Qt::GestureType type, Qt::GestureFlags flags)
{
	karin::rect::ungrabGesture(type);

	m_grab = false;
}

void karin::scroll_view::grabGestureWithCancelPolicy(Qt::GestureType type, Qt::GestureFlags flags)
{
	karin::rect::grabGesture(type, flags);
	m_grab = true;
}

void karin::scroll_view::resize(const QSizeF &s)
{
	setFixedSize(s.toSize());
}

void karin::scroll_view::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	karin::rect::resizeEvent(event);
	relayout();
}

void karin::scroll_view::relayout()
{
	QSize s = size().toSize();

	m_horizontalBar->setFixedSize(s.width(), 6);
	m_horizontalBar->setPos(0, s.height() - m_horizontalBar->height());

	m_verticalBar->setFixedSize(6, s.height());
	m_verticalBar->setPos(s.width() - m_verticalBar->width(), 0);

	setupScrollBar();
}

void karin::scroll_view::hideScrollerBar()
{
	m_verticalBar->hide();
	m_horizontalBar->hide();
}

void karin::scroll_view::showScrollerBar()
{
#if 0
	m_verticalBar->show();
	m_horizontalBar->show();
#else
	m_horizontalBar->setVisible(!Math_IsZero(m_range.width()));
	m_verticalBar->setVisible(!Math_IsZero(m_range.height()));
#endif
	if(!Math_IsZero(m_range.width()) || !Math_IsZero(m_range.height()))
	{
		if(m_timer->isActive())
			m_timer->stop();
		m_timer->start();
	}
}

bool karin::scroll_view::event(QEvent *event)
{
	if(m_grab)
	{
		switch(event->type())
		{
			case QEvent::TouchBegin:
			case QEvent::TouchUpdate:
			case QEvent::TouchEnd:
			case QEvent::TouchCancel:
				if(swipe(static_cast<QTouchEvent *>(event)))
					return true;
				else
					break;

			case QEvent::GraphicsSceneMousePress:
			case QEvent::GraphicsSceneMouseRelease:
			case QEvent::GraphicsSceneMouseMove:
				if(handleScroll(static_cast<QGraphicsSceneMouseEvent *>(event)))
					return true;
				else
					break;
			default:
				return karin::rect::event(event);
		}
	}
	return karin::rect::event(event);
}

