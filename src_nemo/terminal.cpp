#include "terminal.h"

#include "toolbar.h"
#include "key.h"
#include "tab_model.h"
#include "tab_group.h"
#include "k.h"
#include "menu.h"
#include "dialog.h"
#include "button.h"
#include "tool_icon.h"
#include "MTermWidget.h"
#include "MTerminalDisplay.h"

#include <QFileSystemWatcher>
#include <QGraphicsAnchorLayout>
#include <QGraphicsSceneResizeEvent>
#include <QApplication>
#include <QDebug>

karin::terminal::terminal(QGraphicsWidget *parent)
	: QGraphicsWidget(parent),
	m_layout(new QGraphicsAnchorLayout(this)),
	m_toolbar(new karin::toolbar(this)),
	m_menu(new karin::main_menu(this)),
	m_menuBtn(new karin::tool_icon(this)),
	m_watcher(new QFileSystemWatcher(this)),
	tabModel(new karin::tab_model(this)),
	tabGroup(new karin::tab_group(this))
{
	tabGroup->setZValue(1);

	m_toolbar->setFixedHeight(64);
	m_toolbar->setZValue(10);
	m_toolbar->setActiveToolbar(karin::ut::Instance()->getSetting<QString>(ACTIVE_TOOLBAR));

	m_menu->setZValue(20);
	m_menu->setFixedHeight(size().height() - m_toolbar->size().height());
	m_menu->setFixedWidth(480);
	m_menu->setOpacity(0.8);
	m_menu->reset();
	
	m_menuBtn->setImage("./resource/icon-m-menu.png");
	m_menuBtn->setFixedWidth(64);
	m_menuBtn->setRadius(2);
	QFont f = font();
	f.setPixelSize(48);
	m_menuBtn->setFont(f);
	m_menuBtn->setAutoRelease(false);
	m_menuBtn->setZValue(30);

	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);

	setInitLayout();
	setLayout(m_layout);

	m_watcher->addPaths(karin::toolbar::ToolbarDirectory());
	connect(m_watcher, SIGNAL(directoryChanged(const QString &)), m_toolbar, SLOT(updateToolbarDirectory(const QString &)));

	connect(m_menu, SIGNAL(triggered(const QString &, const QVariant &)), this, SLOT(triggeredSlot(const QString &, const QVariant &)));
	connect(m_menuBtn, SIGNAL(clicked(bool)), m_menu, SLOT(toggle(bool)));

	setObjectName(OBJECTNAME_TERMINAL);
}

karin::terminal::~terminal()
{
}

