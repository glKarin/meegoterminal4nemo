#ifndef _KARIN_DIALOG_H
#define _KARIN_DIALOG_H

#include "popup.h"

class QStaticText;
class QTextBrowser;
class QGraphicsAnchorLayout;

namespace karin
{
	class button;
	class tool_icon;
	class button_group;
	class graphics_text_browser;

	class dialog_title_bar : public rect
	{
		Q_OBJECT

		public:
			dialog_title_bar(QGraphicsWidget *parent = 0);
			virtual ~dialog_title_bar();
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			QString title() const;
			QColor lineColor() const { return m_lineColor; }
			QColor textColor() const { return m_textColor; }
			void setLineColor(const QColor &c);
			void setTextColor(const QColor &c);

			public Q_SLOTS:
				void setTitle(const QString &t);

		Q_SIGNALS:
			void closeClicked();

		protected:
			void resizeEvent(QGraphicsSceneResizeEvent* event) override;

			private Q_SLOTS:
				void closeClickedSlot(bool b);

		private:
			QStaticText *m_text;
			tool_icon *m_closeBtn;
			QColor m_lineColor;
			QColor m_textColor;
			QVector<QLineF> m_lines;

			static const int M_Max_Close_Button_Size = 48;
			static const int M_Line_Width = 4;
			static const int M_Line_Color = Qt::white;
			static const int M_Text_Color = Qt::black;

			Q_DISABLE_COPY(dialog_title_bar)
	};

	class dialog : public timer_popup
	{
		Q_OBJECT

		public:
			dialog(QGraphicsWidget *parent = 0);
			virtual ~dialog();
			QString title() const { return m_titleBar->title(); }
			virtual QGraphicsWidget * centerWidget() { return m_realCentralWidget; }
			virtual void setCentralWidget(QGraphicsWidget *w);
			void addButton(const QString &label);
			virtual void setBgColor(const QColor &color);

			public Q_SLOTS:
				void setTitle(const QString &title) { m_titleBar->setTitle(title); }

		private:
			void relayout();

		private:
			rect *m_oriCentralWidget;
			QGraphicsAnchorLayout *m_layout;
			dialog_title_bar *m_titleBar;
			button_group *m_toolBar;
			QGraphicsWidget *m_realCentralWidget;

			static const int M_Button_Group_Height = 64;
			static const int M_Title_Bar_Height = 80;
			static const int M_Dialog_BG_Color = Qt::black;

			Q_DISABLE_COPY(dialog)
	};

	class msg_dialog : public dialog
	{
		Q_OBJECT

		public:
			msg_dialog(const QString &title = QString(), const QString &text = QString(), QGraphicsWidget *parent = 0);
			virtual ~msg_dialog();
			QString text() const;

			public Q_SLOTS:
				void setText(const QString &t);

Q_SIGNALS:
			void linkActivated(const QString &s);

		private:
			graphics_text_browser *m_textBrowser;

			static const int M_Msg_Dialog_Text_Color = Qt::white;
			static const int M_Msg_Dialog_BG_Color = Qt::black;
			Q_DISABLE_COPY(msg_dialog)

			static const int M_Border_Width = 2;
	};

};

typedef karin::dialog MDIalog;

#endif

