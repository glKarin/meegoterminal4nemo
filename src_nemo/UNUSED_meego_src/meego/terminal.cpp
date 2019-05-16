#include "terminal.h"
#include "tab_button.h"
#include "tab_bar.h"
#include "tab_group.h"
#include "tab_model.h"
#include "karin_ut.h"
#include "button_with_label.h"
#include "MTermWidget.h"
#include "MTerminalDisplay.h"

#include <MLinearLayoutPolicy>
#include <MLayout>
#include <MComboBox>
#include <MAction>
#include <MWidgetAction>
#include <MDialog>
#include <MApplicationWindow>
#include <MApplication>
#include <MLabel>
#include <QDesktopServices>
#include <QDateTime>
#include <QFileSystemWatcher>
#include <MButton>
#include <QGraphicsAnchorLayout>

#include <QDebug>

#ifdef APP_NAME
#undef APP_NAME
#endif
#define APP_NAME "Karin Console"

	karin::terminal::terminal(QGraphicsItem *parent)
:MApplicationPage(parent),
	mode(karin::terminal::KarinMode),
	tabModel(0),
	tabBar(0),
	tabGroup(0),
	policy(0),
	m_toolbarsComboBox(0),
	m_colorSchemeComboBox(0),
	m_fullScreenAction(0),
	m_showBannerAction(0),
	m_windowOrientationComboBox(0),
	m_translucentInputMethodAction(0),
	m_enableVirtualKeyboardAction(0),
	watcher(0),
	m_typeModeComboBox(0),
	m_addTabAction(0),
	m_closeTabAction(0),
	m_tabComboBox(0),
	k_menuButton(0),
	k_selectionButton(0),
	layout(0),
	portPolicy(0),
	landPolicy(0),
	traditionContainer(0),
	tabWidgetAction(0),
	m_cursorComboBox(0),
	m_blinkingCursorAction(0)
{
	setTitle("Karin Console");
	setPannable(false);
	setComponentsDisplayMode(MApplicationPage::NavigationBar, MApplicationPageModel::Hide);
	tabModel = new karin::tab_model(this);
	//tabGroup -> setHeaderVisible(false);
	setInitLayout();
}

karin::terminal::~terminal()
{
}

void karin::terminal::init(const QString &cmd, QStringList &args)
{
	setupMenu();
	if(!cmd.isEmpty())
		createNewTab(cmd, args);
	else
		createNewTab();
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(!window)
		return;
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
}

void karin::terminal::createNewTab(const QString &cmd, QStringList &args)
{
	//避免出现重复ID
	qint64 id = QDateTime::currentMSecsSinceEpoch();
	while(tabModel -> hasTab(id))
		id = QDateTime::currentMSecsSinceEpoch();
	MTermWidget *wid = tabGroup -> addTab(id);
	wid -> setShellProgram(cmd);
	wid -> setArgs(args);
	wid -> startShellProgram();
	//取得tab bar当前标签的位置
	karin::tab_button *bt = tabBar -> addTab(id);
	int index = tabBar -> currentIndex();
	//插入tab model
	tabModel -> insertTab(id, QString(), index);
	//关联焦点
	bt -> setFocusProxy(wid);
	//标签与组件同步选择模式状态
	bt -> doSelectionModeChange(wid -> m_selectionButton, id);
	//选择模式同步交互
	connect(wid, SIGNAL(selectionModeChanged(bool, qint64)), this, SLOT(syncSelectionModeStatus(bool, qint64)));
	connect(bt, SIGNAL(requestSelectionMode(bool, qint64)), this, SLOT(setSelectionMode(bool, qint64)));
	//标题同步
	connect(wid, SIGNAL(titleChanged(const QString &, qint64)), this, SLOT(syncTitle(const QString &, qint64)));
	//处理关闭请求
	connect(wid, SIGNAL(sessionFinished(qint64)), this, SLOT(destroyOldTab(qint64)));
	connect(bt, SIGNAL(closeTab(qint64)), this, SLOT(destroyOldTab(qint64)));
	//显示新的tab
	showTab(id);
	//触发同步标题信号
	wid -> doTitleChanged();
}


