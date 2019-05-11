#ifndef _KARIN_KEY_H
#define _KARIN_KEY_H

#include "rect.h"

namespace karin
{
	static const QColor Key_Bg_Color_0_ub(247, 247, 247);
	static const QColor Key_Text_Color_0_ub(41, 40, 41);
	static const QColor Key_Border_Color_0_ub(181, 178, 181);

	static const QColor Key_Bg_Color_1_ub(8, 4, 8);
	static const QColor Key_Text_Color_1_ub(255, 255, 255);
	static const QColor Key_Border_Color_1_ub(0, 0, 0);

	static const QColor Key_Bg_Color_2_ub(24, 28, 24);
	static const QColor Key_Text_Color_2_ub(255, 255, 255); //(115, 113, 115);
	static const QColor Key_Border_Color_2_ub(0, 0, 0);

	class base_key : public rect
	{
		Q_OBJECT
		public:
			enum Key_Type
			{
				Key_General = 0,
				Key_Modifier,
				Key_Switch,
			};
			enum Key_State
			{
				Key_Released = 0,
				Key_Pressed,
				Key_Long_Pressed,
				Key_Unused_State,
			};

		public:
			base_key(QGraphicsWidget *parent = 0);
			virtual ~base_key();
			virtual void reset() = 0;
			virtual void switch2() = 0;
			Key_State state() const { return m_state; }
			Key_Type keyType() const { return m_keyType; }
			unsigned sym() const { return m_sym; }
			unsigned modifier() const { return m_modifier; }
			QString label() const { return m_label; }
			void setSym(unsigned s) { m_sym = s; }
			void setModifier(unsigned s) { m_modifier = s; }
			void setLabel(const QString &s) { m_label = s; }
			bool isValid() const { return m_sym != 0; }
			void setRadius(float r) { m_radius = r; }
			void setName(const QString &name) { m_name = name; }
			QString name() const { return m_name; }
			virtual void setState(Key_State s) { m_state = s; }
			bool pressed() const { return m_pressed; }
			virtual void touchEvent(bool b);

Q_SIGNALS:
			void sendKeySignal(unsigned keycode, unsigned modifier, bool p);

		protected:
			void setKeyType(Key_Type t) { m_keyType = t; }
			void setPressed(bool b) { m_pressed = b; }

		protected:
			Key_Type m_keyType;
			QString m_label;
			unsigned m_sym;
			QString m_name;
			unsigned m_modifier;
			Key_State m_state;
			bool m_pressed;

		private:
			Q_DISABLE_COPY(base_key)
	};

#if 0
	class switch_key : public key
	{
		Q_OBJECT
		public:
			switch_key(QGraphicsWidget *parent = 0);
			virtual ~switch_key();
			virtual void reset();
			virtual void switch2();
	};

#endif

	class modifier_key : public base_key
	{
		Q_OBJECT
		public:
			modifier_key(QGraphicsWidget *parent = 0);
			~modifier_key();
			virtual void reset();
			virtual void switch2();
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			void setState(base_key::Key_State state) override;
			void touchEvent(bool b) override;

		protected:
			void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
			void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;


		private:
			Q_DISABLE_COPY(modifier_key)
	};

	class general_key : public base_key
	{
		Q_OBJECT
		public:
			general_key(QGraphicsWidget *parent = 0);
			virtual ~general_key();
			virtual void reset();
			virtual void switch2();
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			bool repeat() const { return m_repeat; }
			void setRepeat(bool b);
			int interval() const { return m_interval; }
			void setInterval(int i) { m_interval = i; }
			int delay() const { return m_delay; }
			void setDelay(int i) { m_delay = i; }
			void setState(base_key::Key_State state) override;
			void touchEvent(bool b) override;

		protected:
			void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
			void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;

			private Q_SLOTS:
				void timeoutSlot();

		private:
			bool m_repeat;
			QTimer *m_timer;
			int m_interval;
			int m_delay;

			static const unsigned M_Repeat_Interval_MS = 100;
			static const unsigned M_Repeat_Delay_MS = 100;

		private:
			Q_DISABLE_COPY(general_key)
	};

};

#endif
