#ifndef _KARIN_SCROLL_VIEW_H
#define _KARIN_SCROLL_VIEW_H

#include "rect.h"

class QTimer;

namespace karin
{
	class scroll_view;

	class scroll_bar : public rect
	{
		Q_OBJECT

		public:
			scroll_bar(QGraphicsWidget *parent = 0);
			virtual ~scroll_bar();
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			void setRange(qreal full, qreal cur);
			float per() const { return m_per; }
			void setPer(float p);
			void setLineWidth(int w);
			void setBarWidth(int w);
			int lineWidth() const { return m_lineWidth; }
			int barWidth() const { return m_barWidth; }
			Qt::Orientation orientation() const { return m_orientation; }
			void setOrientation(Qt::Orientation o);
			qreal fullLength() const { return m_fullLength; }
			qreal curLength() const { return m_curLength; }
			void setFullLength(qreal len);
			void setCurLength(qreal len);
			QColor barColor() const { return m_barColor; }
			QColor lineColor() const { return m_lineColor; }
			bool lineVisible() const { return m_lineVisible; }
			void setBarColor(const QColor &c);
			void setLineColor(const QColor &c);
			void setLineVisible(bool b);
			
		protected:
			virtual void resizeEvent(QGraphicsSceneResizeEvent* event) override;

		private:
			void setupBar();
			void setupLine();

		private:
			Qt::Orientation m_orientation;
			float m_per;
			qreal m_fullLength;
			qreal m_curLength;
			bool m_lineVisible;
			int m_lineWidth;
			QColor m_lineColor;
			QLineF m_line;
			qreal m_barRadius;
			int m_barWidth;
			QColor m_barColor;
			QRectF m_barRect;
			QPainterPath m_barPath;

			static const int M_Bar_Width = 4;
			static const int M_Line_Width = 2;
			static const int M_Bar_Color = Qt::white;
			static const int M_Line_Color = Qt::lightGray;
			static const bool M_Line_Visible = false;

			Q_DISABLE_COPY(scroll_bar)

				friend class scroll_view;
	};

	class scroll_view : public rect
	{
		Q_OBJECT

		public:
			scroll_view(QGraphicsWidget *parent = 0);
			virtual ~scroll_view();

			QRectF range() const { return m_range; }
			void setRange(const QRectF &r);
			QPointF position() const { return m_position; }
			void setPosition(const QPointF &p);

			QSizeF viewsize() const { return m_viewsize; }
			void reviewsize(const QSizeF &s) { m_viewsize = s; }
			void reviewsize(qreal w, qreal h) { this->resize(QSizeF(w, h)); }
			void press() { m_pressed = true; }
			void release() { m_pressed = false; }
			void resize(const QSizeF &s);

			void reset();
			bool handleScroll(const QGraphicsSceneMouseEvent *e);
			bool swipe(const QTouchEvent *event);
			void ungrabGesture(Qt::GestureType type, Qt::GestureFlags flags = Qt::GestureFlags());
			void grabGestureWithCancelPolicy(Qt::GestureType type, Qt::GestureFlags flags = Qt::GestureFlags());
			void setAutoRange(bool b) { Q_UNUSED(b); }
			void updatePosition(const QPointF &p);

			public Q_SLOTS:
				void updateViewport();

Q_SIGNALS:
			void positionChanged(const QPointF &p);

		protected:
			virtual void resizeEvent(QGraphicsSceneResizeEvent* event) override;
			bool event(QEvent *event) override;

		private:
			void relayout();
			void setupScrollBar();

			private Q_SLOTS:
				void showScrollerBar();
			void hideScrollerBar();

		private:
			scroll_bar *m_verticalBar;
			scroll_bar *m_horizontalBar;
			QTimer *m_timer;
			QRectF m_range;
			QPointF m_position;
			QSizeF m_viewsize;
			bool m_pressed;
			QPointF m_lastPos;
			bool m_grab;

			int m_touchId;

			static constexpr qreal M_Scroll_Bar_Opacity = 0.618;
			static const int M_Scroll_Bar_Hide_Interval = 4000;

			Q_DISABLE_COPY(scroll_view)
	};
};

#endif