void karin::terminal::createNewTab()
{
	//避免出现重复ID
	qint64 id = QDateTime::currentMSecsSinceEpoch();
	while(tabModel -> hasTab(id))
		id = QDateTime::currentMSecsSinceEpoch();
	MTermWidget *wid = tabGroup -> addTab(id);
	wid -> startShellProgram();
	//取得tab bar当前标签的位置
	karin::tab_button *bt = tabBar -> addTab(id);
	int index = tabBar -> currentIndex();
	//插入tab model
	tabModel -> insertTab(id, QString(), index);
	//关联焦点
	bt -> setFocusProxy(wid);
	bt -> doSelectionModeChange(wid -> m_selectionButton, id);
	//标签与组件同步选择模式状态
	connect(wid, SIGNAL(selectionModeChanged(bool, qint64)), this, SLOT(syncSelectionModeStatus(bool, qint64)));
	connect(bt, SIGNAL(requestSelectionMode(bool, qint64)), this, SLOT(setSelectionMode(bool, qint64)));
	//标题同步
	connect(wid, SIGNAL(titleChanged(const QString &, qint64)), this, SLOT(syncTitle(const QString &, qint64)));
	//处理关闭请求
	connect(wid, SIGNAL(sessionFinished(qint64)), this, SLOT(destroyOldTab(qint64)));
	connect(bt, SIGNAL(closeTab(qint64)), this, SLOT(destroyOldTab(qint64)));
	//显示新的tab
	showTab(id);
	//触发同步标题信号
	wid -> doTitleChanged();
}

void karin::terminal::showTab(qint64 id)
{
	if(id < 0)
		return;
	//隐藏之前的tab，显示新的tab ？？ ！=
	if(id == tabGroup -> currentTabId())
		return;
	tabGroup -> showTab(id);
	MTermWidget *wid = tabGroup -> currentWidget();
	if(wid)
	{
		//组件取得焦点
		wid -> setFocus(Qt::MouseFocusReason);
		//同步选择模式按钮状态
		k_selectionButton -> setChecked(wid -> m_selectionButton);
		wid -> onInputMethodAreaChanged(QRect());
	}
	//tab combobox更新当前索引
	if(tabModel -> valueTab(m_tabComboBox -> currentIndex()) != id)
		m_tabComboBox -> setCurrentIndex(tabModel -> indexTab(id));
}

void karin::terminal::destroyOldTab(qint64 id)
{
	if(id < 0)
		return;
	//从tab model移除
	tabModel -> removeTab(id);
	//如果是当前显示
	if(tabBar -> currentTabId() == id)
	{
		//取得按顺序下一tab，如果有则显示
		qint64 newId = tabBar -> nextTab(id);
		if(newId != -1)
		{
			//？？tab bar 触发信号？
			tabBar -> setCurrentWidget(newId);
			tabBar -> updateTabPosition();
			showTab(newId);
			//tabBar -> showTab(newId);
			//tabGroup -> currentWidget() -> setFocus(Qt::MouseFocusReason);
		}
	}
	tabGroup -> removeTab(id);
	tabBar -> closeTab(id);
	//如果无更多tab，触发结束信号
	if(tabGroup -> getAllTabs().isEmpty())
	{
		emit finished();
		return;
	}
	//当前tab组件重新获取焦点
	grabFocusForTab();
	//tabGroup -> setFocus(Qt::MouseFocusReason);
}

void karin::terminal::destroyCurrentTab()
{
	//取得当前tab的ID
	qint64 id = tabBar -> currentTabId();
	if(id < 0)
		return;
	//同自由移除操作
	tabModel -> removeTab(id);
	if(tabGroup -> currentTabId() == id)
	{
		qint64 newId = tabBar -> nextTab(id);
		if(newId != -1)
		{
			//？？tab bar 触发信号？
			tabBar -> setCurrentWidget(newId);
			tabBar -> updateTabPosition();
			showTab(newId);
			//tabBar -> showTab(newId);
			//tabGroup -> currentWidget() -> setFocus(Qt::MouseFocusReason);
		}
	}
	tabGroup -> removeTab(id);
	tabBar -> closeTab(id);
	if(tabGroup -> getAllTabs().isEmpty())
	{
		emit finished();
		return;
	}
	grabFocusForTab();
	//tabGroup -> setFocus(Qt::MouseFocusReason);
}

