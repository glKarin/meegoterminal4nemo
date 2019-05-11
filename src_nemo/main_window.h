#ifndef _KARIN_MAIN_WINDOW_H
#define _KARIN_MAIN_WINDOW_H

#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsWidget;

namespace karin
{
	class main_window : public QGraphicsView
	{
		Q_OBJECT

		public:
			main_window(QWidget *parent = 0);
			virtual ~main_window();
			QGraphicsScene * scene() { return m_scene; };
			QGraphicsWidget * currentWidget() { return m_widget; };
			void setCentralWidget(QGraphicsWidget *widget = 0);
			QGraphicsWidget * deattachWidget();
			QGraphicsWidget * replaceWidget(QGraphicsWidget *widget = 0);

Q_SIGNALS:
			void viewSceneRectChanged(const QRectF &r);

		protected:
			virtual void resizeEvent(QResizeEvent *event) override;

			private Q_SLOTS:
			void sceneRectChangedSlot(const QRectF &r);
			void keyboardRectangleChangedSlot();

		private:
			QGraphicsScene *m_scene;
			QGraphicsWidget *m_widget;
			QSizeF m_viewportSize;

			Q_DISABLE_COPY(main_window)
	};
};

#endif
