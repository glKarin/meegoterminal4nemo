#ifndef _KARIN_TABBAR_H
#define _KARIN_TABBAR_H

#include <MWidgetController>
#include <MListItem>

class MSeparator;
class MButton;
class MPannableViewport;
class QGraphicsAnchorLayout;
class QGraphicsLinearLayout;

namespace karin
{
	class tab_button;
	class terminal;

	class tab_bar : public MWidgetController
	{
		Q_OBJECT

		public:
			tab_bar(QGraphicsItem *parent = 0);
			virtual ~tab_bar();
			tab_button * addTab(qint64 id);
			tab_button * operator[] (qint64 id);
			qint64 nextTab(qint64 id = -1);
			tab_button * currentLabel()
			{
				return operator[](currentWidget);
			}
			void allowWork();
			void forbidWork();
			int currentIndex() const;
			void updateTabPosition();
			void setCurrentWidget(qint64 id)
			{
				currentWidget = id;
			}
			qint64 currentTabId() const
			{
				return currentWidget;
			}

			public Q_SLOTS:
				void closeTab(qint64 id);
				void showTab(qint64 id);

Q_SIGNALS:
			void currentTab(qint64 id);
			void openMenu();
			void requestNewTab();
			void requestCloseTab(qint64 id, bool current);

		private:
			qint64 currentWidget;
			MButton *m_menuButton;
			MButton *addTabButton;
			MSeparator *separator;
			QGraphicsAnchorLayout *layout;
			MPannableViewport *pan;
			QGraphicsLinearLayout *policy;
			MWidget *tabBar;

			friend class terminal;
			Q_DISABLE_COPY(tab_bar)

	};
}

#endif