void karin::terminal::setupMenu()
{
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if (!window) {
		qFatal("No window!");
		return;
	}
	MAction::Locations location = MAction::ApplicationMenuLocation;

	MAction *action = 0;
	MWidgetAction *widgetAction = 0;
	const QFont font("", 18, QFont::Bold);
	karin::ut * const kut = karin::ut::Instance();

	m_typeModeComboBox = new MComboBox(this);
	m_typeModeComboBox -> addItem(tr("Tab Mode"));
	m_typeModeComboBox -> addItem(tr("Tradition Mode"));
	m_typeModeComboBox -> setIconVisible(false);
	m_typeModeComboBox -> setTitle(tr("View Mode"));
	m_typeModeComboBox -> setCurrentIndex(karin::ut::Instance() -> getSetting<int>(TYPE_MODE));
	connect(m_typeModeComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(updateMode(int)));

	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_typeModeComboBox);
	addAction(widgetAction);

	action = new MAction(tr("New Window"), this);
	action -> setLocation(location);
	addAction(action);
	connect(action, SIGNAL(triggered()), SLOT(createNewWindow()));

	m_addTabAction = new MAction(tr("New Tab"), this);
	m_addTabAction -> setLocation(location);
	addAction(m_addTabAction);
	connect(m_addTabAction, SIGNAL(triggered()), SLOT(createNewTab()));

	m_closeTabAction = new MAction(tr("Close Current Tab"), this);
	m_closeTabAction -> setLocation(location);
	addAction(m_closeTabAction);
	connect(m_closeTabAction, SIGNAL(triggered()), SLOT(destroyCurrentTab()));

	m_tabComboBox = new MComboBox(this);
	m_tabComboBox -> setItemModel(tabModel);
	m_tabComboBox -> setIconVisible(false);
	m_tabComboBox -> setTitle(tr("Tabs"));
	connect(m_tabComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(showTabByIndex(int)));

	tabWidgetAction = new MWidgetAction(this);
	tabWidgetAction->setLocation(location);
	tabWidgetAction->setWidget(m_tabComboBox);
	addAction(tabWidgetAction);

	action = new MAction(tr("Clear"), this);
	action -> setLocation(location);
	addAction(action);
	connect(action, SIGNAL(triggered()), SLOT(clearScreen()));

	action = new MAction(tr("Reset"), this);
	action -> setLocation(location);
	addAction(action);
	connect(action, SIGNAL(triggered()), SLOT(resetScreen()));

	m_toolbarsComboBox = new MComboBox(this);
	const QStringList & tbs = Toolbar::getAllToolbars();
	const QString setting = karin::ut::Instance() -> getSetting<QString>(ACTIVE_TOOLBAR);
	for(int i = 0; i < tbs.size(); i++)
	{
		QString n = QFileInfo(tbs.at(i)).baseName();
		n[0] = n[0].toUpper();
		m_toolbarsComboBox->addItem(n);
		if(tbs.at(i) == setting)
			m_toolbarsComboBox->setCurrentIndex(i);
	}
	m_toolbarsComboBox->setIconVisible(false);
	m_toolbarsComboBox->setTitle(tr("Toolbar"));
	connect(m_toolbarsComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(setActiveToolbarIndex(int)));

	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_toolbarsComboBox);
	addAction(widgetAction);

	watcher = new QFileSystemWatcher(this);
	watcher -> addPath(QString(_KARIN_PREFIX_) + IM_TOOLBARS_DIR);
	connect(watcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(updateToolbarDirectory(const QString &)));

	m_colorSchemeComboBox = new MComboBox(this);
	for (int i = COLOR_SCHEME_FIRST; i <= COLOR_SCHEME_LAST; ++i)
		m_colorSchemeComboBox->addItem(COLOR_SCHEME_NAME[i]);
	m_colorSchemeComboBox->setIconVisible(false);
	m_colorSchemeComboBox->setTitle(tr("Color Scheme"));
	m_colorSchemeComboBox -> setCurrentIndex(karin::ut::Instance() -> getSetting<int>(COLOR_SCHEME_SETTING));
	connect(m_colorSchemeComboBox, SIGNAL(currentIndexChanged(int)),
			SLOT(onColorSchemeCbxCurrentIndexChanged(int)));

	widgetAction = new MWidgetAction(this);
	widgetAction -> setLocation(location);
	widgetAction -> setWidget(m_colorSchemeComboBox);
	addAction(widgetAction);

	m_translucentInputMethodAction = new karin::button_with_label(karin::button_with_label::switchType, this);
	m_translucentInputMethodAction -> setTitle(tr("Translucent Input Method"));
	m_translucentInputMethodAction -> setTitleFont(font);
	m_translucentInputMethodAction -> setChecked(kut -> getSetting<bool>(TRANSLUCENT_INPUTMETHOD));
	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_translucentInputMethodAction);
	connect(m_translucentInputMethodAction, SIGNAL(toggled(bool)), SLOT(doTranslucencyToggled(bool)));
	addAction(widgetAction);

	m_enableVirtualKeyboardAction = new karin::button_with_label(karin::button_with_label::switchType, this);
	m_enableVirtualKeyboardAction -> setTitle(tr("Enable Virtual Keyboard"));
	m_enableVirtualKeyboardAction -> setTitleFont(font);
	m_enableVirtualKeyboardAction -> setChecked(kut -> getSetting<bool>(ENABLE_VKB));
	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_enableVirtualKeyboardAction);
	connect(m_enableVirtualKeyboardAction, SIGNAL(toggled(bool)), SLOT(doSuppressionToggled(bool)));
	addAction(widgetAction);

	m_cursorComboBox = new MComboBox(this);
	m_cursorComboBox->addItem(tr("Block"));
	m_cursorComboBox->addItem(tr("Underline"));
	m_cursorComboBox->addItem(tr("I Beam"));
	m_cursorComboBox -> setIconVisible(false);
	m_cursorComboBox->setTitle(tr("Terminal Cursor Type"));
	m_cursorComboBox -> setCurrentIndex(karin::ut::Instance() -> getSetting<int>(CURSOR_TYPE));
	connect(m_cursorComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(setTerminalCursor(int)));

	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_cursorComboBox);
	addAction(widgetAction);

	m_blinkingCursorAction = new karin::button_with_label(karin::button_with_label::switchType, this);
	m_blinkingCursorAction -> setTitle(tr("Blinking Cursor"));
	m_blinkingCursorAction -> setTitleFont(font);
	m_blinkingCursorAction -> setChecked(kut -> getSetting<bool>(BLINKING_CURSOR));
	widgetAction = new MWidgetAction(this);
	widgetAction -> setLocation(location);
	widgetAction -> setWidget(m_blinkingCursorAction);
	connect(m_blinkingCursorAction, SIGNAL(toggled(bool)), this, SLOT(setBlinkingCursor(bool)));
	addAction(widgetAction);

	m_fullScreenAction = new karin::button_with_label(karin::button_with_label::switchType, this);
	m_fullScreenAction -> setTitle(tr("Enable FullScreen"));
	m_fullScreenAction -> setTitleFont(font);
	m_fullScreenAction -> setChecked(kut -> getSetting<bool>(FULL_SCREEN));
	widgetAction = new MWidgetAction(this);
	widgetAction -> setLocation(location);
	widgetAction -> setWidget(m_fullScreenAction);
	connect(m_fullScreenAction, SIGNAL(toggled(bool)), SLOT(toggleFullScreenMode(bool)));
	addAction(widgetAction);

	m_windowOrientationComboBox = new MComboBox(this);
	m_windowOrientationComboBox->addItem(tr("Automatic"));
	m_windowOrientationComboBox->addItem(tr("Lock Portrait"));
	m_windowOrientationComboBox->addItem(tr("Lock Landscape"));
	m_windowOrientationComboBox -> setIconVisible(false);
	m_windowOrientationComboBox->setTitle(tr("Orientation"));
	m_windowOrientationComboBox -> setCurrentIndex(karin::ut::Instance() -> getSetting<int>(ORIENTATION));
	connect(m_windowOrientationComboBox, SIGNAL(currentIndexChanged(int)),
			this, SLOT(setWindowOrientation(int)));

	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_windowOrientationComboBox);
	addAction(widgetAction);

	m_showBannerAction = new karin::button_with_label(karin::button_with_label::switchType, this);
	m_showBannerAction -> setTitle(tr("Show Info Banner"));
	m_showBannerAction -> setTitleFont(font);
	m_showBannerAction -> setChecked(kut -> getSetting<bool>(SHOW_BANNER));
	widgetAction = new MWidgetAction(this);
	widgetAction->setLocation(location);
	widgetAction->setWidget(m_showBannerAction);
	connect(m_showBannerAction, SIGNAL(toggled(bool)), SLOT(setShowBanner(bool)));
	addAction(widgetAction);

	action = new MAction(tr("Help & About"), this);
	action -> setLocation(location);
	addAction(action);
	connect(action, SIGNAL(triggered()), SLOT(showHelp()));

	bool b = false;
	switch(static_cast<karin::terminal::TypeMode>(karin::ut::Instance() -> getSetting<int>(TYPE_MODE)))
	{
		case karin::terminal::TraditionMode:
			b = true;
			break;
		case karin::terminal::KarinMode:
		default:
			b = false;
			break;
	}
	m_addTabAction -> setEnabled(b);
	m_addTabAction -> setVisible(b);
	m_closeTabAction -> setEnabled(b);
	m_closeTabAction -> setVisible(b);
	tabWidgetAction -> setEnabled(b);
	tabWidgetAction -> setVisible(b);
}

