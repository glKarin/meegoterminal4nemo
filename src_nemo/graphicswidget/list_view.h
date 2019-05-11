#ifndef _KARIN_LIST_VIEW_H
#define _KARIN_LIST_VIEW_H

#include "pannable.h"

#include <QList>

class QGraphicsLinearLayout;

namespace karin
{

	class list_view;

	class list_view_layout : public QGraphicsWidget
	{
		Q_OBJECT

		public:
			list_view_layout(QGraphicsWidget *parent = 0);
			virtual ~list_view_layout();
			QRectF boundingRect() const override;
			QRectF caleBoundingRect() const;
			void addItem(QGraphicsWidget *action, int i = -1);
			int count() const;
			void removeItem(int i);
			void removeItem(QGraphicsWidget *w);
			virtual void clear();
			const QGraphicsWidget * getItem(int i) const;
			const QGraphicsWidget * getItem(const QString &name) const;
			QGraphicsWidget * operator[] (const QString &name);
			QGraphicsWidget * operator[] (int i);
			list_view_layout & operator<< (QGraphicsWidget *action);
			void setOrientation(Qt::Orientation o);
			Qt::Orientation orientation() const;
			QList<QGraphicsWidget *> items(bool mustVisible = false);
			void setSpacing(int s);
			int spacing() const;
			int indexOf(const QGraphicsWidget *w) const;
			QGraphicsWidget * takeItem(int i);
			bool isExists(const QGraphicsWidget *action) const;
			void resizeItems(const QSizeF &size);
			QGraphicsWidget * takeItem(QGraphicsWidget *w);

			public Q_SLOTS:
			void invalidate();

Q_SIGNALS:
			void boundingRectChanged(const QRectF &r = QRectF());
			
		private:
			QList<QGraphicsWidget *> m_actions;
			QGraphicsLinearLayout *m_layout;
			QRectF m_lastBoundingRect;

			Q_DISABLE_COPY(list_view_layout)

			friend class list_view;
	};

	class list_view : public pannable
	{
		Q_OBJECT

		public:
			list_view(QGraphicsWidget *parent = 0);
			virtual ~list_view();
			void addItem(QGraphicsWidget *acion, int i = -1);
			int count() const;
			void removeItem(int i);
			void removeItem(QGraphicsWidget *w);
			virtual void clear();
			const QGraphicsWidget * getItem(int i) const;
			const QGraphicsWidget * getItem(const QString &name) const;
			QGraphicsWidget * operator[] (const QString &name);
			QGraphicsWidget * operator[] (int i);
			list_view & operator<< (QGraphicsWidget *action);
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			void setOrientation(Qt::Orientation o);
			Qt::Orientation orientation() const;
			QColor bgColor() const { return m_bgColor; }
			void setColor(const QColor &color);
			QList<QGraphicsWidget *> items(bool mustVisible = false);
			void setSpacing(int s);
			int spacing() const;
			void setAutoFixedSize(bool b);
			int indexOf(const QGraphicsWidget *w) const;
			QGraphicsWidget * takeItem(int i);
			QGraphicsWidget * takeItem(QGraphicsWidget *w);

			public Q_SLOTS:
			void invalidate();

		protected:
			void resizeEvent(QGraphicsSceneResizeEvent * event);

		private:
			list_view_layout *m_content;
			QColor m_bgColor;
			bool m_autoFixedSize;

			Q_DISABLE_COPY(list_view)
	};
};

typedef karin::list_view MList;

#endif
