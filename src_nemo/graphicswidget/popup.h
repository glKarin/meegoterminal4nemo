#ifndef _KARIN_POPUP_H
#define _KARIN_POPUP_H

#include "rect.h"

class QGraphicsAnchorLayout;
class QPropertyAnimation;
class QTimer;

namespace karin
{
	struct Anim_s
	{
		QString property_name;
		QVariant open_state;
		QVariant close_state;
	};

	class popup : public rect
	{
		Q_OBJECT

		public:
			enum Popup_State_e
			{
				Popup_Is_Closed = 0,
				Popup_Is_Opening,
				Popup_Is_Opened,
				Popup_Is_Closing,
			};
			enum Popup_Type_e
			{
				Popup_Fade = 0,
				Popup_R2L,
				Popup_L2R,
				Popup_U2D,
				Popup_D2U,
			};
			enum Popup_Appear_e
			{
				Popup_Appear_In_Scene = 0,
				Popup_Appear_In_ParentWidget
			};

		public:
			popup(QGraphicsWidget *parent = 0);
			virtual ~popup();
			Popup_State_e state() const { return m_state; }
			void setDuration(int d) { m_duration = d; }
			void setParentWidget(QGraphicsWidget *w);
			QGraphicsWidget * parentWidget() { return m_parentWidget; }
			virtual QGraphicsWidget * centerWidget() { return m_centralWidget; }
			virtual void setCentralWidget(QGraphicsWidget *w);
			Popup_Type_e popupType() const { return m_popupType; }
			void setPopupType(Popup_Type_e e);
			Popup_Appear_e oppearType() const { return m_appearType; }
			void setAppearType(Popup_Appear_e e);
			QGraphicsWidget *take();
			virtual void setFixedWidth(int w) override;
			virtual void setFixedHeight(int h) override;
			virtual void setFixedSize(const QSize &s) override;
			virtual void setFixedSize(int w, int h) override;

			public Q_SLOTS:
				virtual void toggle(bool open);
			virtual void open(bool anim = false);
			virtual void close(bool anim = false);
			void reset(bool close = true);

Q_SIGNALS:
			void stateChanged(Popup_State_e s);
			void done(bool open);
			void opened();
			void closed();

		protected:
			void resizeEvent(QGraphicsSceneResizeEvent * event) override;

			private Q_SLOTS:
				void finishedSlot();
			void sceneRectChangedSlot(const QRectF &rect);

		protected:
			void startState();
			void endState();

		private:
			bool isValid() const;
			void setState(Popup_State_e s);
			void storeCloseState();
			void storeOpenState();
			QVariant startValue() const;

		private:
			QPropertyAnimation *m_animation;
			Popup_State_e m_state;
			int m_duration;
			QGraphicsWidget *m_parentWidget;
			QGraphicsWidget *m_centralWidget;
			QGraphicsAnchorLayout *m_layout;
			Popup_Type_e m_popupType;
			Popup_Appear_e m_appearType;
			struct Anim_s m_animState;

			static const int M_Duration = 180;

			Q_DISABLE_COPY(popup)
	};

	class timer_popup : public popup
	{
		Q_OBJECT

		public:
			enum Timer_Popup_DeletionPolicy_e
			{
				KeepWhenDone, 
				DestroyWhenDone, 
				DestroyWhenDismissed
			};
			
		public:
			timer_popup(QGraphicsWidget *parent = 0);
			virtual ~timer_popup();
			void setInterval(int ms);
			void setAutoHideEnabled(bool on);
			bool autoHideEnabled() const { return m_autoHideEnabled; }
			bool isAppeared() const;

			public Q_SLOTS:
				virtual void toggle(bool open) override;
			virtual void open(bool anim = false) override;
			virtual void close(bool anim = false) override;
			virtual void appear(Timer_Popup_DeletionPolicy_e e);
			virtual void appear();
			virtual void disappear();

		protected:
			void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
			void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) override;

			private Q_SLOTS:
				void timeoutSlot();
				void doneSlot(bool open);

		private:
			Timer_Popup_DeletionPolicy_e m_deletionPolicy;
			QTimer *m_timer;
			int m_interval;
			bool m_pressed;
			bool m_deleteLater;
			bool m_autoHideEnabled;

			static const int M_Interval = 2000;

			Q_DISABLE_COPY(timer_popup)

	};

};

#endif