void karin::terminal::openMenu()
{
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(window)
		window -> openMenu();
}

void karin::terminal::showHelp()
{
	QString text;

	text += QString(APP_NAME) + " - " + tr("Help") + "<br><br>";
	text += tr("Version") + " : " + VERSION + "<br><br>";

	text += tr("Click '+' button to create a new tab. In tab label, you can set selection mode, show this tab and close this tab. Current tab label is highlight.") + "<br><br>";
	text += tr("Also change Tab view type or traditional view type.") + "<br><br>";
	text += tr("Fixed copy strings that include wide charactor, like Chinese and so on.") + "<br><br>";
	text += tr("Change active toolbar with swipe gesture, it is active for current tab. If you want to save to settings, must set it in menu, and it will change active toolbar of current tab and new tabs.") + "<br><br>";
	text += tr("Change font size with pinch gesture, it is will save to settings, and it will only be active for new tabs.") + "<br><br>";
	text += tr("Change color scheme with menu, it will save to settings and be active for all tabs that include old tabs and new tabs.") + "<br><br>";
	text += tr("You can set window orientation, and show or hide info banner.") + "<br><br>";
	text += tr("You can show or hide virtual keyboard, and make virtual keyboard translucent. It will save to settings and make active for all tabs.") + "<br><br>";
	text += tr("You can choose cursor shape, and set cursor is blinking. It will save to settings and make active for all tabs.") + "<br><br>";
	text += tr("Add clear screen echo or reset session.") + "<br><br>";
	text += tr("Add a new default toolbar named \"karin\", support portrait and landscape orientation.") + "<br><br>";
	text += tr("Special Thanks: daols (DOSPY) for testing and desktop icon.") + "<br><br>";
	text += QString("<a href=mailto:beyondk2000@gmail.com>%1</a> @ %2").arg(tr("Karin")).arg("2015");

	text += "<br><br><br>";

	text += QString(
			"MeeGo Terminal - Help<br><br>"
			"Swipe down on the virtual keyboard to hide it.<br><br>"
			"Semi-transparent button in the top-right corner opens the menu. "
			"The button under it toggles the text selection mode.<br><br>"
			"The text selection mode enables you to select and copy "
			"text. Double-tap to select a word, triple-tap to select an entire "
			"line. To enable column/block selection mode press Ctrl+Alt keys "
			"before commencing the selection. Copied text remains in the "
			"clipboard, so it can be pasted multiple times. Use the 'Clear' "
			"button to empty the clipboard contents.<br><br>"
			"While the text selection mode is disabled these gestures are "
			"supported by the terminal window (not the virtual keyboard):<br><br>"
			"Pinch to change font size.<br><br>"
			"Swipe horizontally to change toolbar.<br><br>"
			"Toolbars are specified in XML and are located in %1. Number of "
			"supported toolbars is unlimited, so you can add your own toolbars "
			"to that directory. For the toolbar XML format specification see "
			"<a href='%2'>MeeGo IM Framework - Toolbar XML document</a>"
			).arg(QString(_KARIN_PREFIX_) + "/toolbar/")
#ifdef _KARIN_LOCAL_
		.arg(QString(_KARIN_PREFIX_) + "/src_meegotouch/resource/toolbarxml.html");
#else
		.arg(QString(_KARIN_PREFIX_) + "/resource/toolbarxml.html");
#endif

	MLabel *label = new MLabel(text, this);
	label->setStyleName("CommonBodyTextInverted");
	label->setAlignment(Qt::AlignCenter);
	label->setWordWrap(true);
	label->setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	connect(label, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));

	MDialog *dialog = new MDialog(QString(APP_NAME), M::NoStandardButton);
	dialog->setCentralWidget(label);
	dialog->setTitleBarIconId("icon-l-terminal");
	connect(dialog, SIGNAL(destroyed()), this, SLOT(grabFocusForTab()));
	dialog->appear(MSceneWindow::DestroyWhenDone);
}

