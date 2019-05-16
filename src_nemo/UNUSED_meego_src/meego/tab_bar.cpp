#include "tab_bar.h"
#include "tab_button.h"

#include <MButton>
#include <MSeparator>
#include <MPannableViewport>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>

#define TAB_BAR_HEIGHT 50
#define TAB_BAR_ICON_WIDTH 50

	karin::tab_bar::tab_bar(QGraphicsItem *parent)
:MWidgetController(parent),
	currentWidget(-1),
	m_menuButton(new MButton),
	addTabButton(new MButton),
	separator(new MSeparator(this, Qt::Vertical)),
	layout(new QGraphicsAnchorLayout),
	pan(new MPannableViewport),
	policy(new QGraphicsLinearLayout(Qt::Horizontal)),
	tabBar(new MWidget)
{
	setPreferredHeight(TAB_BAR_HEIGHT);
	setMinimumHeight(TAB_BAR_HEIGHT);
	setMaximumHeight(TAB_BAR_HEIGHT);
	pan -> setPreferredHeight(TAB_BAR_HEIGHT);
	pan -> setMinimumHeight(TAB_BAR_HEIGHT);
	pan -> setMaximumHeight(TAB_BAR_HEIGHT);
	pan -> setPanDirection(Qt::Horizontal);
	addTabButton->setViewType(MButton::iconType);
	addTabButton->setIconID("icon-m-toolbar-add");
	m_menuButton->setViewType(MButton::iconType);
	m_menuButton->setIconID("icon-m-toolbar-view-menu");
	//addTabButton -> setFocusPolicy(Qt::NoFocus);
	//m_menuButton -> setFocusPolicy(Qt::NoFocus);
	addTabButton -> setPreferredWidth(TAB_BAR_ICON_WIDTH);
	addTabButton -> setMinimumWidth(TAB_BAR_ICON_WIDTH);
	addTabButton -> setMaximumWidth(TAB_BAR_ICON_WIDTH);
	m_menuButton -> setPreferredWidth(TAB_BAR_ICON_WIDTH);
	m_menuButton -> setMinimumWidth(TAB_BAR_ICON_WIDTH);
	m_menuButton -> setMaximumWidth(TAB_BAR_ICON_WIDTH);
	policy -> setContentsMargins(0, 5, 0, 5);
	tabBar -> setLayout(policy);
	pan -> setWidget(tabBar);
	layout -> addAnchor(pan, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout -> addAnchor(m_menuButton, Qt::AnchorRight, layout, Qt::AnchorRight);
	layout -> addAnchor(addTabButton, Qt::AnchorRight, m_menuButton, Qt::AnchorLeft);
	layout -> addAnchor(separator, Qt::AnchorRight, addTabButton, Qt::AnchorLeft);
	layout -> addAnchor(pan, Qt::AnchorRight, separator, Qt::AnchorLeft);
	layout -> addAnchor(pan, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(addTabButton, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(separator, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(m_menuButton, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	connect(m_menuButton, SIGNAL(clicked(bool)), this, SIGNAL(openMenu()));
	connect(addTabButton, SIGNAL(clicked(bool)), this, SIGNAL(requestNewTab()));
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	setLayout(layout);
}

karin::tab_bar::~tab_bar()
{
}

karin::tab_button * karin::tab_bar::addTab(qint64 widget)
{
	if(widget < 0)
		return 0;
	int i;
	for(i = 0; i < policy -> count(); i++)
		if(dynamic_cast<karin::tab_button *>(policy -> itemAt(i)) -> widget == currentWidget)
		{
			break;
		}
	if(currentWidget > -1)
	{
		karin::tab_button *b = operator[](currentWidget);
		if(b)
			b -> setHighlight(false);
	}
	karin::tab_button *bt = new karin::tab_button;
	bt -> widget = widget;
	bt -> setHighlight(true);
	//bt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	policy -> insertItem(i + 1, bt);
	currentWidget = widget;
	if(i >= 0 && i < policy -> count())
	{
		QPointF p = dynamic_cast<karin::tab_button *>(policy -> itemAt(i)) -> pos();
		p.rx() += bt -> preferredWidth();
		pan -> setPosition(p);
	}
	connect(bt, SIGNAL(showTab(qint64)), this, SLOT(showTab(qint64)));
	return bt;
	//connect(bt, SIGNAL(closeTab(qint64)), this, SLOT(closeTab(qint64)));
}

void karin::tab_bar::showTab(qint64 widget)
{
	if(widget < 0)
		return;
	if(currentWidget != widget)
	{
		if(currentWidget > -1)
		{
			karin::tab_button *b = operator[](currentWidget);
			if(b)
				b -> setHighlight(false);
		}
		currentWidget = widget;
		updateTabPosition();
		emit currentTab(currentWidget);
	}
}

void karin::tab_bar::closeTab(qint64 widget)
{
	if(widget < 0)
		return;
	karin::tab_button *bt = operator[](widget);
	if(bt)
	{
		policy -> removeItem(bt);
		bt -> hide();
		if(bt -> scene())
			bt -> scene() -> removeItem(bt);
		bt -> clearFocus();
		bt -> setFocusProxy(0);
		bt -> setParentItem(0);
		bt -> setParent(0);
		bt -> disconnect();
		delete bt;
	}
}

qint64 karin::tab_bar::nextTab(qint64 widget)
{
	if(widget < 0)
		widget = currentWidget;
	int i;
	for(i = 0; i < policy -> count(); i++)
		if(dynamic_cast<karin::tab_button *>(policy -> itemAt(i)) -> widget == widget)
			break;
	karin::tab_button *bt2 = 0;
	if(policy -> count() > 1)
	{
		if(i == policy -> count() - 1)
		{
			bt2 = dynamic_cast<karin::tab_button *>(policy -> itemAt(i - 1));
		}
		else
		{
			bt2 = dynamic_cast<karin::tab_button *>(policy -> itemAt(i + 1));
		}
		if(bt2)
			return bt2 -> widget;
		else
			return -1;
	}
	else
		return -1;
}

karin::tab_button * karin::tab_bar::operator[] (qint64 id)
{
	karin::tab_button *b = 0;
	for(int i = 0; i < policy -> count(); i++)
	{
		b = dynamic_cast<karin::tab_button *>(policy -> itemAt(i));
		if(b -> widget == id)
		{
			break;
		}
	}
	return b;
}

void karin::tab_bar::allowWork()
{
	setPreferredHeight(TAB_BAR_HEIGHT);
	setMinimumHeight(TAB_BAR_HEIGHT);
	setMaximumHeight(TAB_BAR_HEIGHT);
	setEnabled(true);
	show();
}

void karin::tab_bar::forbidWork()
{
	setPreferredHeight(0);
	setMinimumHeight(0);
	setMaximumHeight(0);
	setEnabled(false);
	hide();
}

int karin::tab_bar::currentIndex() const
{
	for(int i = 0; i < policy -> count(); i++)
	{
		karin::tab_button *b = dynamic_cast<karin::tab_button *>(policy -> itemAt(i));
		if(b -> widget == currentWidget)
		{
			return i;
		}
	}
	return -1;
}

void karin::tab_bar::updateTabPosition()
{
	karin::tab_button *b = operator[](currentWidget);
	if(b)
	{
		b -> setHighlight(true);
		QPointF p = b -> pos();
		p.ry() = pan -> position().y();
		pan -> setPosition(p);
	}
}
