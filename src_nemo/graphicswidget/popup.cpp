#include "popup.h"

#include "k.h"

#include <QGraphicsAnchorLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsScene>
#include <QDebug>


// popup

karin::popup::popup(QGraphicsWidget *parent)
	: karin::rect(parent),
	m_animation(new QPropertyAnimation(this, "geometry")),
	m_state(karin::popup::Popup_Is_Closed),
	m_duration(karin::popup::M_Duration),
	m_parentWidget(parent),
	m_centralWidget(0),
	m_layout(new QGraphicsAnchorLayout(this)),
	m_popupType(karin::popup::Popup_R2L),
	m_appearType(karin::popup::Popup_Appear_In_ParentWidget)
{
	setObjectName(OBJECTNAME_POPUP);
	setNeedToPaint(false);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);

	setParentItem(m_parentWidget);

	connect(m_animation, SIGNAL(finished()), this, SLOT(finishedSlot()));
	setLayout(m_layout);
	m_animState.property_name = m_popupType == karin::popup::Popup_Fade ? "opacity" : "geometry";
		
	storeCloseState();
	storeOpenState();
	startState();
}

karin::popup::~popup()
{
}

void karin::popup::setFixedWidth(int w)
{
	if(m_fixedWidth == w && width() == w)
		return;
	karin::rect::setFixedWidth(w);
	reset(false);
}

void karin::popup::setFixedHeight(int h)
{
	if(m_fixedHeight == h && height() == h)
		return;
	karin::rect::setFixedHeight(h);
	reset(false);
}

void karin::popup::setFixedSize(const QSize &s)
{
	if((m_fixedHeight == s.height() && height() == s.height())
			&& (m_fixedWidth == s.width() && width() == s.width()))
		return;
	karin::rect::setFixedSize(s);
	reset(false);
}

void karin::popup::setFixedSize(int w, int h)
{
	if((m_fixedHeight == h && height() == h)
			&& (m_fixedWidth == w && width() == w))
		return;
	karin::rect::setFixedSize(w, h);
	reset(false);
}

QVariant karin::popup::startValue() const
{
	return m_animState.property_name == "geometry" ? QVariant(geometry()) : QVariant(opacity());
}

void karin::popup::startState()
{
	if(m_animState.property_name == "geometry")
		setGeometry(m_animState.close_state.toRectF());
	else
		setOpacity(m_animState.close_state.toReal());
}

void karin::popup::endState()
{
	if(m_animState.property_name == "geometry")
		setGeometry(m_animState.open_state.toRectF());
	else
		setOpacity(m_animState.open_state.toReal());
}

void karin::popup::toggle(bool b)
{
	int d;

	if(!isValid())
		return;
	if(b && m_state == karin::popup::Popup_Is_Opened)
		return;
	if(!b && m_state == karin::popup::Popup_Is_Closed)
		return;
	setVisible(true);
	setState((m_state == karin::popup::Popup_Is_Opened || m_state == karin::popup::Popup_Is_Opening) ? karin::popup::Popup_Is_Closing : karin::popup::Popup_Is_Opening);
	m_animation->setStartValue(startValue());

	d = int(m_duration);
	m_animation->setEndValue(m_state == karin::popup::Popup_Is_Closing ? m_animState.close_state : m_animState.open_state);
	m_animation->setDuration(d);
	m_animation->start();
}

void karin::popup::finishedSlot()
{
	if(m_state == karin::popup::Popup_Is_Closing)
	{
		setState(karin::popup::Popup_Is_Closed);
		emit done(false);
	}
	else if(m_state == karin::popup::Popup_Is_Opening)
	{
		setState(karin::popup::Popup_Is_Opened);
		emit done(true);
	}
	if(m_state == karin::popup::Popup_Is_Closed)
		setVisible(false);
}

void karin::popup::setState(Popup_State_e s)
{
	if(m_state != s)
	{
		m_state = s;
		emit stateChanged(m_state);
		if(m_state == karin::popup::Popup_Is_Opened)
		{
			emit opened();
		}
		else if(m_state == karin::popup::Popup_Is_Closed)
		{
			emit closed();
		}
	}
}

void karin::popup::setParentWidget(QGraphicsWidget *w)
{
	m_parentWidget = w;
	setParentItem(m_parentWidget);
	reset();
}

void karin::popup::open(bool anim)
{
	if(!isValid())
		return;
	if(m_state == karin::popup::Popup_Is_Opened)
		return;
	if(anim)
		toggle(true);
	else
	{
		m_animation->stop();
		endState();
		setVisible(true);
		setState(karin::popup::Popup_Is_Opened);
		emit done(true);
	}
}