void karin::terminal::toggleFullScreenMode(bool fullScreen)
{
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(!window)
		return;

	if(fullScreen)
	{
		window -> showFullScreen();
	}
	else
	{
		window -> showNormal();
	}
	karin::ut::Instance() -> setSetting<bool>(FULL_SCREEN, fullScreen);
	window -> closeMenu();
	grabFocusForTab();
}

void karin::terminal::createNewWindow() const
{
	static const QString myProgName = QApplication::arguments()[0];

	bool result = QProcess::startDetached(myProgName, QStringList() << "-n");
	if (!result)
		qWarning("Failed creating new window!");
}

void karin::terminal::openLink(const QString& link) const
{
	QDesktopServices::openUrl(QUrl(link));
}

void karin::terminal::setActiveToolbarIndex(int index)
{
	if(index >= 0 && index < Toolbar::getAllToolbars().size())
	{
		karin::ut::Instance() -> setSetting<QString>(ACTIVE_TOOLBAR, Toolbar::getAllToolbars().at(index));
		//只修改当前tab组件的工具条
		MTermWidget *wid = dynamic_cast<MTermWidget *>(tabGroup -> currentWidget());
		if(wid && wid -> m_display)
			wid -> m_display -> setActiveToolbarIndex(index);
	}
	grabFocusForTab();
}

