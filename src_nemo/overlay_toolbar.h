#ifndef _KARIN_OVERLAY_TOOLBAR_H
#define _KARIN_OVERLAY_TOOLBAR_H

#include "popup.h"

namespace karin
{
	class button_group;
	class button;

	class overlay_toolbar : public timer_popup
	{
		Q_OBJECT

		public:
			enum ToolbarPlacement
			{
				AbovePointOfInterest,
				BelowPointOfInterest,
			};

		public:
			overlay_toolbar(QGraphicsWidget *parent = 0);
			virtual ~overlay_toolbar();
			void addActionItem(button *action);
			void setPosition(const QPointF &pos, ToolbarPlacement p = AbovePointOfInterest);

			public Q_SLOTS:
				virtual void appear();
			virtual void disappear();

		protected:
			bool event(QEvent *event);

			private Q_SLOTS:
				void resizeByContent(const QSizeF &r);
			void visibilityUpdated();
			void updateEditorItemVisibility();

		private:
			void handleActionChanged(QActionEvent *actionEvent);
			void handleActionRemoved(QActionEvent *actionEvent);
			void handleActionAdded(QActionEvent *actionEvent);

		private:
			button_group *m_container;
			QList<button *> m_buttons;

			static const int M_Container_Height = 72;
			static const int M_Container_Button_Width = 84;
			Q_DISABLE_COPY(overlay_toolbar)
	};

};

typedef karin::overlay_toolbar MEditorToolbar;

#endif
