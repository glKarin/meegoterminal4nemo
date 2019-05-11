#ifndef _KARIN_TABGROUP_H
#define _KARIN_TABGROUP_H

#include <QGraphicsWidget>
#include <QVector>
#include <QList>
#include <QHash>

class QGraphicsAnchorLayout;
class MTermWidget;

namespace karin
{
	class terminal;

	class tab_group : public QGraphicsWidget
	{
		Q_OBJECT

		public:
			tab_group(QGraphicsWidget *parent = 0);
			~tab_group();
			MTermWidget * addTab(qint64 id, int *r_index = 0);
			void removeTab(qint64 id);
			MTermWidget * take(qint64 id);
			void showTab(qint64 id);
			qint64 currentTabId() { return current; }
			MTermWidget * currentWidget() { return m_currentWidget; }
			MTermWidget * operator[] (qint64 id);
			QVector<MTermWidget *> getAllTabs();
			const QList<MTermWidget *> & tabs() const { return m_tabs; }
			int currentIndex() const { return m_currentIndex; }
			int count() const { return m_tabs.size(); }
			qint64 nextTab(qint64 widget);

		private:
			MTermWidget * getCurrentWidget();
			int getCurrentIndex() const;
			qint64 getWidgetId(const MTermWidget *wid);

		private:
			QGraphicsAnchorLayout *layout;
			qint64 current;
			QHash<qint64, MTermWidget *> tabMap;
			QList<MTermWidget *> m_tabs;
			MTermWidget *m_currentWidget;
			int m_currentIndex;

			friend class terminal;
			Q_DISABLE_COPY(tab_group)
	};

}

#endif
