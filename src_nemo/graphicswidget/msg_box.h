#ifndef _KARIN_MSG_BOX_H
#define _KARIN_MSG_BOX_H

#include "popup.h"

namespace karin
{
	class label;

	class msg_box : public timer_popup
	{
		Q_OBJECT

		public:
			msg_box(const QString &title = QString(), const QString &text = QString(), QGraphicsWidget *parent = 0);
			~msg_box();
			void setTitle(const QString &title);
			QString title() const { return m_title; }
			void setText(const QString &text);
			QString text() const { return m_text; }
			void setTextColor(const QColor &c);

		private:
			void setupText();

		private:
			label *m_label;
			QString m_title;
			QString m_text;

			Q_DISABLE_COPY(msg_box)
	};

	class info_banner : public timer_popup
	{
		Q_OBJECT

		public:
			info_banner(QGraphicsWidget *parent = 0);
			~info_banner();
			void setTitle(const QString &title);
			QString title() const { return m_title; }
			void setTextColor(const QColor &c);

			public Q_SLOTS:
				virtual void appear();
				virtual void appear(Timer_Popup_DeletionPolicy_e e);

		private:
			void setupText();

		private:
			label *m_label;
			QString m_title;

			Q_DISABLE_COPY(info_banner)
	};

};

typedef karin::msg_box MMessageBox;
typedef karin::info_banner MBanner;

#endif
