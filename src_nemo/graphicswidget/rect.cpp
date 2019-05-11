#include "rect.h"

#include <QDebug>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

karin::rect::rect(QGraphicsWidget *parent)
	: QGraphicsWidget(parent),
	m_needToPaint(true),
	m_bgColor(static_cast<Qt::GlobalColor>(karin::rect::M_BgColor)),
	m_borderColor(static_cast<Qt::GlobalColor>(karin::rect::M_BorderColor)),
	m_borderWidth(0),
	m_radius(0/*karin::rect::M_Radius*/),
	m_positionType(karin::rect::Rect_In_Border),
	m_interactiveType(karin::rect::Rect_Mouse),
	m_handleMouseEvent(true)
{
	setObjectName(OBJECTNAME_RECT);
	//setFixedSize(karin::rect::M_FixedWidth, karin::rect::M_FixedHeight);
	resize(karin::rect::M_FixedWidth, karin::rect::M_FixedHeight);

	caleInnerRect();
	caleOuterRect();
	caleCenter();
	caleInnerPath();
	caleOuterPath();

	setAcceptTouchEvents(false);
	setFlags(QGraphicsItem::ItemClipsToShape | QGraphicsItem::ItemClipsChildrenToShape);
	//setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	
	setContentsMargins(0, 0, 0, 0);
	setupDefaultFont();
}

karin::rect::~rect()
{
	K_DELETE_DEBUG();
}

void karin::rect::setupFixedSize(const QSizeF &s)
{
	setFixedSize(s.toSize());
}

void karin::rect::setFixedWidth(int s)
{
	if(m_fixedWidth == s && width() == s)
		return;
	m_fixedWidth = s;
	setMinimumWidth(s);
	setMaximumWidth(s);
	setPreferredWidth(s);
	K_SET_WIDTH(this, s);

	caleInnerRect();
	caleOuterRect();
	caleCenter();
	caleInnerPath();
	caleOuterPath();
	update();
}

void karin::rect::setFixedHeight(int s)
{
	if(m_fixedHeight == s && height() == s)
		return;
	m_fixedHeight = s;
	setMinimumHeight(s);
	setMaximumHeight(s);
	setPreferredHeight(s);
	K_SET_HEIGHT(this, s);

	caleInnerRect();
	caleOuterRect();
	caleCenter();
	caleInnerPath();
	caleOuterPath();
	update();
}

void karin::rect::setFixedSize(const QSize &s)
{
	if(QSize(m_fixedWidth, m_fixedHeight) == s && size() == s)
		return;
	m_fixedWidth = s.width();
	m_fixedHeight = s.height();
	setMinimumSize(s);
	setMaximumSize(s);
	setPreferredSize(s);
	resize(s);

	caleInnerRect();
	caleOuterRect();
	caleCenter();
	caleInnerPath();
	caleOuterPath();
	update();
}

void karin::rect::setFixedSize(int w, int h)
{
	setFixedSize(QSize(w, h));
}

void karin::rect::setBorderWidth(float w)
{
	m_borderWidth = w;

	caleInnerRect();
	caleOuterRect();
	caleCenter();
	caleInnerPath();
	caleOuterPath();
	update();
}

void karin::rect::caleCenter()
{
	QSizeF size(boundingRect().size());

	m_center.setX(Math_IsZero(size.width()) ? 0 : size.width() / 2);
	m_center.setY(Math_IsZero(size.height()) ? 0 : size.height() / 2);
}

void karin::rect::caleInnerRect()
{
	QRectF rect = boundingRect();

	if(Math_IsZero(m_borderWidth))
	{
		m_innerRect = rect;
		return;
	}
	
	m_innerRect.setX(rect.left() + m_borderWidth);
	m_innerRect.setY(rect.top() + m_borderWidth);
	m_innerRect.setWidth(rect.width() - m_borderWidth * 2);
	m_innerRect.setHeight(rect.height() - m_borderWidth * 2);

	if(!m_innerRect.isValid())
		m_innerRect = QRectF(0, 0, 0, 0);
}

void karin::rect::caleOuterRect()
{
	QRectF rect = boundingRect();
	
	if(Math_IsZero(m_borderWidth))
	{
		m_outerRect = rect;
		return;
	}

	m_outerRect.setX(rect.left() + m_borderWidth / 2);
	m_outerRect.setY(rect.top() + m_borderWidth / 2);
	m_outerRect.setWidth(rect.width() - m_borderWidth);
	m_outerRect.setHeight(rect.height() - m_borderWidth);

	if(!m_outerRect.isValid())
		m_outerRect = QRectF(0, 0, 0, 0);
}

void karin::rect::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	caleInnerRect();
	caleOuterRect();
	caleCenter();
	caleInnerPath();
	caleOuterPath();

	QGraphicsWidget::resizeEvent(event);
	emit sizeChanged(size());
}

void karin::rect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(!m_needToPaint)
		return;

	QPen pen;
	QBrush brush;

	Q_UNUSED(widget);

	setupPainter(painter);
	painter->save();
	{
		brush = painter->brush();
		brush.setStyle(Qt::SolidPattern);
		brush.setColor(m_bgColor);
		painter->setBrush(brush);
		painter->fillPath(m_innerPath, brush);
	}
	painter->restore();

	painter->save();
	{
		pen = painter->pen();
		pen.setWidth(m_borderWidth);
		pen.setColor(m_borderColor);
		painter->setPen(pen);
		//painter->drawRoundedRect(m_outerRect, m_radius, m_radius);
		painter->drawPath(m_innerPath);
	}
	painter->restore();
}

