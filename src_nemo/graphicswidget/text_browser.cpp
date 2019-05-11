#include "text_browser.h"
#include "k.h"

#include <QVariant>
#include <QImage>
#include <QByteArray>
#include <QUrl>
#include <QKeyEvent>
#include <QScrollBar>
#include <QDesktopServices>
#include <QProcess>
#include <QDebug>

// text_browser
karin::text_browser::text_browser(QWidget *parent)
	:QTextBrowser(parent),
	slide_mode(true),
	start_x(-1),
	start_y(-1),
	last_x(-1),
	last_y(-1),
	mouse_pressed(false),
	zoom_dir(true),
	m_zoomRange(karin::text_browser::M_Zoom_Range),
	m_slideIgnoreAngle(karin::text_browser::M_Slide_Ignore_Angle),
	m_doubleClickZoom(karin::text_browser::M_Double_Click_Zoom),
	m_doubleClickZoomRange(karin::text_browser::M_Double_Click_Zoom_Range)
{
	setObjectName(OBJECTNAME_TEXT_BROWSER);

	setOpenLinks(false);

	connect(QTextBrowser::verticalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(scrollVBarValueChanged(int)));
	connect(QTextBrowser::horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SIGNAL(scrollHBarValueChanged(int)));
	connect(QTextBrowser::verticalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SIGNAL(scrollVBarRangeChanged(int, int)));
	connect(QTextBrowser::horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SIGNAL(scrollHBarRangeChanged(int, int)));
  setWordWrapMode(QTextOption::WordWrap);
}

karin::text_browser::~text_browser()
{
}

void karin::text_browser::setupStyle()
{
	QStringList style;
	if(m_textColor.length())
		style.push_back("color: " + m_textColor);
	if(m_bgColor.length())
		style.push_back("background-color: " + m_bgColor);
	QString styleStr = style.join(";");
	QTextBrowser::viewport()->setStyleSheet(styleStr);
}

void karin::text_browser::setStyleTextColor(const QString &color)
{
	if(m_textColor != color)
	{
		m_textColor = color;
		setupStyle();
	}
}

void karin::text_browser::setStyleBgColor(const QString &color)
{
	if(m_bgColor != color)
	{
		m_bgColor = color;
		setupStyle();
	}
}

void karin::text_browser::keyPressEvent(QKeyEvent* event)
{
  int key = event -> key();
  Qt::KeyboardModifiers mods = event -> modifiers();
  if (mods & (Qt::ControlModifier | Qt::ShiftModifier ))
  {
    switch ( key ) 
    {
      case Qt::Key_Left:
      case Qt::Key_B:
				backward();
				break;
      case Qt::Key_Right:
      case Qt::Key_F:
				forward();
				break;
      case Qt::Key_Up:
      case Qt::Key_I:
				QTextBrowser::zoomIn(m_zoomRange);
				break;
      case Qt::Key_Down:
      case Qt::Key_O:
				QTextBrowser::zoomOut(m_zoomRange);
				break;
      case Qt::Key_H:
				home();
				break;
      case Qt::Key_R:
				QTextBrowser::reload();
				break;
      default:
        QTextBrowser::keyPressEvent(event);
      break;
    }
  }
  else
  {
    QTextBrowser::keyPressEvent(event);
  }
}

int karin::text_browser::getVScrollBarPosition() const
{
	return QTextBrowser::verticalScrollBar() -> sliderPosition();
}

void karin::text_browser::setVScrollBarPosition( int pos )
{
	QTextBrowser::verticalScrollBar() -> setSliderPosition(pos);
}

int karin::text_browser::getHScrollBarPosition() const
{
	return QTextBrowser::horizontalScrollBar() -> sliderPosition();
}

void karin::text_browser::setHScrollBarPosition( int pos )
{
	QTextBrowser::horizontalScrollBar() -> setSliderPosition(pos);
}

