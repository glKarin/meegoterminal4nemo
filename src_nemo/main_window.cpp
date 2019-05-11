#include "main_window.h"

#include "karin_ut.h"
#include "k.h"

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QApplication>
#include <QResizeEvent>
#include <QDebug>

karin::main_window::main_window(QWidget *parent)
	: QGraphicsView(parent),
	m_scene(new QGraphicsScene(this)),
	m_widget(0)
{
	setScene(m_scene);
	setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	m_scene->setSceneRect(QRectF(0, 0, width(), height()));

	connect(m_scene, SIGNAL(sceneRectChanged(const QRectF &)), this, SLOT(sceneRectChangedSlot(const QRectF &)));

	m_viewportSize = size();

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	connect(qApp->inputMethod(), SIGNAL(keyboardRectangleChanged()), this, SLOT(keyboardRectangleChangedSlot()));

	//setViewportMargins(0, 0, 0, 0);
	setDragMode(QGraphicsView::NoDrag);
	setWindowFlags(Qt::Window);
	setBackgroundBrush(Qt::black);

	setObjectName(OBJECTNAME_MAIN_WINDOW);
}

karin::main_window::~main_window()
{
}

void karin::main_window::resizeEvent(QResizeEvent *event)
{
	if(event)
	{
		m_scene->setSceneRect(QRectF(0, 0, event->size().width(), event->size().height()));
		m_viewportSize = event->size();
	}
	if(m_widget)
	{
		QRectF imArea = qApp->inputMethod()->keyboardRectangle();
    if(karin::ut::Instance()->getSetting<bool>(TRANSLUCENT_INPUTMETHOD) || imArea.isEmpty()) 
			m_widget->resize(m_viewportSize);
		else
		{
			imArea.setWidth(0);
			QSizeF s = m_viewportSize - imArea.size();
			m_widget->resize(s);
		}
	}
}

void karin::main_window::setCentralWidget(QGraphicsWidget *widget)
{
	if(m_widget)
	{
		m_widget->setParent(0);
		m_scene->removeItem(m_widget);
		m_widget->deleteLater();
		m_widget = 0;
	}
	if(widget)
	{
		m_widget = widget;
		m_scene->addItem(m_widget);
		m_widget->setParent(this);
		m_widget->resize(m_viewportSize);
	}
}

QGraphicsWidget * karin::main_window::deattachWidget()
{
	QGraphicsWidget *w;

	w = 0;
	if(m_widget)
	{
		m_scene->removeItem(m_widget);
		w = m_widget;
		m_widget = 0;
	}
	return w;
}

QGraphicsWidget * karin::main_window::replaceWidget(QGraphicsWidget *widget)
{
	QGraphicsWidget *w;

	w = 0;
	if(m_widget)
	{
		m_scene->removeItem(m_widget);
		w = m_widget;
		m_widget = 0;
	}
	if(widget)
	{
		m_scene->addItem(widget);
		m_widget = widget;
		m_widget->resize(m_viewportSize);
	}
	return w;
}

void karin::main_window::sceneRectChangedSlot(const QRectF &r)
{
	Q_UNUSED(r);
	if(m_widget)
	{
		QRectF imArea = qApp->inputMethod()->keyboardRectangle();
    if(karin::ut::Instance()->getSetting<bool>(TRANSLUCENT_INPUTMETHOD) || imArea.isEmpty()) 
			m_widget->resize(m_viewportSize);
		else
		{
			imArea.setWidth(0);
			QSizeF s = m_viewportSize - imArea.size();
			m_widget->resize(s);
		}
	}
	emit viewSceneRectChanged(QRectF(0, 0, m_viewportSize.width(), m_viewportSize.height()));
}

void karin::main_window::keyboardRectangleChangedSlot()
{
	resizeEvent(0);
}