void karin::rect::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	QGraphicsWidget::mousePressEvent(e);
	if(m_handleMouseEvent)
		e->accept(); // accept mouse release event
}

void karin::rect::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	QGraphicsWidget::mouseReleaseEvent(e);
}

void karin::rect::setBgColor(const QColor &c)
{
	if(m_bgColor != c)
	{
		m_bgColor = c;
		update();
	}
}

void karin::rect::setBorderColor(const QColor &c)
{
	if(m_borderColor != c)
	{
		m_borderColor = c;
		update();
	}
}

void karin::rect::setNeedToPaint(bool b)
{
	if(m_needToPaint != b)
	{
		m_needToPaint = b;
		update();
	}
}

void karin::rect::caleOuterPath()
{
	QPainterPath path;

	path.moveTo(m_outerRect.topRight() - QPointF(m_positionType & karin::rect::Rect_In_TR ? m_radius : 0, 0));
	path.lineTo(m_outerRect.topLeft() + QPointF(m_positionType & karin::rect::Rect_In_TL ? m_radius : 0, 0));
	if(m_positionType & karin::rect::Rect_In_TL)
		path.quadTo(m_outerRect.topLeft(), m_outerRect.topLeft() + QPointF(0, m_radius));
	path.lineTo(m_outerRect.bottomLeft() + QPointF(0, m_positionType & karin::rect::Rect_In_BL ? -m_radius : 0));
	if(m_positionType & karin::rect::Rect_In_BL)
		path.quadTo(m_outerRect.bottomLeft(), m_outerRect.bottomLeft() + QPointF(m_radius, 0));
	path.lineTo(m_outerRect.bottomRight() - QPointF(m_positionType & karin::rect::Rect_In_BR ? m_radius : 0, 0));
	if(m_positionType & karin::rect::Rect_In_BR)
		path.quadTo(m_outerRect.bottomRight(), m_outerRect.bottomRight() + QPointF(0, -m_radius));
	path.lineTo(m_outerRect.topRight() + QPointF(0, m_positionType & karin::rect::Rect_In_TR ? m_radius : 0));
	if(m_positionType & karin::rect::Rect_In_TR)
		path.quadTo(m_outerRect.topRight(), m_outerRect.topRight() + QPointF(-m_radius, 0));

	m_outerPath = path;
}

void karin::rect::caleInnerPath()
{
	QPainterPath path;

	path.moveTo(m_innerRect.topRight() - QPointF(m_positionType & karin::rect::Rect_In_TR ? m_radius : 0, 0));
	path.lineTo(m_innerRect.topLeft() + QPointF(m_positionType & karin::rect::Rect_In_TL ? m_radius : 0, 0));
	if(m_positionType & karin::rect::Rect_In_TL)
		path.quadTo(m_innerRect.topLeft(), m_innerRect.topLeft() + QPointF(0, m_radius));
	path.lineTo(m_innerRect.bottomLeft() + QPointF(0, m_positionType & karin::rect::Rect_In_BL ? -m_radius : 0));
	if(m_positionType & karin::rect::Rect_In_BL)
		path.quadTo(m_innerRect.bottomLeft(), m_innerRect.bottomLeft() + QPointF(m_radius, 0));
	path.lineTo(m_innerRect.bottomRight() - QPointF(m_positionType & karin::rect::Rect_In_BR ? m_radius : 0, 0));
	if(m_positionType & karin::rect::Rect_In_BR)
		path.quadTo(m_innerRect.bottomRight(), m_innerRect.bottomRight() + QPointF(0, -m_radius));
	path.lineTo(m_innerRect.topRight() + QPointF(0, m_positionType & karin::rect::Rect_In_TR ? m_radius : 0));
	if(m_positionType & karin::rect::Rect_In_TR)
		path.quadTo(m_innerRect.topRight(), m_innerRect.topRight() + QPointF(-m_radius, 0));

	m_innerPath = path;
}

void karin::rect::setRadius(float f)
{
	QSizeF s = size();
	float r = qMin(qMin(s.width() / 2, s.height() / 2), f);
	if(m_radius != r)
	{
		m_radius = r;
		caleInnerPath();
		caleOuterPath();
	}
}

void karin::rect::setupPainter(QPainter *painter)
{
	if(!painter)
		return;
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setClipping(true);
	// painter->setRenderHint(QPainter::TextAntialiasing, true);
}

void karin::rect::setupDefaultFont()
{
	QFont f = font();

	f.setPixelSize(karin::rect::M_FontPixelSize);
	setFont(f);
}

void karin::rect::setPositionType(unsigned t)
{
	if(m_positionType == t)
		return;

	m_positionType = t;
	caleInnerPath();
	caleOuterPath();
	update();
}

void karin::rect::setInteractiveType(karin::rect::Rect_Interactive_Type_e e)
{
	if(m_interactiveType == e)
		return;
	m_interactiveType = e;
	setAcceptTouchEvents(m_interactiveType == karin::rect::Rect_Touch);
}

void karin::rect::setHandleMouseEvent(bool b)
{
	if(m_handleMouseEvent != b)
		m_handleMouseEvent = b;
}
