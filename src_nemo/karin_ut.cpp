#include "karin_ut.h"

#include <QSettings>
#include <QFont>

#include <QDebug>

#ifdef _KARIN_INSTALL_
#define IM_TOOLBARS_DIR "/usr/share/karin-console/toolbars/"
#else
#define IM_TOOLBARS_DIR "/home/user/karinconsole/src_meegotouch/toolbars/"
#endif


karin::ut::ut()
	:settings(new QSettings)
{
}

karin::ut::~ut()
{
	delete settings;
}

karin::ut * karin::ut::Instance()
{
	static karin::ut s_ut;
	return &s_ut;
}

void karin::ut::setSetting(const QString &key, const QVariant &value)
{
	if(!settings -> contains(key) || value.isNull())
	{
		QVariant defSets = karin::ut::defaultSettings(key);
		if(defSets.isNull())
			return;
		settings -> setValue(key, defSets);
#ifdef _SIMLATOR
		qDebug()<<"Setting key -> \""<<key<<"\" set to default -> "<<defSets;
#endif
	}
	else
		settings -> setValue(key, value);
}

QVariant karin::ut::getSetting(const QString &key)
{
	if(!settings -> contains(key))
		setSetting(key);
	return settings -> value(key);
}

QVariant karin::ut::defaultSettings(const QString &key)
{
	if(key == FONT)
		return QVariant(QVariant(QFont("Monospace", 12).toString()));
	else if(key == COLOR_SCHEME_SETTING)
		return QVariant(1);
	else if(key == FULL_SCREEN)
		return QVariant(false);
	else if(key == ACTIVE_TOOLBAR)
		return QVariant(QVariant(QString("%1%2").arg(IM_TOOLBARS_DIR).arg("karin.xml")));
	else if(key == SHOW_BANNER)
		return QVariant(true);
	else if(key == ORIENTATION)
		return QVariant(0);
	else if(key == TRANSLUCENT_INPUTMETHOD)
		return QVariant(false);
	else if(key == ENABLE_VKB)
		return QVariant(true);
	else if(key == TYPE_MODE)
		return QVariant(0);
	else if(key == CURSOR_TYPE)
		return QVariant(0);
	else if(key == BLINKING_CURSOR)
		return QVariant(false);
	else if(key == IM_NO_PREDICTIVE)
		return QVariant(true);
	else
		return QVariant();
}

const QString & karin::ut::GetMeeGoTerminalHelp()
{
	static const QString MeeGoTerminal_Help = QString(
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

		return MeeGoTerminal_Help;
}

const QString & karin::ut::GetKarinConsoleHelp()
{
	static const QString MeeGoTerminal_Help = QString(
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

	static const QString KarinConsole_Help;
	if(KarinConsole_Help.isEmpty())
	{
		QString &text = const_cast<QString &>(KarinConsole_Help);

		text += QString(APP_NAME) + " - " + QObject::tr("Help") + "<br><br>";
		text += QObject::tr("Click '+' button to create a new tab. In tab label, you can set selection mode, show this tab and close this tab. Current tab label is highlight.") + "<br><br>";
		// text += QObject::tr("Also change Tab view type or traditional view type.") + "<br><br>";
		text += QObject::tr("Fixed copy strings that include wide charactor, like Chinese and so on.") + "<br><br>";
		// text += QObject::tr("Change active toolbar with swipe gesture, it is active for current tab. If you want to save to settings, must set it in menu, and it will change active toolbar of current tab and new tabs.") + "<br><br>";
		// text += QObject::tr("Change font size with pinch gesture, it is will save to settings, and it will only be active for new tabs.") + "<br><br>";
		// text += QObject::tr("Change color scheme with menu, it will save to settings and be active for all tabs that include old tabs and new tabs.") + "<br><br>";
		// text += QObject::tr("You can set window orientation, and show or hide info banner.") + "<br><br>";
		// text += QObject::tr("You can show or hide virtual keyboard, and make virtual keyboard translucent. It will save to settings and make active for all tabs.") + "<br><br>";
		// text += QObject::tr("You can choose cursor shape, and set cursor is blinking. It will save to settings and make active for all tabs.") + "<br><br>";
		// text += QObject::tr("Add clear screen echo or reset session.") + "<br><br>";
		// text += QObject::tr("Add a new default toolbar named \"karin\", support portrait and landscape orientation.") + "<br><br>";
		text += QObject::tr("Add 'New Session' for multiple tab.") + "<br><br>";
		text += QObject::tr("Suggest using 'Harmattan Dev' keyboard layout in package 'harmattan-keyboard' for input with no-prediction.") + "<br><br>";
		text += QObject::tr("If not installed 'harmattan-keyboard', need to open 'no-prediction'. But it has some bugs, like it will input preedit text when 'Backspace' key is clicked, for this, first clicking 'Return' key to commit all predict text, second swipe screen to make application to minisized, final make application to active.") + "<br><br>";
		text += QObject::tr("Toolbar layout configure is a JSON file. 'sym' is Qt key code, 'name' is name of the key, 'label' is text for shown, 'modifier' is modifier key when the key is pressed, 'data' is unused now, 'repeat' is setting key can repeat.") + "<br><br>";
		text += QObject::tr("Special Thanks: daols (DOSPY) for testing and desktop icon.") + "<br><br>";
		text += QString("<a href=mailto:beyondk2000@gmail.com>%1</a> @ %2").arg(QObject::tr("Karin")).arg("2015");
	}

	return KarinConsole_Help;
}

