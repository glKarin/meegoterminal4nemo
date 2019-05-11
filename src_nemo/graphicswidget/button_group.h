#ifndef _KARIN_BUTTON_GROUP_H
#define _KARIN_BUTTON_GROUP_H

#include "list_view.h"

namespace karin
{
	class button;
	class list_view_layout;

	class button_group : public rect
	{
		Q_OBJECT

		public:
			button_group(QGraphicsWidget *parent = 0);
			virtual ~button_group();
			void clear();
			void addButton(karin::button *b, int id = -1);
			karin::button * button(int id) const;
			QList<karin::button *> buttons() const;
			int id(karin::button *b) const;
			void removeButton(karin::button *b);
			void removeButton(int index);
			karin::button * takeButton(int index);
			void setItemWidth(int w);
			void setItemHeight(int h);
			bool exclusive(void) const { return m_exclusive; }
			void setExclusive(bool b);
			int count() const; // only count visiblity button

		public Q_SLOTS:
			void relayout();

		protected:
		void resizeEvent(QGraphicsSceneResizeEvent * event);

		private Q_SLOTS:
			void buttonBoundingRectChangeSlot(const QRectF &r);

		private:
			list_view_layout *m_layout;
			int m_itemWidth;
			int m_itemHeight;
			bool m_exclusive; // unavaliable, only for button border style

			static const int M_ItemWidth = 128;
			static const int M_ItemHeight = 96;

			Q_DISABLE_COPY(button_group)
	};

};

typedef karin::button_group MButtonGroup;

#endif
