#include "msg_box.h"
#include "label.h"
#include "k.h"

// msg_box
karin::msg_box::msg_box(const QString &title, const QString &text, QGraphicsWidget *parent)
	: karin::timer_popup(parent),
	m_label(new karin::label(this)),
	m_title(title),
	m_text(text)
{
	setObjectName(OBJECTNAME_MSG_BOX);
	setCentralWidget(m_label);
	setFixedHeight(128);
	if(parent)
		setFixedWidth(K_GET_WIDTH(parent));

	setPopupType(karin::popup::Popup_Fade);
	QFont f = font();
	f.setPixelSize(32);
	m_label->setFont(f);

	m_label->setBgColor(Qt::black);
	m_label->setColor(Qt::white);
	setupText();
}

karin::msg_box::~msg_box()
{
}

void karin::msg_box::setTitle(const QString &title)
{
	if(m_title != title)
	{
		m_title = title;
		setupText();
	}
}

void karin::msg_box::setText(const QString &text)
{
	if(m_text != text)
	{
		m_text = text;
		setupText();
	}
}

void karin::msg_box::setupText()
{
	m_label->setText(QString("[%1]: %2").arg(m_title).arg(m_text));
}

void karin::msg_box::setTextColor(const QColor &c)
{
	m_label->setColor(c);
}



// info_banner
karin::info_banner::info_banner(QGraphicsWidget *parent)
	: karin::timer_popup(parent),
	m_label(new karin::label(this))
{
	setObjectName(OBJECTNAME_INFO_BANNER);
	setCentralWidget(m_label);
	setFixedHeight(48);
	if(parent)
		setFixedWidth(K_GET_WIDTH(parent));

	setPopupType(karin::popup::Popup_U2D);
	QFont f = font();
	f.setPixelSize(24);
	m_label->setFont(f);

	m_label->setBgColor(Qt::black);
	m_label->setColor(Qt::white);
	setupText();
}

karin::info_banner::~info_banner()
{
}

void karin::info_banner::setTitle(const QString &title)
{
	if(m_title != title)
	{
		m_title = title;
		setupText();
	}
}

void karin::info_banner::setupText()
{
	m_label->setText(m_title);
}

void karin::info_banner::setTextColor(const QColor &c)
{
	m_label->setColor(c);
}

void karin::info_banner::appear()
{
	karin::timer_popup::appear(karin::timer_popup::DestroyWhenDone);
}

void karin::info_banner::appear(Timer_Popup_DeletionPolicy_e e)
{
	karin::timer_popup::appear(e);
}