void karin::terminal::onColorSchemeCbxCurrentIndexChanged(int index)
{
	if(index >= 0 && index < 4)
	{
		karin::ut::Instance() -> setSetting<int>(COLOR_SCHEME_SETTING, index);
		//配色方案全局生效
		QVector<MTermWidget *> vec = tabGroup -> getAllTabs();
		foreach(MTermWidget *wid, vec)
			if(wid)
				wid -> setColorScheme(index + 1);
	}
	grabFocusForTab();
}

void karin::terminal::setShowBanner(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(SHOW_BANNER, b);
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(window)
		window -> closeMenu();
	grabFocusForTab();
}

void karin::terminal::setWindowOrientation(int i)
{
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(!window)
		return;
	switch(i)
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
	karin::ut::Instance() -> setSetting<int>(ORIENTATION, i);
	grabFocusForTab();
}

void karin::terminal::setSelectionMode(bool b, qint64 id)
{
	if(id < 0)
		return;
	//如果是当前tab组件
	if(id == tabBar -> currentTabId())
	{
		//修改组件的选择模式状态
		MTermWidget *wid = (*tabGroup)[id];
		if(wid)
			wid -> toggleSelectionModeWithBanner(b, id);
		//同步选择模式按钮状态
		if(id == tabGroup -> currentTabId())
			k_selectionButton -> setChecked(wid -> m_selectionButton);
	}
	else
	{
		//如果不是当前tab组件，如点击非当前的tab标签，则不去修改选择模式状态，而进行切换标签操作
		tabBar -> showTab(id);
		if(tabBar -> currentLabel() && tabGroup -> currentWidget())
			tabBar -> currentLabel() -> doSelectionModeChange(tabGroup -> currentWidget() -> m_selectionButton, id);
	}
}

