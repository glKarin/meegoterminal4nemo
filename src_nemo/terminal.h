#ifndef _KARIN_TERMINAL_H
#define _KARIN_TERMINAL_H

#include <QGraphicsWidget>
#include "karin_ut.h"

class QGraphicsAnchorLayout;
class QFileSystemWatcher;

namespace karin
{
	class toolbar;
	class main_menu;
	class button;
	class tab_model;
	class tab_group;
	class tool_icon;

	class terminal : public QGraphicsWidget
	{
		Q_OBJECT

		public:
			enum TypeMode
			{
				KarinMode = 0,
				TraditionMode
			};

		public:
			terminal(QGraphicsWidget *parent = 0);
			~terminal();
			void init(const QString &cmd, QStringList &args);

			public Q_SLOTS:
				void createNewWindow();
			void resetScreen(); SETTING_FOR_CUR_INSTANCE
			void clearScreen(); SETTING_FOR_CUR_INSTANCE
			void setBlinkingCursor(bool b); SETTING_FOR_ALL_INSTANCE
			void setIMNoPredictive(bool b); SETTING_FOR_GLO_INSTANCE
			void onColorSchemeCbxCurrentIndexChanged(int index); SETTING_FOR_ALL_INSTANCE
			void setTerminalCursor(int index); SETTING_FOR_ALL_INSTANCE
			void setActiveToolbarIndex(int index);

Q_SIGNALS:
			void finished();
			void sizeChanged(const QSizeF &s);

		protected:
			void resizeEvent(QGraphicsSceneResizeEvent *event);

			private Q_SLOTS:
				void triggeredSlot(const QString &name, const QVariant &value = QVariant());
			void closeMenu();
			void openLink(const QString &link) const;
			void showTab(qint64 id);
			void showTabByIndex(int index);
			void openMenu();
			void destroyOldTab(qint64 id);
			void destroyCurrentTab();
			void syncTitle(const QString &title, qint64 id);
			void createNewTab(const QString &cmd, QStringList &args);
			void setInitLayout();
			void syncSelectionModeStatus(bool b, qint64 id);
			void toggleSelectionMode(bool b);
			void showHelp();

		private:
			void grabFocusForTab();
			void ungrabFocusForTab();

		private:
			QGraphicsAnchorLayout *m_layout;
			toolbar *m_toolbar;
			main_menu *m_menu;
			tool_icon *m_menuBtn;
			QFileSystemWatcher *m_watcher;
			tab_model *tabModel;
			tab_group *tabGroup;

			Q_DISABLE_COPY(terminal)
	};
};

#endif
