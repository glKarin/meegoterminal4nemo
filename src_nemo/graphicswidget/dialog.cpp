#include "dialog.h"
#include "button_group.h"
#include "button.h"
#include "tool_icon.h"
#include "text_browser.h"
#include "k.h"

#include <QStaticText>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsAnchorLayout>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include <QTextBrowser>
#include <QDebug>

	karin::dialog_title_bar::dialog_title_bar(QGraphicsWidget *parent)
: rect(parent),
	m_text(new QStaticText),
	m_closeBtn(new karin::tool_icon(this)),
	m_lineColor(static_cast<Qt::GlobalColor>(karin::dialog_title_bar::M_Line_Color)),
	m_textColor(static_cast<Qt::GlobalColor>(karin::dialog_title_bar::M_Text_Color))
{
	setObjectName(OBJECTNAME_DIALOG_TITLE_BAR);
	setContentsMargins(20, 0, 20, 0);
	m_closeBtn->setImage(_KARIN_PREFIX_ "/resource/icon-m-close.png");
	connect(m_closeBtn, SIGNAL(clicked(bool)), this, SLOT(closeClickedSlot(bool)));
}

karin::dialog_title_bar::~dialog_title_bar()
{
	delete m_text;
}

QString karin::dialog_title_bar::title() const
{
	return m_text->text();
}

void karin::dialog_title_bar::closeClickedSlot(bool b)
{
	if(b)
		emit closeClicked();
}

void karin::dialog_title_bar::setTitle(const QString &title)
{
	QString rich = QString("<font size='%1' color='%2'>%3</font>").arg(18).arg(KARIN_COLOR).arg(title);
	m_text->setText(rich);
}

void karin::dialog_title_bar::resizeEvent(QGraphicsSceneResizeEvent* event)
{
	QSizeF s = event ? size() : event->newSize();
	int max_h = karin::dialog_title_bar::M_Max_Close_Button_Size;
	int h = qMin((int)s.height(), max_h);
	m_closeBtn->setFixedSize(QSize(h, h));
	m_closeBtn->setRadius(h / 2);
	qreal l = 0.0;
	qreal r = 0.0;
	qreal t = 0.0;
	qreal b = 0.0;
	getContentsMargins(&l, &t, &r, &b);
	m_closeBtn->setX(s.width() - m_closeBtn->width() - r * 2);
	m_closeBtn->setY((s.height() - t - b - m_closeBtn->height() - karin::dialog_title_bar::M_Line_Width) / 2 + t);

	m_lines.clear();
	m_lines.push_back(QLineF(QPointF(l, s.height() - b - karin::dialog_title_bar::M_Line_Width), QPointF(s.width() - r, s.height() - b - karin::dialog_title_bar::M_Line_Width)));
	m_lines.push_back(QLineF(QPointF(m_closeBtn->x() - r, t + karin::dialog_title_bar::M_Line_Width), QPointF(m_closeBtn->x() - r, s.height() - b - karin::dialog_title_bar::M_Line_Width)));

	if(event)
		rect::resizeEvent(event);
}

void karin::dialog_title_bar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;
	QBrush brush;
	qreal l = 0.0;
	qreal r = 0.0;
	qreal t = 0.0;
	qreal b = 0.0;

	getContentsMargins(&l, &t, &r, &b);
	setupPainter(painter);

	karin::rect::paint(painter, option, widget);

	painter->save();
	{
		pen = painter->pen();
		pen.setColor(m_textColor);
		painter->setPen(pen);
		QFont f = painter->font();
		f.setPixelSize(font().pixelSize());
		painter->setFont(f);
		painter->drawStaticText(QPointF(l * 2, t + karin::dialog_title_bar::M_Line_Width), *m_text);
	}
	painter->restore();

	painter->save();
	{
		pen = painter->pen();
		pen.setColor(m_lineColor);
		pen.setWidth(karin::dialog_title_bar::M_Line_Width);
		painter->setPen(pen);
		painter->drawLines(m_lines);
	}
	painter->restore();
}

void karin::dialog_title_bar::setLineColor(const QColor &c)
{
	if(m_lineColor != c)
		m_lineColor = c;
}

void karin::dialog_title_bar::setTextColor(const QColor &c)
{
	if(m_textColor != c)
		m_textColor = c;
}


// dialog
	karin::dialog::dialog(QGraphicsWidget *parent)
