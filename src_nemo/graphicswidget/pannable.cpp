#include "pannable.h"
#include "k.h"

#include <QScroller>
#include <QDebug>

karin::pannable::pannable(QGraphicsWidget *parent)
	: karin::rect(parent),
	m_allowPan(true)
{
	setObjectName(OBJECTNAME_PANNABLE);
	//setNeedToPaint(false);
	setBorderWidth(0);
	QScroller::grabGesture(this, RECT_IS_TOUCH() ? QScroller::TouchGesture : QScroller::LeftMouseButtonGesture);

	setFlags(QGraphicsItem::ItemClipsToShape | QGraphicsItem::ItemClipsChildrenToShape);
}

karin::pannable::~pannable()
{
}

// from Qt 5.6 QtWidgets scroller example.

bool karin::pannable::event(QEvent *e)
{
	if(!m_allowPan)
		return QGraphicsWidget::event(e);
	if(!m_pannableWidget)
		return QGraphicsWidget::event(e);
	switch (e->type()) {
		// ![2]
		case QEvent::ScrollPrepare: 
			{
				QSizeF m_size = boundingRect().size();
				QScrollPrepareEvent *se = static_cast<QScrollPrepareEvent *>(e);
				se->setViewportSize(m_size);
				QRectF br = m_pannableWidget->boundingRect();
				se->setContentPosRange(QRectF(0, 0, qMax(qreal(0), br.width() - m_size.width()), qMax(qreal(0), br.height() - m_size.height())));
				se->setContentPos(-m_pannableWidget->pos());
				se->accept();
				return true;
			}
			// ![1]
			// ![2]
		case QEvent::Scroll: 
			{
				QScrollEvent *se = static_cast<QScrollEvent *>(e);
				m_pannableWidget->setPos(-se->contentPos() - se->overshootDistance());
				return true;
			}
			// ![2]
		default:
			break;
	}
	return QGraphicsWidget::event(e);
}

bool karin::pannable::sceneEvent(QEvent *e)
{
	if(!m_allowPan)
		return QGraphicsWidget::sceneEvent(e);
	if(!m_pannableWidget)
		return QGraphicsWidget::sceneEvent(e);
	switch (e->type()) {
		case QEvent::TouchBegin: 
			{
				if(RECT_IS_TOUCH())
					return true;
			}
		case QEvent::GraphicsSceneMousePress: 
			{
				// We need to return true for the MousePress here in the
				// top-most graphics object - otherwise gestures in our parent
				// objects will NOT work at all (the accept() flag is already
				// set to true by Qt)
				if(!RECT_IS_TOUCH())
					return true;
			}
		default:
			break;
	}
	return QGraphicsWidget::sceneEvent(e);
}

void karin::pannable::reset()
{
	if(m_pannableWidget)
		m_pannableWidget->setPos(0, 0);
}

void karin::pannable::setPannableWidget(QGraphicsWidget *widget)
{
	m_pannableWidget = widget; 
	reset();
}
