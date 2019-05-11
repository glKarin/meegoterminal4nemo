#include "menu.h"

#include "k.h"
#include "karin_ut.h"
#include "MTermWidget.h"
#include "menu_item.h"
#include "combobox.h"
#include "button.h"
#include "toolbar.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QDebug>

// menu, only a container
	karin::menu::menu(QGraphicsWidget *parent)
: list_view(parent)
{
	setBgColor(QColor(206, 207, 206));
	setAutoFixedSize(true);
	setOrientation(Qt::Vertical);

	setObjectName(OBJECTNAME_MENU);
}

karin::menu::~menu()
{
}

void karin::menu::addMenuItem(abstract_menu_item *item)
{
	if(!item)
		return;
	addItem(item);
	connect(item, SIGNAL(triggered(const QString &, const QVariant &)), this, SIGNAL(triggered(const QString &, const QVariant &)));
}

void karin::menu::triggeredSlot(const QString &name, const QVariant &value)
{
	emit triggered(name, value);
}



// main_menu, visible in main gui
	karin::main_menu::main_menu(QGraphicsWidget *parent)
: popup(parent),
	m_menu(new menu(this))
{
	setCentralWidget(m_menu);
	setupMenu();

	connect(m_menu, SIGNAL(triggered(const QString &, const QVariant &)), this, SIGNAL(triggered(const QString &, const QVariant &)));
	setObjectName(OBJECTNAME_MAIN_MENU);
}

karin::main_menu::~main_menu()
{
}