: karin::timer_popup(parent),
	m_oriCentralWidget(new karin::rect(this)),
	m_titleBar(new karin::dialog_title_bar(this)),
	m_toolBar(new karin::button_group(this)),
	m_realCentralWidget(0)
{
	setObjectName(OBJECTNAME_DIALOG);

	if(parent)
		setFixedSize(parent->size().toSize());
	setPopupType(karin::popup::Popup_Fade);
	setAutoHideEnabled(false);

	this->setBgColor(static_cast<Qt::GlobalColor>(karin::dialog::M_Dialog_BG_Color));
	
	m_layout = new QGraphicsAnchorLayout(m_oriCentralWidget);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(10);
	m_oriCentralWidget->setLayout(m_layout);
	m_titleBar->setFixedHeight(karin::dialog::M_Title_Bar_Height);
	m_toolBar->setFixedHeight(karin::dialog::M_Button_Group_Height);
	relayout();
	karin::timer_popup::setCentralWidget(m_oriCentralWidget);
	connect(m_titleBar, SIGNAL(closeClicked()), this, SLOT(disappear()));

	setZValue(INT_MAX);
}

karin::dialog::~dialog()
{
}

void karin::dialog::setBgColor(const QColor &color)
{
	m_titleBar->setBgColor(color);
	m_oriCentralWidget->setBgColor(color);
	m_toolBar->setBgColor(color);
}

void karin::dialog::setCentralWidget(QGraphicsWidget *w)
{
	m_realCentralWidget = w;
	relayout();
}

void karin::dialog::relayout()
{
	Widget_LayoutClear(m_layout);

	// title bar
	m_layout->addAnchor(m_titleBar, Qt::AnchorRight, m_layout, Qt::AnchorRight);
	m_layout->addAnchor(m_titleBar, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
	m_layout->addAnchor(m_titleBar, Qt::AnchorTop, m_layout, Qt::AnchorTop);

	// tool bar
	if(m_toolBar->count())
	{
		m_layout->addAnchor(m_toolBar, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
		m_layout->addAnchor(m_toolBar, Qt::AnchorRight, m_layout, Qt::AnchorRight);
		m_layout->addAnchor(m_toolBar, Qt::AnchorBottom, m_layout, Qt::AnchorBottom);
	}

	// central widget
	if(m_realCentralWidget)
	{
		m_layout->addAnchor(m_realCentralWidget, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
		m_layout->addAnchor(m_realCentralWidget, Qt::AnchorRight, m_layout, Qt::AnchorRight);
		m_layout->addAnchor(m_realCentralWidget, Qt::AnchorTop, m_titleBar, Qt::AnchorBottom);
		if(m_toolBar->count())
			m_layout->addAnchor(m_realCentralWidget, Qt::AnchorBottom, m_toolBar, Qt::AnchorTop);
		else
			m_layout->addAnchor(m_realCentralWidget, Qt::AnchorBottom, m_layout, Qt::AnchorBottom);
	}
}


// msg_dialog
karin::msg_dialog::msg_dialog(const QString &title, const QString &text, QGraphicsWidget *parent)
	: karin::dialog(parent),
	m_textBrowser(new karin::graphics_text_browser(this))
{
	setObjectName(OBJECTNAME_MSG_DIALOG);

	setNeedToPaint(true);
	setRadius(10);
	setBorderWidth(karin::msg_dialog::M_Border_Width);
	setBorderColor(QColor(Qt::white));
	int contentsMargin = m_borderWidth * 2;
	setContentsMargins(contentsMargin, contentsMargin, contentsMargin, contentsMargin);
	setTitle(title);
	setText(text);

	m_textBrowser->setTextColor(QColor(static_cast<Qt::GlobalColor>(karin::msg_dialog::M_Msg_Dialog_Text_Color)).name());
	m_textBrowser->setBgColor(QColor(static_cast<Qt::GlobalColor>(karin::msg_dialog::M_Msg_Dialog_BG_Color)).name());
	connect(m_textBrowser, SIGNAL(linkActivated(const QString &)), this, SIGNAL(linkActivated(const QString &)));
	karin::dialog::setCentralWidget(m_textBrowser);
}

karin::msg_dialog::~msg_dialog()
{
}

void karin::msg_dialog::setText(const QString &t)
{
	m_textBrowser->setText(t);
}

QString karin::msg_dialog::text() const
{
	return m_textBrowser->text();
}