void karin::terminal::doTranslucencyToggled(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(TRANSLUCENT_INPUTMETHOD, b);
	QVector<MTermWidget *> vec = tabGroup -> getAllTabs();
	//透明键盘全局生效
	foreach(MTermWidget *wid, vec)
		if(wid)
			wid -> setTranslucency(b);
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(window)
		window -> closeMenu();
	grabFocusForTab();
}

void karin::terminal::doSuppressionToggled(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(ENABLE_VKB, b);
	QVector<MTermWidget *> vec = tabGroup -> getAllTabs();
	//禁用键盘全局生效
	foreach(MTermWidget *wid, vec)
		if(wid)
			wid -> setSuppression(b);
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(window)
		window -> closeMenu();
	grabFocusForTab();
}

void karin::terminal::updateToolbarDirectory(const QString &path)
{
	if(QString(_KARIN_PREFIX_) + IM_TOOLBARS_DIR != path)
		return;
	if(!m_toolbarsComboBox)
		return;
	//m_toolbarsComboBox -> setCurrentIndex(-1);
	m_toolbarsComboBox -> clear();
	const QStringList & tbs = Toolbar::getAllToolbars(true);
	const QString tSetting = karin::ut::Instance() -> getSetting<QString>(ACTIVE_TOOLBAR);
	int setting = -1;
	for(int i = 0; i < tbs.size(); i++)
	{
		QString n = QFileInfo(tbs.at(i)).baseName();
		n[0] = n[0].toUpper();
		m_toolbarsComboBox->addItem(n);
		if(tbs.at(i) == tSetting)
			setting = i;
	}
	if(setting < 0)
	{
		if(!tbs.isEmpty())
		{
			//karin::ut::Instance() -> setSetting<QString>(ACTIVE_TOOLBAR, Toolbar::getAllToolbars().at(0));
			setting = 0;
		}
	}
	m_toolbarsComboBox->setCurrentIndex(setting);
	QVector<MTermWidget *> vec = tabGroup -> getAllTabs();
	foreach(MTermWidget *wid, vec)
		if(wid && wid -> m_display)
		{
			QString name;
			if(wid -> m_display -> activeToolbar())
				name = wid -> m_display -> activeToolbar() -> fileName;
			if(!name.isEmpty() && tbs.contains(name))
				wid -> m_display -> setActiveToolbarIndex(tbs.indexOf(name), true);
			else
				wid -> m_display -> setActiveToolbarIndex(setting, true);
		}
}

void karin::terminal::setInitLayout()
{
	tabBar = new karin::tab_bar;
	tabGroup = new karin::tab_group;
	policy = new QGraphicsAnchorLayout;
	layout = new MLayout;
	portPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	landPolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
	policy -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	policy -> setSpacing(0.0);
	mode = static_cast<karin::terminal::TypeMode>(karin::ut::Instance() -> getSetting<int>(TYPE_MODE));
	const qreal buttonOpacity = 0.5f;

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

	tabBar -> setFocusPolicy(Qt::NoFocus);
	portPolicy -> addItem(k_menuButton);
	portPolicy -> addItem(k_selectionButton);
	landPolicy -> addItem(k_selectionButton);
	landPolicy -> addItem(k_menuButton);
	layout -> setPortraitPolicy(portPolicy);
	layout -> setLandscapePolicy(landPolicy);
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
	//tab group
	policy -> addAnchor(tabGroup, Qt::AnchorLeft, policy, Qt::AnchorLeft);
	policy -> addAnchor(tabGroup, Qt::AnchorRight, policy, Qt::AnchorRight);
	policy -> addAnchor(tabGroup, Qt::AnchorTop, tabBar, Qt::AnchorBottom);
	policy -> addAnchor(tabGroup, Qt::AnchorBottom, policy, Qt::AnchorBottom);
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
	centralWidget() -> setLayout(policy);
}

