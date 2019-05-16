#ifndef _KARIN_VIEWPORT_H
#define _KARIN_VIEWPORT_H

#include <QGraphicsWidget>

namespace karin
{
	class viewport : public QObject
	{
		Q_OBJECT

		public:
			viewport(QGraphicsWidget *parent = 0);
			~viewport();

			QRectF range() const { return m_range; }
			void setRange(const QRectF &r);
			QPointF position() const { return m_position; }
			void setPosition(const QPointF &p) { m_position = p; }

			QSizeF size() const { return m_viewsize; }
			void resize(const QSizeF &s) { m_viewsize = s; }
			void resize(qreal w, qreal h) { resize(QSizeF(w, h)); }
			void press() { m_pressed = true; }
			void release() { m_pressed = false; }

			void reset();
			bool scroll(const QGraphicsSceneMouseEvent *e);
			bool swipe(const QTouchEvent *event);
			void ungrabGesture(Qt::GestureType type, Qt::GestureFlags flags = Qt::GestureFlags());
      void grabGestureWithCancelPolicy(Qt::GestureType type, Qt::GestureFlags flags = Qt::GestureFlags());
			void setAutoRange(bool b) { Q_UNUSED(b); }
			void updatePosition(const QPointF &p);

			public Q_SLOTS:
				void update();

Q_SIGNALS:
			void positionChanged(const QPointF &p);

		private:
			QRectF m_range;
			QPointF m_position;
			QSizeF m_viewsize;
			bool m_pressed;
			QPointF m_lastPos;
			bool m_grab;

			int m_touchId;

			Q_DISABLE_COPY(viewport)
	};
};

#endif