void karin::terminal::setInitLayout()
{
	//mode = static_cast<karin::terminal::TypeMode>(karin::ut::Instance() -> getSetting<int>(TYPE_MODE));
	// const qreal buttonOpacity = 0.5f;

#if 0
	k_menuButton = new MButton(this);
	//k_menuButton->setObjectName("m_menuButton");
	k_menuButton->setIconID("icon-m-toolbar-view-menu");
	k_menuButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	k_menuButton->setViewType(MButton::iconType);
	//k_menuButton->setOpacity(buttonOpacity);
	connect(k_menuButton, SIGNAL(clicked()), this, SLOT(openMenu()));

	k_selectionButton = new MButton(this);
	//k_selectionButton->setObjectName("m_selectionButton");
	k_selectionButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	k_selectionButton->setViewType(MButton::iconType);
	//k_selectionButton->setOpacity(buttonOpacity);
	k_selectionButton -> setIconID("icon-m-toolbar-callhistory");
	k_selectionButton -> setToggledIconID("icon-m-toolbar-trim");
	k_selectionButton->setCheckable(true);
	k_selectionButton->setChecked(false);
	connect(k_selectionButton, SIGNAL(clicked(bool)), this, SLOT(toggleSelectionMode(bool)));

	//k tabBar -> setFocusPolicy(Qt::NoFocus);
	traditionContainer = new MWidget;
	traditionContainer -> setOpacity(buttonOpacity);
	traditionContainer -> setZValue(2);
	traditionContainer -> setLayout(layout);

	connect(tabBar, SIGNAL(currentTab(qint64)), this, SLOT(showTab(qint64)));
	connect(tabBar, SIGNAL(openMenu()), this, SLOT(openMenu()));
	connect(tabBar, SIGNAL(requestNewTab()), this, SLOT(createNewTab()));

	//tab bar
	policy -> addAnchor(tabBar, Qt::AnchorLeft, policy, Qt::AnchorLeft);
	policy -> addAnchor(tabBar, Qt::AnchorTop, policy, Qt::AnchorTop);
	policy -> addAnchor(tabBar, Qt::AnchorRight, policy, Qt::AnchorRight);
#endif
	//tab group
	m_layout->addAnchor(m_menuBtn, Qt::AnchorRight, m_layout, Qt::AnchorRight);
	m_layout->addAnchor(m_menuBtn, Qt::AnchorBottom, m_layout, Qt::AnchorBottom);
	m_layout->addAnchor(m_menuBtn, Qt::AnchorTop, m_toolbar, Qt::AnchorTop);

	m_layout->addAnchor(m_toolbar, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
	m_layout->addAnchor(m_toolbar, Qt::AnchorRight, m_menuBtn, Qt::AnchorLeft);
	m_layout->addAnchor(m_toolbar, Qt::AnchorBottom, m_layout, Qt::AnchorBottom);

	m_layout->addAnchor(tabGroup, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
	m_layout->addAnchor(tabGroup, Qt::AnchorRight, m_layout, Qt::AnchorRight);
	m_layout->addAnchor(tabGroup, Qt::AnchorTop, m_layout, Qt::AnchorTop);
	m_layout->addAnchor(tabGroup, Qt::AnchorBottom, m_toolbar, Qt::AnchorTop);
#if 0
	//tradition tool
	policy -> addAnchor(traditionContainer, Qt::AnchorRight, policy, Qt::AnchorRight);
	policy -> addAnchor(traditionContainer, Qt::AnchorTop, tabBar, Qt::AnchorBottom);

	switch(mode)
	{
		case karin::terminal::TraditionMode:
			tabBar -> forbidWork();
			traditionContainer -> setEnabled(true);
			traditionContainer -> show();
			break;
		case karin::terminal::KarinMode:
		default:
			tabBar -> allowWork();
			traditionContainer -> setEnabled(false);
			traditionContainer -> hide();
			break;
	}
#endif
}

void karin::terminal::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	QGraphicsWidget::resizeEvent(event);
	m_menu->setFixedHeight(size().height() - m_toolbar->size().height());
	m_menu->reset();
	m_menuBtn->reset();
	m_layout->invalidate();
	emit sizeChanged(event->newSize());
}

void karin::terminal::init(const QString &cmd, QStringList &args)
{
	createNewTab(cmd, args);
#if 0
	karin::ut * const kut = karin::ut::Instance();
	if(kut -> getSetting<bool>(FULL_SCREEN))
		window -> showFullScreen();
	else
		window -> showNormal();
	switch(kut -> getSetting<int>(ORIENTATION))
	{
		case 1:
			window -> setPortraitOrientation();
			window -> lockOrientation();
			break;
		case 2:
			window -> setLandscapeOrientation();
			window -> lockOrientation();
			break;
		case 0:
		default:
			if(window -> isOrientationLocked())
				window -> unlockOrientation();
			break;
	}
#endif
}

void karin::terminal::createNewTab(const QString &cmd, QStringList &args)
{
	int index;

	index = -1;
	qint64 id = tabModel->genTabId();
	MTermWidget *wid = tabGroup->addTab(id, &index);
	if(!cmd.isEmpty())
	{
		wid->setShellProgram(cmd);
		wid->setArgs(args);
	}
	wid->startShellProgram();
#if 0
	karin::tab_button *bt = tabBar -> addTab(id);
	int index = tabBar -> currentIndex();
#endif
	tabModel->insertTab(id, QString(), index);
	//k bt -> setFocusProxy(wid);
	//k bt -> doSelectionModeChange(wid -> m_selectionButton, id);
	connect(wid, SIGNAL(selectionModeChanged(bool, qint64)), this, SLOT(syncSelectionModeStatus(bool, qint64)));
	//k connect(bt, SIGNAL(requestSelectionMode(bool, qint64)), this, SLOT(setSelectionMode(bool, qint64)));
	connect(wid, SIGNAL(titleChanged(const QString &, qint64)), this, SLOT(syncTitle(const QString &, qint64)));
	connect(wid, SIGNAL(sessionFinished(qint64)), this, SLOT(destroyOldTab(qint64)));
	//k connect(bt, SIGNAL(closeTab(qint64)), this, SLOT(destroyOldTab(qint64)));
	m_menu->addTab(index, tabModel->getTabInfo(index));
	showTab(id);
	wid->doTitleChanged();
}

void karin::terminal::triggeredSlot(const QString &name, const QVariant &value)
{
	if(name == NEW_WINDOW)
		createNewWindow();
	else if(name == POST_EXIT)
		QApplication::quit();
	else if(name == SCREEN_CLEAR)
		clearScreen();
	else if(name == SCREEN_RESET)
		resetScreen();
	else if(name == OPEN_ABOUT)
	{
		showHelp();
		closeMenu();
	}
	else if(name == BLINKING_CURSOR)
		setBlinkingCursor(value.toBool());
	else if(name == IM_NO_PREDICTIVE)
		setIMNoPredictive(value.toBool());
	else if(name == COLOR_SCHEME_SETTING)
		onColorSchemeCbxCurrentIndexChanged(value.toInt());
	else if(name == CURSOR_TYPE)
		setTerminalCursor(value.toInt());
	else if(name == ACTIVE_TOOLBAR)
		setActiveToolbarIndex(value.toInt());
	else if(name == NEW_SESSION)
	{
		QStringList list;
		createNewTab(QString(), list);
		closeMenu();
	}
	else if(name == CLOSE_TAB)
	{
		destroyCurrentTab();
		closeMenu();
	}
	else if(name == SELECT_TAB)
	{
		showTab(value.value<qint64>());
		closeMenu();
	}
	else if(name == SELECTION_MODE)
	{
		toggleSelectionMode(value.toBool());
		closeMenu();
	}

}

void karin::terminal::createNewWindow()
{
	static const QString myProgName = QApplication::arguments()[0];

	bool result = QProcess::startDetached(myProgName, QStringList() << "-n");
	if (!result)
		qWarning() << tr("Failed creating new window!");
	closeMenu();
}

void karin::terminal::clearScreen() SETTING_FOR_CUR_INSTANCE
{
	MTermWidget *wid = tabGroup->currentWidget();
	if(wid)
		wid->clearScreen();
	closeMenu();
	grabFocusForTab();
}

void karin::terminal::resetScreen() SETTING_FOR_CUR_INSTANCE
{
	MTermWidget *wid = tabGroup->currentWidget();
	if(wid)
		wid->resetScreen();
	closeMenu();
	grabFocusForTab();
}

void karin::terminal::setBlinkingCursor(bool b) SETTING_FOR_ALL_INSTANCE
{
	karin::ut::Instance()->setSetting<bool>(BLINKING_CURSOR, b);
	QList<MTermWidget *> vec = tabGroup->tabs();
	foreach(MTermWidget *wid, vec)
	{
		if(wid && wid->m_display)
			wid->m_display->setBlinkingCursor(b);
	}
}

void karin::terminal::setIMNoPredictive(bool b) SETTING_FOR_GLO_INSTANCE
{
	karin::ut::Instance()->setSetting<bool>(IM_NO_PREDICTIVE, b);
	QList<MTermWidget *> vec = tabGroup->tabs();
	foreach(MTermWidget *wid, vec)
	{
		if(wid && wid->m_display)
			wid->m_display->setInputMethodNoPredictive(b);
	}
}

void karin::terminal::closeMenu()
{
	m_menuBtn->reset();
	m_menu->close();
}

void karin::terminal::openMenu()
{
}

void karin::terminal::onColorSchemeCbxCurrentIndexChanged(int index) SETTING_FOR_ALL_INSTANCE
{
	if(index >= COLOR_SCHEME_FIRST && index <= COLOR_SCHEME_LAST)
	{
		karin::ut::Instance() -> setSetting<int>(COLOR_SCHEME_SETTING, index);
		QList<MTermWidget *> vec = tabGroup->tabs();
		foreach(MTermWidget *wid, vec)
		{
			if(wid)
				wid->setColorScheme(index/* + 1*/);
		}
	}
}

void karin::terminal::setTerminalCursor(int index) SETTING_FOR_ALL_INSTANCE
{
	if(index >= 0 && index < 3)
	{
		karin::ut::Instance()->setSetting<int>(CURSOR_TYPE, index);

		QList<MTermWidget *> vec = tabGroup->tabs();
		foreach(MTermWidget *wid, vec)
		{
			if(wid && wid->m_display)
				wid->m_display->setKeyboardCursorShape(static_cast<Konsole::TerminalDisplay::KeyboardCursorShape>(index));
		}
	}
}

void karin::terminal::setActiveToolbarIndex(int index)
{
	const QStringList & tbs = karin::toolbar::getAllToolbars();
	if(index >= 0 && index < tbs.size())
	{
		karin::ut::Instance()->setSetting<QString>(ACTIVE_TOOLBAR, tbs.at(index));
		m_toolbar->setActiveToolbarIndex(index);
	}
}

void karin::terminal::syncTitle(const QString &title, qint64 id)
{
#if 0
	karin::tab_button *b = (*tabBar)[id];
	if(b)
		b -> setLabel(title, id);
#endif
	tabModel->updateTab(id, title);
	m_menu->updateTab(tabModel->indexTab(id), title);
}

void karin::terminal::destroyOldTab(qint64 id)
{
	if(id < 0)
		return;
	tabModel -> removeTab(id);
	//k if(tabBar -> currentTabId() == id)
	if(tabGroup->currentTabId() == id)
	{
		//k qint64 newId = tabBar -> nextTab(id);
		qint64 newId = tabGroup->nextTab(id);
		if(newId != -1)
		{
			//k tabBar -> setCurrentWidget(newId);
			//k tabBar -> updateTabPosition();
			showTab(newId);
			//tabBar -> showTab(newId);
			//tabGroup -> currentWidget() -> setFocus(Qt::MouseFocusReason);
		}
	}
	tabGroup -> removeTab(id);
	//k tabBar -> closeTab(id);
	if(tabGroup->tabs().isEmpty())
	{
		emit finished();
		return;
	}
	grabFocusForTab();
	//tabGroup -> setFocus(Qt::MouseFocusReason);
}

void karin::terminal::destroyCurrentTab()
{
	//k qint64 id = tabBar->currentTabId();
	qint64 id = tabGroup->currentTabId();
	if(id < 0)
		return;
	tabModel->removeTab(id);
	if(tabGroup->currentTabId() == id)
	{
		//k qint64 newId = tabBar -> nextTab(id);
		qint64 newId = tabGroup->nextTab(id);
		if(newId != -1)
		{
			//k tabBar -> setCurrentWidget(newId);
			//k tabBar -> updateTabPosition();
			showTab(newId);
			//tabBar -> showTab(newId);
			//tabGroup -> currentWidget() -> setFocus(Qt::MouseFocusReason);
		}
	}
	tabGroup->removeTab(id);
	//k tabBar -> closeTab(id);
	if(tabGroup->tabs().isEmpty())
	{
		emit finished();
		return;
	}
	grabFocusForTab();
	//tabGroup -> setFocus(Qt::MouseFocusReason);
}
void karin::terminal::showTab(qint64 id)
{
	MTermWidget *wid;

	if(id < 0)
		return;
	if(id == tabGroup->currentTabId())
		return;
	wid = tabGroup->currentWidget();
	if(wid)
	{
		wid->m_display->setGrabFocus(false);
		wid->m_display->bindToolbar(0);
		m_toolbar->bindTerminalDisplay(0);
	}
	tabGroup->showTab(id);
	wid = tabGroup->currentWidget();
	if(wid)
	{
		wid->m_display->setGrabFocus(true);
		//k k_selectionButton -> setChecked(wid -> m_selectionButton);
		//k wid->onInputMethodAreaChanged();

		m_toolbar->bindTerminalDisplay(wid->m_display);
		wid->m_display->bindToolbar(m_toolbar);
		//k wid->setFocus(Qt::MouseFocusReason);
     //////this->setFocusPolicy( m_terminalDisplay->focusPolicy() ); 

		m_menu->setSelectionMode(wid->m_selectionButton);
	}
	m_menu->setCurrentTabIndex(tabModel->indexTab(id));
}

void karin::terminal::openLink(const QString& link) const
{
	QDesktopServices::openUrl(QUrl(link));
}

void karin::terminal::showTabByIndex(int index)
{
	qint64 id = tabModel->valueTab(index);
	if(id < 0)
		return;
	if(id == tabGroup->currentTabId())
		return;
	/*
		 karin::tab_button *b = tabBar -> currentLabel();
		 if(b)
		 b -> setHighlight(false);
		 tabBar -> setCurrentWidget(id);
	//tabBar -> updateTabPosition();
	*/
	//k tabBar->showTab(id);
	tabGroup->showTab(id);
}

void karin::terminal::toggleSelectionMode(bool b)
{
	MTermWidget *wid = tabGroup->currentWidget();
	if(wid)
	{
		wid->toggleSelectionModeWithBanner(b, tabGroup->currentTabId());
	}
}

void karin::terminal::syncSelectionModeStatus(bool b, qint64 id)
{
	if(id < 0)
		return;
	/*
	if((*tabBar)[id])
		(*tabBar)[id] -> doSelectionModeChange(b, id);
		*/
	if(id == tabGroup->currentTabId())
		m_menu->setSelectionMode(b);
}

void karin::terminal::ungrabFocusForTab()
{
	MTermWidget *wid = tabGroup->currentWidget();
	if(wid)
	{
		wid->m_display->setGrabFocus(false);
		wid->clearFocus();
	}
	QApplication::inputMethod()->hide();
}

void karin::terminal::grabFocusForTab()
{
	MTermWidget *wid = tabGroup->currentWidget();
	if(wid)
	{
		wid->m_display->setGrabFocus(true);
		if(!wid->hasFocus())
			wid->setFocus(Qt::MouseFocusReason);
	}
	QApplication::inputMethod()->show();
}

void karin::terminal::showHelp()
{
	QString text;

	text += QString(APP_NAME) + "<br><br>";
	text += tr("Version") + " : " + VERSION + "<br><br><br>";

	text += karin::ut::GetKarinConsoleHelp();

	text += "<br><br><br>";

	text += karin::ut::GetMeeGoTerminalHelp();

	karin::msg_dialog *dialog = new karin::msg_dialog(QString(APP_NAME).replace("-", ""), text, this);

	//connect(dialog, SIGNAL(destroyed()), this, SLOT(grabFocusForTab()));
	connect(dialog, SIGNAL(linkActivated(const QString &)), this, SLOT(openLink(const QString &)));
	dialog->appear(karin::timer_popup::DestroyWhenDone);
}