void karin::terminal::updateMode(int m)
{
	karin::terminal::TypeMode tm = static_cast<karin::terminal::TypeMode>(m);
	if(tm == mode)
		return;
	mode = tm;
	karin::ut::Instance() -> setSetting<int>(TYPE_MODE, m);
	bool b = false;
	switch(mode)
	{
		case karin::terminal::TraditionMode:
			tabBar -> forbidWork();
			traditionContainer -> setEnabled(true);
			traditionContainer -> show();
			b = true;
			break;
		case karin::terminal::KarinMode:
		default:
			tabBar -> allowWork();
			traditionContainer -> setEnabled(false);
			traditionContainer -> hide();
			b = false;
			break;
	}
	m_addTabAction -> setEnabled(b);
	m_addTabAction -> setVisible(b);
	m_closeTabAction -> setEnabled(b);
	m_closeTabAction -> setVisible(b);
	tabWidgetAction -> setEnabled(b);
	tabWidgetAction -> setVisible(b);
	grabFocusForTab();
}

void karin::terminal::showTabByIndex(int index)
{
	//取得当前索引的ID
	qint64 id = tabModel -> valueTab(index);
	if(id < 0)
		return;
	if(id == tabGroup -> currentTabId())
		return;
	/*
	karin::tab_button *b = tabBar -> currentLabel();
	if(b)
		b -> setHighlight(false);
	tabBar -> setCurrentWidget(id);
	//tabBar -> updateTabPosition();
	*/
	//触发切换tab标签信号
	tabBar -> showTab(id);
}

void karin::terminal::syncTitle(const QString &title, qint64 id)
{
	//同步tab标签和tab model的标题
	karin::tab_button *b = (*tabBar)[id];
	if(b)
		b -> setLabel(title, id);
	tabModel -> updateTab(id, title);
}

void karin::terminal::toggleSelectionMode(bool b)
{
	MTermWidget *wid = tabGroup -> currentWidget();
	if(wid)
	{
		wid -> toggleSelectionModeWithBanner(b, tabGroup -> currentTabId());
	}
}

void karin::terminal::syncSelectionModeStatus(bool b, qint64 id)
{
	if(id < 0)
		return;
	if((*tabBar)[id])
		(*tabBar)[id] -> doSelectionModeChange(b, id);
	if(id == tabGroup -> currentTabId())
		k_selectionButton -> setChecked(b);
}

void karin::terminal::setTerminalCursor(int index)
{
	if(index >= 0 && index < 3)
	{
		karin::ut::Instance() -> setSetting<int>(CURSOR_TYPE, index);
		QVector<MTermWidget *> vec = tabGroup -> getAllTabs();
		//光标类型全局生效
		foreach(MTermWidget *wid, vec)
			if(wid && wid -> m_display)
				wid -> m_display -> setKeyboardCursorShape(static_cast<Konsole::TerminalDisplay::KeyboardCursorShape>(index));
	}
	grabFocusForTab();
}

void karin::terminal::clearScreen()
{
	MTermWidget *wid = tabGroup -> currentWidget();
	if(wid)
		wid -> clearScreen();
	grabFocusForTab();
}

void karin::terminal::resetScreen()
{
	MTermWidget *wid = tabGroup -> currentWidget();
	if(wid)
		wid -> resetScreen();
	grabFocusForTab();
}

void karin::terminal::setBlinkingCursor(bool b)
{
	karin::ut::Instance() -> setSetting<bool>(BLINKING_CURSOR, b);
	QVector<MTermWidget *> vec = tabGroup -> getAllTabs();
	//闪烁光标全局生效
	foreach(MTermWidget *wid, vec)
		if(wid && wid -> m_display)
			wid -> m_display -> setBlinkingCursor(b);
	MApplicationWindow *window = MApplication::activeApplicationWindow();
	if(window)
		window -> closeMenu();
	grabFocusForTab();
}

void karin::terminal::grabFocusForTab()
{
	MTermWidget *wid = tabGroup -> currentWidget();
	if(wid)
		if(!wid -> hasFocus())
			wid -> setFocus(Qt::MouseFocusReason);
}
