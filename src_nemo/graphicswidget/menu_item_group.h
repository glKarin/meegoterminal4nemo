#ifndef _KARIN_MENU_ITEM_GROUP_H
#define _KARIN_MENU_ITEM_GROUP_H

#include "list_view.h"

namespace karin
{
	class menu_item;

	class menu_item_group : public list_view
	{
		Q_OBJECT

		public:
			menu_item_group(QGraphicsWidget *parent = 0);
			virtual ~menu_item_group();
			virtual void addAction(const QString &label, const QString &name = QString(), const QVariant &value = QVariant(), int i = -1);
			virtual void removeAction(int index);
			virtual void updateAction(int index, const QString &label);
			void reset();
			bool exclusive() const { return m_exclusive; }
			void setExclusive(bool b) { m_exclusive = b; }
			const QList<menu_item *> & checkedActions() const { return m_checkedActions; }
			const QList<int> & checkedIndexs() const { return m_checkedIndexs; }
			void setCheckedIndexs(const QList<int> &indexs);
			void manageActions(menu_item *btn, bool on);
			virtual void clear() override;
			void setItemWidth(int w);
			void setItemHeight(int h);

Q_SIGNALS:
			void triggered(const QString &name, const QVariant &value = QVariant());
			void checkedChanged();

			private Q_SLOTS:
				void triggeredSlot(const QString &name, const QVariant &value = QVariant());

			private:
			bool m_exclusive;
			QList<menu_item *> m_checkedActions;
			QList<int> m_checkedIndexs;
			int m_itemWidth;
			int m_itemHeight;

			static const int M_ItemWidth = 192;
			static const int M_ItemHeight = 96;

			Q_DISABLE_COPY(menu_item_group)
	};

}

#endif