void karin::popup::close(bool anim)
{
	if(!isValid())
		return;
	if(m_state == karin::popup::Popup_Is_Closed)
		return;
	if(anim)
		toggle(false);
	else
	{
		m_animation->stop();
		startState();
		setVisible(false);
		setState(karin::popup::Popup_Is_Closed);
		emit done(false);
	}
}

void karin::popup::setCentralWidget(QGraphicsWidget *w)
{
	if(m_centralWidget == w)
		return;
	if(m_centralWidget)
	{
		m_layout->removeAt(0); //m_centralWidget;
		m_centralWidget->deleteLater();
		m_centralWidget = 0;
	}
	m_centralWidget = w;
	if(m_centralWidget)
	{
		m_centralWidget->setParent(this);
		m_centralWidget->setParentItem(this);
		m_layout->addAnchor(m_centralWidget, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
		m_layout->addAnchor(m_centralWidget, Qt::AnchorRight, m_layout, Qt::AnchorRight);
		m_layout->addAnchor(m_centralWidget, Qt::AnchorTop, m_layout, Qt::AnchorTop);
		m_layout->addAnchor(m_centralWidget, Qt::AnchorBottom, m_layout, Qt::AnchorBottom);
	}
}

void karin::popup::setPopupType(karin::popup::Popup_Type_e e)
{
	if(m_popupType == e)
		return;
	endState();
	m_popupType = e;
	m_animState.property_name = m_popupType == karin::popup::Popup_Fade ? "opacity" : "geometry";
	m_animation->setPropertyName(QString(m_animState.property_name).toLatin1());
	reset(true);
}

bool karin::popup::isValid() const
{
	return (m_appearType == karin::popup::Popup_Appear_In_ParentWidget && m_parentWidget) || (m_appearType == karin::popup::Popup_Appear_In_Scene && scene());
}

void karin::popup::storeCloseState()
{
	if(!isValid())
		return;

	if(m_animState.property_name == "geometry")
	{
		QRectF ng = geometry();
		QSizeF s = m_appearType == karin::popup::Popup_Appear_In_ParentWidget ? m_parentWidget->size() : scene()->sceneRect().size();
		QPointF p = m_appearType == karin::popup::Popup_Appear_In_ParentWidget ? m_parentWidget->pos() : scene()->sceneRect().topLeft();
		switch(m_popupType)
		{
			case karin::popup::Popup_R2L:
				{
					ng.setX(s.width());
					ng.setY(p.y());
				}
				break;
			case karin::popup::Popup_L2R:
				{
					ng.setX(-width());
					ng.setY(p.y());
				}
				break;
			case karin::popup::Popup_U2D:
				{
					ng.setX(p.x());
					ng.setY(-height());
				}
				break;
			case karin::popup::Popup_D2U:
				{
					ng.setX(p.x());
					ng.setY(s.height());
				}
				break;
		}
		m_animState.close_state = QVariant(ng);
	}
	else
	{
		m_animState.close_state = QVariant(0.0);
	}

}

void karin::popup::storeOpenState()
{
	if(!isValid())
		return;

	if(m_animState.property_name == "geometry")
	{
		QRectF ng = geometry();
		QSizeF s = m_appearType == karin::popup::Popup_Appear_In_ParentWidget ? m_parentWidget->size() : scene()->sceneRect().size();
		QPointF p = m_appearType == karin::popup::Popup_Appear_In_ParentWidget ? m_parentWidget->pos() : scene()->sceneRect().topLeft();
		switch(m_popupType)
		{
			case karin::popup::Popup_R2L:
				{
					ng.setX(qMax(s.width() - width(), qreal(0)));
					ng.setY(p.y());
				}
				break;
			case karin::popup::Popup_L2R:
				{
					ng.setX(0);
					ng.setY(p.y());
				}
				break;
			case karin::popup::Popup_U2D:
				{
					ng.setX(p.x());
					ng.setY(0);
				}
				break;
			case karin::popup::Popup_D2U:
				{
					ng.setX(p.x());
					ng.setY(qMax(s.height() - height(), qreal(0)));
				}
				break;
		}
		m_animState.open_state = QVariant(ng);
	}
	else
	{
		m_animState.open_state = QVariant(1.0);
	}
}

void karin::popup::resizeEvent(QGraphicsSceneResizeEvent * event)
{
	if(m_appearType == karin::popup::Popup_Appear_In_ParentWidget)
		reset();
	karin::rect::resizeEvent(event);
}

void karin::popup::reset(bool close)
{
	QGraphicsScene *s;

	if(m_appearType == karin::popup::Popup_Appear_In_Scene)
	{
		s = scene();
		if(s)
		{
			QRectF r = s->sceneRect();
			if(m_popupType == karin::popup::Popup_R2L || m_popupType == karin::popup::Popup_L2R || m_popupType )
				setFixedHeight(r.height());
			else if(m_popupType == karin::popup::Popup_U2D || m_popupType == karin::popup::Popup_D2U)
				setFixedHeight(r.width());
			else
				setFixedSize(r.size().toSize());
		}
	}
	storeCloseState();
	storeOpenState();
	if(close)
	{
		m_animation->stop();
		setVisible(false);
		startState();
		setState(karin::popup::Popup_Is_Closed);
	}
}

void karin::popup::sceneRectChangedSlot(const QRectF &r)
{
	Q_UNUSED(r);

	if(m_appearType == karin::popup::Popup_Appear_In_Scene)
		reset();
}

void karin::popup::setAppearType(karin::popup::Popup_Appear_e e)
{
	if(m_appearType == e)
		return;
	m_appearType = e;
	reset();
}

QGraphicsWidget * karin::popup::take()
{
	QGraphicsWidget *r;

	if(!m_centralWidget)
		return 0;

	m_layout->removeAt(0); //m_centralWidget;
	m_centralWidget->setParent(0);
	m_centralWidget->setParentItem(0);
	r = m_centralWidget;
	m_centralWidget = 0;

	return r;
}



// timer_popup
	karin::timer_popup::timer_popup(QGraphicsWidget *parent)
: karin::popup(parent),
	m_deletionPolicy(karin::timer_popup::KeepWhenDone),
	m_timer(new QTimer(this)),
	m_interval(karin::timer_popup::M_Interval),
	m_pressed(false),
	m_deleteLater(false),
	m_autoHideEnabled(true)
{
	setObjectName(OBJECTNAME_TIMER_POPUP);
	m_timer->setSingleShot(true);
	m_timer->setInterval(m_interval);

	connect(m_timer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
	connect(this, SIGNAL(done(bool)), this, SLOT(doneSlot(bool)));
}

karin::timer_popup::~timer_popup()
{
}

void karin::timer_popup::appear(karin::timer_popup::Timer_Popup_DeletionPolicy_e e)
{
	m_deletionPolicy = e;
	open(true);
}

void karin::timer_popup::doneSlot(bool open)
{
	if(open)
	{
		if(m_autoHideEnabled)
			m_timer->start();
	}
	else
	{
		if(m_deletionPolicy == karin::timer_popup::DestroyWhenDone)
		{
			if(!m_deleteLater)
			{
				m_deleteLater = true;
				this->deleteLater();
			}
		}
	}
}

void karin::timer_popup::timeoutSlot()
{
	if(m_deletionPolicy == karin::timer_popup::DestroyWhenDone && m_deleteLater)
		return;
	if(!m_pressed)
		close(true);
}

void karin::timer_popup::setInterval(int ms)
{
	if(m_interval != ms)
	{
		m_interval = ms;
		m_timer->setInterval(m_interval);
	}
}

void karin::timer_popup::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	karin::popup::mousePressEvent(e);
	return;
	if(state() == karin::popup::Popup_Is_Opened)
	{
		m_pressed = true;
	}
}

void karin::timer_popup::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	karin::popup::mouseReleaseEvent(e);
	return;
	if(m_pressed)
	{
		if(state() == karin::popup::Popup_Is_Opened)
		{
			m_timer->stop();
			close(true);
		}
		m_pressed = false;
	}
}

void karin::timer_popup::setAutoHideEnabled(bool on)
{
	if(m_autoHideEnabled == on)
		return;
	if(m_timer->isActive())
		return;
	m_autoHideEnabled = on;
}

void karin::timer_popup::appear()
{
	open(true);
}

void karin::timer_popup::disappear()
{
	close(true);
}

bool karin::timer_popup::isAppeared() const
{
	return state() == karin::popup::Popup_Is_Opened;
}

void karin::timer_popup::toggle(bool open)
{
	m_timer->stop();
	karin::popup::toggle(open);
}

void karin::timer_popup::open(bool anim)
{
	m_timer->stop();
	karin::popup::open(anim);
}

void karin::timer_popup::close(bool anim)
{
	m_timer->stop();
	karin::popup::close(anim);
}