void karin::text_browser::mouseMoveEvent(QMouseEvent *e)
{
	if(slide_mode && mouse_pressed)
	{
		int delta_x = e -> x() - last_x;
		int delta_y = e -> y() - last_y;
		float arg = Math_ComputeAngle(delta_x, delta_y);
		float limit = (float)(m_slideIgnoreAngle);
		if((arg > 90.0 - limit && arg < 90.0 + limit) || (arg > 270.0 - limit && arg < 270.0 + limit))
			QTextBrowser::verticalScrollBar() -> setSliderPosition(QTextBrowser::verticalScrollBar() -> sliderPosition() - delta_y);
		else if((arg >= 0.0 && arg < limit) || (arg > 180.0 - limit && arg < 180.0 + limit) || (arg > 360.0 - limit && arg <= 360.0))
			QTextBrowser::horizontalScrollBar() -> setSliderPosition(QTextBrowser::horizontalScrollBar() -> sliderPosition() - delta_x);
		else
		{
			QTextBrowser::verticalScrollBar() -> setSliderPosition(QTextBrowser::verticalScrollBar() -> sliderPosition() - delta_y);
			QTextBrowser::horizontalScrollBar() -> setSliderPosition(QTextBrowser::horizontalScrollBar() -> sliderPosition() - delta_x);
		}
		last_x = e -> x();
		last_y = e -> y();
	}
	else
	{
		QTextBrowser::mouseMoveEvent(e);
	}
}

void karin::text_browser::mousePressEvent(QMouseEvent *e)
{
	if(slide_mode)
	{
		start_x = e -> x();
		start_y = e -> y();
		last_x = e -> x();
		last_y = e -> y();
		mouse_pressed = true;
	}
	QTextBrowser::mousePressEvent(e);
}

void karin::text_browser::mouseReleaseEvent(QMouseEvent *e)
{
	if(slide_mode)
	{
		if(start_x == e -> x() && start_y == e -> y())
			QTextBrowser::mouseReleaseEvent(e);
		last_x = -1;
		last_y = -1;
		start_x = -1;
		start_y = -1;
		mouse_pressed = false;
	}
	else
		QTextBrowser::mouseReleaseEvent(e);
}

void karin::text_browser::setSlideMode(bool b)
{
	if(slide_mode != b)
	{
		slide_mode = b;
		last_x = -1;
		last_y = -1;
		start_x = -1;
		start_y = -1;
		mouse_pressed = false;
		emit slideModeChanged(slide_mode);
	}
}

bool karin::text_browser::slideMode() const
{
	return slide_mode;
}

void karin::text_browser::mouseDoubleClickEvent(QMouseEvent *e)
{
	if(slide_mode && m_doubleClickZoom)
	{
		int delta_x = QTextBrowser::width() / 2 - e -> x();
		int delta_y = QTextBrowser::height() / 2 - e -> y();
		QTextBrowser::verticalScrollBar() -> setSliderPosition(QTextBrowser::verticalScrollBar() -> sliderPosition() - delta_y);
		QTextBrowser::horizontalScrollBar() -> setSliderPosition(QTextBrowser::horizontalScrollBar() -> sliderPosition() - delta_x);

		if(zoom_dir)
		{
			QTextBrowser::zoomIn(m_doubleClickZoomRange);
			zoom_dir = false;
		}
		else
		{
			QTextBrowser::zoomOut(m_doubleClickZoomRange);
			zoom_dir = true;
		}
	}
	else
		QTextBrowser::mouseDoubleClickEvent(e);
}



// graphics_text_browser
karin::graphics_text_browser::graphics_text_browser(QGraphicsWidget *parent)
	: QGraphicsProxyWidget(parent),
	m_textBrowser(new karin::text_browser)
{
	setObjectName(OBJECTNAME_GRAPHICS_TEXT_BROWSER);

	setWidget(m_textBrowser);
	connect(m_textBrowser, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(linkClicked(const QUrl &)));
}

karin::graphics_text_browser::~graphics_text_browser()
{
}

void karin::graphics_text_browser::linkClicked(const QUrl &url)
{
	emit linkActivated(url.toString());
}
