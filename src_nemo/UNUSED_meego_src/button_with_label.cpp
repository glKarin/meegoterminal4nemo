#include "button_with_label.h"

#include <MButton>
#include <QGraphicsAnchorLayout>
#include <MLabel>
#include <QColor>

//#define BUTTON_WITH_LABEL_HEIGHT 50

karin::button_with_label::button_with_label(karin::button_with_label::karin_ViewType type, QGraphicsItem *parent)
	:MWidgetController(parent),
	label(new MLabel),
	button(new MButton)
{
	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout;
	/*
	setPreferredHeight(BUTTON_WITH_LABEL_HEIGHT);
	setMaximumHeight(BUTTON_WITH_LABEL_HEIGHT);
	setMinimumHeight(BUTTON_WITH_LABEL_HEIGHT);
	*/
	label -> setAlignment(Qt::AlignLeft);
	if(type == karin::button_with_label::switchType)
	{
		button -> setViewType(MButton::switchType);
		layout -> addAnchor(label, Qt::AnchorLeft, layout, Qt::AnchorLeft);
		layout -> addAnchor(button, Qt::AnchorRight, layout, Qt::AnchorRight);
		layout -> addAnchor(label, Qt::AnchorRight, button, Qt::AnchorLeft);
	}
	else
	{
		button -> setViewType(MButton::checkboxType);
		layout -> addAnchor(button, Qt::AnchorLeft, layout, Qt::AnchorLeft);
		layout -> addAnchor(label, Qt::AnchorRight, layout, Qt::AnchorRight);
		layout -> addAnchor(label, Qt::AnchorLeft, button, Qt::AnchorRight);
	}
	layout -> addAnchor(button, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(label, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	button -> setCheckable(true);
	connect(button, SIGNAL(pressed()), this, SIGNAL(pressed()));
	connect(button, SIGNAL(released()), this, SIGNAL(released()));
	connect(button, SIGNAL(clicked(bool)), this, SIGNAL(clicked(bool)));
	connect(button, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));
	setLayout(layout);
}

karin::button_with_label::~button_with_label()
{
}

void karin::button_with_label::setTitle(const QString &title)
{
	label -> setText(title);
}

void karin::button_with_label::setTitleColor(const QString &color)
{
	label -> setColor(QColor(color));
}

void karin::button_with_label::setCheckable(bool b)
{
	button -> setCheckable(b);
}

void karin::button_with_label::setChecked(bool b)
{
	button -> setChecked(b);
}

void karin::button_with_label::setEnabled(bool b)
{
	button -> setEnabled(b);
}

void karin::button_with_label::setTitleFont(const QFont &font)
{
	label -> setFont(font);
}

QFont karin::button_with_label::titleFont() const
{
	return label -> font();
}
