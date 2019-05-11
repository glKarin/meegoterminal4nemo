#ifndef _KARIN_TEXTBROWSER_H
#define _KARIN_TEXTBROWSER_H

#include <QTextBrowser>
#include <QGraphicsProxyWidget>

namespace karin
{
	class text_browser : public QTextBrowser
	{
		Q_OBJECT
		public:
			text_browser(QWidget *widget = 0);
			virtual ~text_browser();
			int getVScrollBarPosition() const;
			void setVScrollBarPosition( int pos );
			int getHScrollBarPosition() const;
			void setHScrollBarPosition( int pos );
			bool slideMode() const;
			void setStyleTextColor(const QString &color);
			void setStyleBgColor(const QString &color);
			
			public Q_SLOTS:
			void setSlideMode(bool b);

Q_SIGNALS:
			void scrollHBarValueChanged(int value);
			void scrollVBarValueChanged(int value);
			void scrollHBarRangeChanged(int min, int max);
			void scrollVBarRangeChanged(int min, int max);
			void slideModeChanged(bool b);

		protected:
			virtual void keyPressEvent(QKeyEvent *event) override;
			virtual void mouseMoveEvent(QMouseEvent *e) override;
			virtual void mousePressEvent(QMouseEvent *e) override;
			virtual void mouseReleaseEvent(QMouseEvent *e) override;
			virtual void mouseDoubleClickEvent(QMouseEvent *e) override;

		private:
			void setupStyle();

		private:
			bool slide_mode;
			int start_x;
			int start_y;
			int last_x;
			int last_y;
			bool mouse_pressed;
			bool zoom_dir;

			int m_zoomRange;
			int m_slideIgnoreAngle;
			bool m_doubleClickZoom;
			int m_doubleClickZoomRange;
			QString m_textColor;
			QString m_bgColor;

			static const int M_Zoom_Range = 2;
			static const int M_Slide_Ignore_Angle = 20;
			static const bool M_Double_Click_Zoom = true;
			static const int M_Double_Click_Zoom_Range = 4;

			Q_DISABLE_COPY(text_browser)
	};

	class graphics_text_browser : public QGraphicsProxyWidget
	{
		Q_OBJECT

		public:
			graphics_text_browser(QGraphicsWidget *parent = 0);
			~graphics_text_browser();
			void setText(const QString &text) { m_textBrowser->setText(text); }
			QString text() const { return m_textBrowser->toPlainText(); }
			void setTextColor(const QString &color) { m_textBrowser->setTextColor(QColor(color)); }
			void setBgColor(const QString &color) { m_textBrowser->setTextBackgroundColor(QColor(color)); }

Q_SIGNALS:
			void linkActivated(const QString &link);

		private Q_SLOTS:
			void linkClicked(const QUrl &url);

		private:
			text_browser *m_textBrowser;
	};
}

#endif
