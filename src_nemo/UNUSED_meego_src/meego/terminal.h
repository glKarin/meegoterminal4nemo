#ifndef _KARIN_TERMINAL_H
#define _KARIN_TERMINAL_H

#include <MApplicationPage>

class MLayout;
class MLinearLayoutPolicy;
class MComboBox;
class QFileSystemWatcher;
class MAction;
class MWidgetAction;
class QGraphicsAnchorLayout;
class MButton;

namespace karin
{
	class tab_bar;
	class tab_group;
	class button_with_label;
	class tab_model;

	class terminal : public MApplicationPage
	{
		Q_OBJECT

		public:
			enum TypeMode
			{
				KarinMode = 0,
				TraditionMode
			};

		public:
			terminal(QGraphicsItem *parent = 0);
			virtual ~terminal();
			void init(const QString &cmd, QStringList &args);

Q_SIGNALS:
			void finished();

			private Q_SLOTS:
				void showTab(qint64 id);
			void showTabByIndex(int index);
			void openMenu();
			void showHelp();
			void toggleFullScreenMode(bool b);
			void createNewWindow() const;
			void createNewTab();
			void openLink(const QString &link) const;
			void destroyOldTab(qint64 id);
			void destroyCurrentTab();
			void setActiveToolbarIndex(int index);
			void onColorSchemeCbxCurrentIndexChanged(int index);
			void setShowBanner(bool b);
			void setWindowOrientation(int i);
			void setSelectionMode(bool b, qint64 id);
			void doTranslucencyToggled(bool b);
			void doSuppressionToggled(bool b);
			void updateToolbarDirectory(const QString &path);
			void updateMode(int m);
			void syncTitle(const QString &title, qint64 id);
			void toggleSelectionMode(bool b);
			void syncSelectionModeStatus(bool b, qint64 id);
			void setTerminalCursor(int index);
			void clearScreen();
			void resetScreen();
			void setBlinkingCursor(bool b);
			void grabFocusForTab();

		private:
			void setupMenu();
			void createNewTab(const QString &cmd, QStringList &args);
			void setInitLayout();

		private:
			TypeMode mode;
			tab_model *tabModel;
			tab_bar *tabBar;
			tab_group *tabGroup;
			QGraphicsAnchorLayout *policy;
			MComboBox *m_toolbarsComboBox;
			MComboBox *m_colorSchemeComboBox;
			button_with_label *m_fullScreenAction;
			button_with_label *m_showBannerAction;
			MComboBox *m_windowOrientationComboBox;
			button_with_label *m_translucentInputMethodAction;
			button_with_label *m_enableVirtualKeyboardAction;
			QFileSystemWatcher *watcher;
			MComboBox *m_typeModeComboBox;
			MAction *m_addTabAction;
			MAction *m_closeTabAction;
			MComboBox *m_tabComboBox;
			MButton *k_menuButton;
			MButton *k_selectionButton;
			MLayout *layout;
			MLinearLayoutPolicy *portPolicy;
			MLinearLayoutPolicy *landPolicy;
			MWidget *traditionContainer;
			MWidgetAction *tabWidgetAction;
			MComboBox *m_cursorComboBox;
			button_with_label *m_blinkingCursorAction;

			Q_DISABLE_COPY(terminal)
	};
}

#endif