void karin::main_menu::setupMenu()
{
	ut * const kut = ut::Instance();

	QStringList strlist;
	int index;
	menu_item *btn;
	menu_item_switcher *switcher_btn;
	combobox *cb;

	QFont f = font();
	f.setPixelSize(32);

	{
		m_selectionButton = new combobox(this);
		m_selectionButton->setFont(f);
		m_selectionButton->addItem(tr("Scroll"), SELECTION_MODE, false);
		m_selectionButton->addItem(tr("Select"), SELECTION_MODE, true);
		m_selectionButton->setTitle(tr("Selection Mode"));
		m_selectionButton->setCurrentIndex(QList<int>() << 1);
		m_menu->addMenuItem(m_selectionButton);
	}

	{
		btn = new menu_item(this);
		btn->setFont(f);
		btn->setText(tr("New Session"));
		btn->setName(NEW_SESSION);
		m_menu->addMenuItem(btn);
	}

	{
		m_tabComboBox = new combobox(this);
		m_tabComboBox->setFont(f);
		m_tabComboBox->setTitle(tr("Tabs"));
		m_tabComboBox->setName(SELECT_TAB);
		m_menu->addMenuItem(m_tabComboBox);
	}

	{
		btn = new menu_item(this);
		btn->setFont(f);
		btn->setText(tr("Close Tab"));
		btn->setName(CLOSE_TAB);
		m_menu->addMenuItem(btn);
	}

	{
		btn = new menu_item(this);
		btn->setFont(f);
		btn->setText(tr("New Window"));
		btn->setName(NEW_WINDOW);
		m_menu->addMenuItem(btn);
	}

	{
		cb = new combobox(this);
		cb->setFont(f);
		for (int i = COLOR_SCHEME_FIRST; i <= COLOR_SCHEME_LAST; ++i)
		{
			cb->addItem(COLOR_SCHEME_NAME[i], COLOR_SCHEME_SETTING, i);
		}
		cb->setTitle(tr("Color Scheme"));
		//action->setChecked(i == ut::Instance()->getSetting<int>(COLOR_SCHEME_SETTING));
		cb->setCurrentIndex(QList<int>() << ut::Instance()->getSetting<int>(COLOR_SCHEME_SETTING) - COLOR_SCHEME_FIRST);
		m_menu->addMenuItem(cb);
	}

	{
		switcher_btn = new menu_item_switcher(this);
		switcher_btn->setFont(f);
		switcher_btn->setOn(kut->getSetting<bool>(IM_NO_PREDICTIVE));
		switcher_btn->setText(tr("IM No Predictive"));
		switcher_btn->setName(IM_NO_PREDICTIVE);
		m_menu->addMenuItem(switcher_btn);
	}

	{
		switcher_btn = new menu_item_switcher(this);
		switcher_btn->setFont(f);
		switcher_btn->setOn(kut->getSetting<bool>(BLINKING_CURSOR));
		switcher_btn->setText(tr("Blinking Cursor"));
		switcher_btn->setName(BLINKING_CURSOR);
		m_menu->addMenuItem(switcher_btn);
	}

	{
		cb = new combobox(this);
		cb->setFont(f);
		strlist.clear();
		strlist << tr("Block") << tr("Underline") << tr("I Beam");
		for (int i = 0; i < strlist.size(); ++i)
		{
			cb->addItem(strlist[i], CURSOR_TYPE, i);
		}
		cb->setTitle(tr("Terminal Cursor Type"));
		cb->setCurrentIndex(QList<int>() << ut::Instance()->getSetting<int>(CURSOR_TYPE));
		m_menu->addMenuItem(cb);
	}

	{
		cb = new combobox(this);
		cb->setFont(f);
		index = 0;
		const QStringList & tbs = toolbar::getAllToolbars();
		const QString setting = ut::Instance()->getSetting<QString>(ACTIVE_TOOLBAR);
		for(int i = 0; i < tbs.size(); i++)
		{
			QString n = QFileInfo(tbs.at(i)).baseName();
			n[0] = n[0].toUpper();
			cb->addItem(n, ACTIVE_TOOLBAR, i);
			if(tbs.at(i) == setting)
				index = i;
		}
		cb->setCurrentIndex(QList<int>() << index);
		cb->setTitle(tr("Toolbar"));
		m_menu->addMenuItem(cb);
	}

	if(0)
	{
		switcher_btn = new menu_item_switcher(this);
		switcher_btn->setFont(f);
		switcher_btn->setOn(kut->getSetting<bool>(ENABLE_VKB));
		switcher_btn->setText(tr("Enable Virtual Keyboard"));
		switcher_btn->setName(ENABLE_VKB);
		m_menu->addMenuItem(switcher_btn);
	}

	{
		btn = new menu_item(this);
		btn->setFont(f);
		btn->setText(tr("Clear"));
		btn->setName(SCREEN_CLEAR);
		m_menu->addMenuItem(btn);
	}

	{
		btn = new menu_item(this);
		btn->setFont(f);
		btn->setText(tr("Reset"));
		btn->setName(SCREEN_RESET);
		m_menu->addMenuItem(btn);
	}

	{
		btn = new menu_item(this);
		btn->setFont(f);
		btn->setText(tr("Help & About"));
		btn->setName(OPEN_ABOUT);
		m_menu->addMenuItem(btn);
	}

	{
		btn = new menu_item(this);
		btn->setFont(f);
		btn->setText(tr("Quit"));
		btn->setName(POST_EXIT);
		m_menu->addMenuItem(btn);
	}

#if 0
	{
		group = new QActionGroup(this);

		strlist << tr("Tab Mode") << tr("Tradition Mode");

		K_FOREACH_INDEX(strlist)
		{
			action = new QAction(strlist.at(i), this);
			action->setCheckable(true);
			action->setData(i);
			action->setChecked(i == ut::Instance()->getSetting<int>(TYPE_MODE));
		}

		m_menu->addActions(group->actions());

		//m_typeModeComboBox->setTitle(tr("View Mode"));
		connect(group, SIGNAL(triggered(QAction *)), this, SLOT(updateMode(QAction *)));
	}

	m_tabComboBox = new MComboBox(this);
	m_tabComboBox->setItemModel(tabModel);
	m_tabComboBox->setIconVisible(false);
	m_tabComboBox->setTitle(tr("Tabs"));
	connect(m_tabComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(showTabByIndex(int)));

	tabWidgetAction = new MWidgetAction(this);
	tabWidgetAction->setLocation(location);
	tabWidgetAction->setWidget(m_tabComboBox);
	addAction(tabWidgetAction);

	m_translucentInputMethodAction = new button_with_label(button_with_label::switchType, this);
	m_translucentInputMethodAction->setTitle(tr("Translucent Input Method"));
	m_translucentInputMethodAction->setTitleFont(font);
	m_translucentInputMethodAction->setChecked(kut->getSetting<bool>(TRANSLUCENT_INPUTMETHOD));
	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_translucentInputMethodAction);
	connect(m_translucentInputMethodAction, SIGNAL(toggled(bool)), SLOT(doTranslucencyToggled(bool)));
	addAction(widgetAction);

	m_fullScreenAction = new button_with_label(button_with_label::switchType, this);
	m_fullScreenAction->setTitle(tr("Enable FullScreen"));
	m_fullScreenAction->setTitleFont(font);
	m_fullScreenAction->setChecked(kut->getSetting<bool>(FULL_SCREEN));
	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_fullScreenAction);
	connect(m_fullScreenAction, SIGNAL(toggled(bool)), SLOT(toggleFullScreenMode(bool)));
	addAction(widgetAction);

	m_windowOrientationComboBox = new MComboBox(this);
	m_windowOrientationComboBox->addItem(tr("Automatic"));
	m_windowOrientationComboBox->addItem(tr("Lock Portrait"));
	m_windowOrientationComboBox->addItem(tr("Lock Landscape"));
	m_windowOrientationComboBox->setIconVisible(false);
	m_windowOrientationComboBox->setTitle(tr("Orientation"));
	m_windowOrientationComboBox->setCurrentIndex(ut::Instance()->getSetting<int>(ORIENTATION));
	connect(m_windowOrientationComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(setWindowOrientation(int)));

	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_windowOrientationComboBox);
	addAction(widgetAction);

	m_showBannerAction = new button_with_label(button_with_label::switchType, this);
	m_showBannerAction->setTitle(tr("Show Info Banner"));
	m_showBannerAction->setTitleFont(font);
	m_showBannerAction->setChecked(kut->getSetting<bool>(SHOW_BANNER));
	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_showBannerAction);
	connect(m_showBannerAction, SIGNAL(toggled(bool)), SLOT(setShowBanner(bool)));
	addAction(widgetAction);

	action = new MAction(tr("Help & About"), this);
	action->setLocation(location);
	addAction(action);
	connect(action, SIGNAL(triggered()), SLOT(showHelp()));

	bool b = false;
	switch(static_cast<terminal::TypeMode>(ut::Instance()->getSetting<int>(TYPE_MODE)))
	{
		case terminal::TraditionMode:
			b = true;
			break;
		case terminal::KarinMode:
		default:
			b = false;
			break;
	}
	m_addTabAction->setEnabled(b);
	m_addTabAction->setVisible(b);
	m_closeTabAction->setEnabled(b);
	m_closeTabAction->setVisible(b);
	tabWidgetAction->setEnabled(b);
	tabWidgetAction->setVisible(b);
#endif
}

void karin::main_menu::triggeredSlot(const QString &name, const QVariant &value)
{
	emit triggered(name, value);
}

void karin::main_menu::setCurrentTabIndex(int index)
{
	m_tabComboBox->setCurrentIndex(QList<int>() << index);
}

void karin::main_menu::addTab(int i, const QPair<qint64, QString> &p)
{
	m_tabComboBox->addItem(p.second, SELECT_TAB, p.first, i);
}

void karin::main_menu::updateTab(int i, const QString &label)
{
	m_tabComboBox->updateItem(i, label);
}

void karin::main_menu::setSelectionMode(bool on)
{
	m_selectionButton->setCurrentIndex(QList<int>() << (on ? 1 : 0));
}
