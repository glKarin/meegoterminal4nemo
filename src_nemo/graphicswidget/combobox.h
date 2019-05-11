#ifndef _KARIN_COMBOBOX_H
#define _KARIN_COMBOBOX_H

#include "menu_item.h"

namespace karin
{
	class menu_item_group;
	class label;

	class combobox : public abstract_menu_item
	{
		Q_OBJECT

		public:
			combobox(QGraphicsWidget *parent = 0);
			virtual ~combobox();
			QString title() const;
			int count() const;
			bool exclusive() const;
			void setExclusive(bool b);
			void addItem(const QString &label, const QString &name = QString(), const QVariant &value = QVariant(), int i = -1);
			void removeItem(int index);
			void updateItem(int index, const QString &text);
			QList<int> currentIndex() const;
			virtual bool isOn() const override { return false; }
			virtual void setOn(bool b) override { Q_UNUSED(b); };

			public Q_SLOTS:
			void setCurrentIndex(const QList<int> &index);
			void setTitle(const QString &title);
			void clear();

Q_SIGNALS:
			void currentIndexChanged(const QList<int> &index);
			void triggered(const QString &name, const QVariant &value = QVariant());

			private Q_SLOTS:
				void checkedChangedSlot();

		private:
			label *m_title;
			menu_item_group *m_buttonGroup;


			Q_DISABLE_COPY(combobox)
	};
};

typedef karin::combobox MComboBox;

#endif
