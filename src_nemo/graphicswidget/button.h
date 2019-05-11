#ifndef _KARIN_BUTTON_H
#define _KARIN_BUTTON_H

#include "rect.h"

namespace karin
{
	static const QColor Button_Bg_Color_0_ub(247, 247, 247);
	static const QColor Button_Text_Color_0_ub(41, 40, 41);
	static const QColor Button_Border_Color_0_ub(222, 219, 222);
	static const QColor Button_Bg_Color_1_ub(222, 219, 222);
	static const QColor Button_Text_Color_1_ub(123, 121, 123);
	static const QColor Button_Border_Color_1_ub(189, 186, 189);
	static const QColor Button_Bg_Color_2_ub(247, 247, 247);
	static const QColor Button_Text_Color_2_ub(41, 40, 41);
	static const QColor Button_Border_Color_2_ub(222, 219, 222);

	class button : public rect
	{
		Q_OBJECT
		public:
			button(QGraphicsWidget *parent = 0);
			button(const QString &text, QGraphicsWidget *parent = 0);
			virtual ~button();
			QString text() const { return m_text; }
			void setText(const QString &s);
			bool isChecked() const { return m_checked; }
			bool isCheckable() const { return m_checkable; }
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			void setAutoRelease(bool b);
			bool autoRelease() const { return m_autoRelease; }

Q_SIGNALS:
			void clicked(bool c = false);
			void pressed();
			void released();
			void toggled(bool c);
			void triggered(bool c);

			public Q_SLOTS:
			void setChecked(bool b);
			void setCheckable(bool b);
			void touchEvent(bool b);
			virtual void reset();
			virtual void cancel();

		protected:
			void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
			void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;
			void resizeEvent(QGraphicsSceneResizeEvent* event) override;

		protected:
			QString m_text;
			bool m_checked;
			bool m_checkable;
			bool m_autoRelease;

			static const int M_Radius = 40;
			Q_DISABLE_COPY(button)
	};

};

typedef karin::button MButton;

#endif
