#ifndef _KARIN_TABMODEL_H
#define _KARIN_TABMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QHash>

namespace karin
{
	class terminal;

	class tab_model : public QAbstractListModel
	{
		Q_OBJECT

		public:
			tab_model(QObject *parent = 0);
			virtual ~tab_model();
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
			void insertTab(qint64 id, const QString &title, int index);
			void addTab(qint64 id, const QString &title) { insertTab(id, title, tabList.size()); }
			void removeTab(qint64 id);
			void updateTab(qint64 id, const QString &title);
			bool hasTab(qint64 id) const { return tabList.contains(id); }
			int indexTab(qint64 id) const { return tabList.indexOf(id); }
			qint64 valueTab(int i) const;
			qint64 genTabId() const;
			QPair<qint64, QString> getTabInfo(int i) const;

		private:
			QList<qint64> tabList;
			QHash<qint64, QString> tabMap;

			friend class terminal;

	};
}

#endif
