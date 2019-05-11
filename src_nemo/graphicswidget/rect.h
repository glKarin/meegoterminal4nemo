#ifndef _KARIN_RECT_H
#define _KARIN_RECT_H

#include "k.h"
#include <QGraphicsWidget>

#define RECT_IS_TOUCH() (karin::rect::interactiveType() == karin::rect::Rect_Touch)

namespace karin
{
	class rect : public QGraphicsWidget
	{
		Q_OBJECT

		public:
			enum Rect_Position_Type_e
			{
				Rect_in_Center = 0,
				Rect_In_TL = K_MASK(0),
				Rect_In_TR = K_MASK(1),
				Rect_In_BL = K_MASK(2),
				Rect_In_BR = K_MASK(3),
				Rect_In_Border = K_MASK_ALL, // default
			};
			enum Rect_Interactive_Type_e
			{
				Rect_Mouse = 0,
				Rect_Touch
			};

		public:
			rect(QGraphicsWidget *parent = 0);
			virtual ~rect();
			virtual void setFixedSize(const QSize &s);
			virtual void setFixedSize(int w, int h);
			virtual void setFixedWidth(int w);
			virtual void setFixedHeight(int h);
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			QColor bgColor() const { return m_bgColor; }
			virtual void setBgColor(const QColor &c);
			QColor borderColor() const { return m_borderColor; }
			virtual void setBorderColor(const QColor &c);
			virtual void setRadius(float r);
			virtual void setPositionType(unsigned t);
			unsigned positionType() const { return m_positionType; }
			qreal width() const { return size().width(); }
			qreal height() const { return size().height(); }
			Rect_Interactive_Type_e interactiveType() const { return m_interactiveType; }
			void setHandleMouseEvent(bool b);
			bool handleMouseEvent() const { return m_handleMouseEvent; }

		public Q_SLOTS:
			void setupFixedSize(const QSizeF &s);

		Q_SIGNALS:
			void sizeChanged(const QSizeF &size = QSize());

		protected:
			void resizeEvent(QGraphicsSceneResizeEvent* event) override;
			void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
			void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
			void setBorderWidth(float w);
			QRectF innerRect() const { return m_innerRect; }
			QRectF outerRect() const { return m_outerRect; }
			QPointF center() const { return m_center; }
			void setNeedToPaint(bool b);
			bool needToPaint() const { return m_needToPaint; }
			void setupPainter(QPainter *painter);

		private:
			void caleInnerRect();
			void caleOuterRect();
			void caleCenter();
			void caleOuterPath();
			void caleInnerPath();
			void setupDefaultFont();
			void setInteractiveType(Rect_Interactive_Type_e e);

		protected:
			bool m_needToPaint;
			QColor m_bgColor;
			QColor m_borderColor;
			float m_borderWidth;
			float m_radius;
			QRectF m_innerRect;
			QRectF m_outerRect;
			QPointF m_center;
			int m_fixedWidth;
			int m_fixedHeight;
			QPainterPath m_outerPath;
			QPainterPath m_innerPath;
			unsigned m_positionType;
			Rect_Interactive_Type_e m_interactiveType; // always mouse
			bool m_handleMouseEvent;


			static const int M_Radius = 20;
			static const int M_Border = 2;
			static const int M_FixedWidth = 64;
			static const int M_FixedHeight = 64;
			static const int M_BgColor = Qt::white;
			static const int M_BorderColor = Qt::black;
			static const int M_FontPixelSize = 24;

			Q_DISABLE_COPY(rect)
	};
};

#endif
