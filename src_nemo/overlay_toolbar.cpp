#include "overlay_toolbar.h"
#include "button_group.h"
#include "button.h"
#include "k.h"

#include <QAction>
#include <QDebug>

karin::overlay_toolbar::overlay_toolbar(QGraphicsWidget *parent)
	: karin::timer_popup(parent),
	m_container(new karin::button_group(this))
{
	setObjectName(OBJECTNAME_OVERLAY_TOOLBAR);
	setInterval(5000);
	setPopupType(karin::popup::Popup_Fade);
	setCentralWidget(m_container);
	setFixedSize(karin::overlay_toolbar::M_Container_Height, karin::overlay_toolbar::M_Container_Height);

	connect(m_container, SIGNAL(sizeChanged(const QSizeF &)), this, SLOT(resizeByContent(const QSizeF &)));
}

karin::overlay_toolbar::~overlay_toolbar()
{
}

void karin::overlay_toolbar::setPosition(const QPointF &pos, ToolbarPlacement placement)
{
	QPointF p;
	QSizeF s = size();

	p.setX(pos.x() - s.width() / 2);
	if(placement == karin::overlay_toolbar::AbovePointOfInterest)
	{
		p.setY(pos.y() - s.height());
	}
	else
	{
		p.setY(pos.y());
	}
	setPos(p);
}

void karin::overlay_toolbar::resizeByContent(const QSizeF &r)
{
	setFixedSize(r.toSize());
}

bool karin::overlay_toolbar::event(QEvent *event)
{
	switch (event->type()) {
		case QEvent::ActionAdded:
			handleActionAdded(static_cast<QActionEvent *>(event));
			break;
		case QEvent::ActionRemoved:
			handleActionRemoved(static_cast<QActionEvent *>(event));
			break;
		case QEvent::ActionChanged:
			handleActionChanged(static_cast<QActionEvent *>(event));
			break;
		default:
			return karin::timer_popup::event(event);
			break;
	}

	event->accept();
	return true;
}

void karin::overlay_toolbar::handleActionAdded(QActionEvent *actionEvent)
{
	const int Btn_Height = (int)size().height();
	const int Btn_Width = karin::overlay_toolbar::M_Container_Button_Width;

	if(!actionEvent)
		return;
	Q_ASSERT(!actionEvent->before()); // we support appending only
	QAction *action(qobject_cast<QAction *>(actionEvent->action()));
	if(!action)
		return;

	karin::button *newButton = new karin::button(action->text());
	/*
		 newButton->grabGesture(Qt::TapGesture);
		 newButton->grabGesture(Qt::TapAndHoldGesture);
		 newButton->installEventFilter(eatMButtonGestureFilter);
		 newButton->setStyleName(action->objectName());
		 */
	QObject::connect(newButton, SIGNAL(clicked(bool)), action, SLOT(trigger()));

	m_buttons.push_back(newButton);

	newButton->setMaximumWidth(Btn_Width);
	newButton->setMinimumWidth(Btn_Width);
	newButton->setPreferredWidth(Btn_Width);
	K_SET_WIDTH(newButton, Btn_Width);

	newButton->setMaximumHeight(Btn_Height);
	newButton->setMinimumHeight(Btn_Height);
	newButton->setPreferredHeight(Btn_Height);
	K_SET_HEIGHT(newButton, Btn_Height);
	newButton->update();

	m_container->addButton(newButton);

	if (true || action->isVisible()) {
		visibilityUpdated();
	}
}

void karin::overlay_toolbar::handleActionRemoved(QActionEvent *actionEvent)
{
	const int actionIndex = actions().indexOf(actionEvent->action());
	if (actionIndex >= 0) {
		// Actions list is in sync with buttons list so we can
		// use the same index to delete the corresponding button.
		m_container->removeButton(actionIndex);
	}

	if (true || actionEvent->action()->isVisible()) {
		// Action was visible before removal, need to update widget.
		visibilityUpdated();
	}
}

void karin::overlay_toolbar::handleActionChanged(QActionEvent *actionEvent)
{
	// Name of action might have been changed.
	const int actionIndex = actions().indexOf(actionEvent->action());
	karin::button *button(m_buttons[actionIndex]);
	if (button->text() != actionEvent->action()->text()) {
		button->setText(actionEvent->action()->text());
	}

	// Update visibility of buttons to match visibility of actions.
	visibilityUpdated();
}

void karin::overlay_toolbar::appear()
{
	setFocusPolicy(Qt::ClickFocus);
	setFocusProxy(parentItem());
	karin::timer_popup::open(false);
	QList<karin::button *> buttons = m_container->buttons();
	K_FOREACH(QList<karin::button *>, buttons)
	{
		(*itor)->show();
	}
	m_container->relayout();
}

void karin::overlay_toolbar::disappear()
{
	setFocusProxy(0);
	setFocusPolicy(Qt::NoFocus);
	karin::timer_popup::disappear();
}

void karin::overlay_toolbar::visibilityUpdated()
{
	karin::button *button;
	QList<QAction *> actionList(actions());

	while (m_container->count() > 0) {
		m_container->takeButton(m_container->count() - 1);
	}

	for (int i = 0; i < m_buttons.count(); ++i) {
		button = m_buttons.at(i);

		//button->setCheckable(actionList.at(i)->isCheckable());
		button->setChecked(actionList.at(i)->isChecked());

		if (actionList.at(i)->isVisible()) {
			m_container->addButton(button);
			button->show();
		} else {
			button->hide();
		}
	}

	// Resize manually since this widget is not managed by layout.
	//resize(preferredSize());
	m_container->relayout();
	setFixedSize(m_container->boundingRect().size().toSize());

	// Hide if there is no buttons.
	updateEditorItemVisibility();
}

void karin::overlay_toolbar::updateEditorItemVisibility()
{
    // Visibility of editor item is determined solely by existence of buttons.
    if (m_container->count() > 0) {
        appear();
    } else {
			disappear();
    }
}

