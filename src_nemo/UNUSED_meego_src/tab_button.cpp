#include "tab_button.h"

#include <MButton>
#include <MLabel>
#include <MLabelHighlighter>
#include <QRegExp>
#include <QGraphicsAnchorLayout>

#define TAB_BUTTON_ICON_WIDTH 40
#define TAB_BUTTON_WIDTH 260
#define TAB_BUTTON_HEIGHT 40

karin::tab_button::tab_button(QGraphicsItem *parent)
	:MListItem(parent),
	label(new MLabel),
	hler(new MCommonLabelHighlighter(QRegExp(".*"))),
	button(new MButton),
	button2(new MButton),
	widget(-1),
	layout(new QGraphicsAnchorLayout)
{
	setPreferredSize(TAB_BUTTON_WIDTH, TAB_BUTTON_HEIGHT);
	setMinimumSize(TAB_BUTTON_WIDTH, TAB_BUTTON_HEIGHT);
	setMaximumSize(TAB_BUTTON_WIDTH, TAB_BUTTON_HEIGHT);
	label -> setTextElide(true);
	QFont font;
	font.setPointSize(16);
	label -> setFont(font);
	button -> setViewType(MButton::iconType);
	button -> setIconID("icon-m-toolbar-close");
	button2 -> setViewType(MButton::iconType);
	button2 -> setIconID("icon-m-toolbar-callhistory");
	button2 -> setToggledIconID("icon-m-toolbar-trim");
	button2 -> setCheckable(true);
	//button2 -> setChecked(true);
	button -> setPreferredSize(TAB_BUTTON_ICON_WIDTH, TAB_BUTTON_HEIGHT);
	button -> setMinimumSize(TAB_BUTTON_ICON_WIDTH, TAB_BUTTON_HEIGHT);
	button -> setMaximumSize(TAB_BUTTON_ICON_WIDTH, TAB_BUTTON_HEIGHT);
	button2 -> setPreferredSize(TAB_BUTTON_ICON_WIDTH, TAB_BUTTON_HEIGHT);
	button2 -> setMinimumSize(TAB_BUTTON_ICON_WIDTH, TAB_BUTTON_HEIGHT);
	button2 -> setMaximumSize(TAB_BUTTON_ICON_WIDTH, TAB_BUTTON_HEIGHT);
	layout -> addAnchor(label, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout -> addAnchor(button, Qt::AnchorRight, layout, Qt::AnchorRight);
	layout -> addAnchor(button2, Qt::AnchorRight, button, Qt::AnchorLeft);
	layout -> addAnchor(label, Qt::AnchorRight, button2, Qt::AnchorLeft);
	layout -> addAnchor(label, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(button, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(button2, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	connect(button, SIGNAL(clicked(bool)), this, SLOT(doLongTapped(bool)));
	connect(button2, SIGNAL(clicked(bool)), this, SLOT(doClicked2(bool)));
	connect(hler, SIGNAL(clicked(const QString &)), this, SLOT(doClicked3(const QString &)));
	connect(this, SIGNAL(clicked()), SLOT(doClicked()));
	//connect(this, SIGNAL(longTapped(const QPointF &f)), SLOT(doLongTapped()));
	setLayout(layout);
}

karin::tab_button::~tab_button()
{
}

void karin::tab_button::setHighlight(bool b)
{
	label -> removeAllHighlighters();
	if(b)
		label -> addHighlighter(hler);
}

void karin::tab_button::setLabel(const QString &name)
{
	label -> setText(name);
}

void karin::tab_button::setLabel(const QString &name, qint64 id)
{
	if(id >= 0 && widget == id)
		label -> setText(name);
}

void karin::tab_button::doSelectionModeChange(bool b, qint64 id)
{
	if(id >= 0 && id == widget && button2 -> isChecked() != b)
		button2 -> setChecked(b);
}

void karin::tab_button::doClicked()
{
	if(widget >= 0)
	{
		emit showTab(widget);
	}
}

void karin::tab_button::doClicked2(bool b)
{
	if(widget >= 0)
	{
		button -> setChecked(b);
		emit requestSelectionMode(b, widget);
	}
}

void karin::tab_button::doClicked3(const QString &t)
{
	Q_UNUSED(t);
	if(widget >= 0)
	{
		emit showTab(widget);
	}
}

void karin::tab_button::doLongTapped(bool b)
{
	if(!b && widget >= 0)
	{
		emit closeTab(widget);
	}
}

