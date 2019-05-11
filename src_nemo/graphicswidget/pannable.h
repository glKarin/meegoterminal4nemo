#ifndef _KARIN_PANNABLE_H
#define _KARIN_PANNABLE_H

#include "rect.h"

namespace karin
{
	class pannable : public rect
	{
		Q_OBJECT

		public:
			pannable(QGraphicsWidget *parent = 0);
			~pannable();
			QGraphicsWidget * pannableWidget() { return m_pannableWidget; }
			void setPannableWidget(QGraphicsWidget *widget);
			void setAllowPan(bool b) { m_allowPan = b; }
			bool allowPan() const { return m_allowPan; }

			public Q_SLOTS:
				void reset();

		protected:
			virtual bool event(QEvent *e);
			virtual bool sceneEvent(QEvent *e);

		private:
			QGraphicsWidget *m_pannableWidget;
			bool m_allowPan;

			Q_DISABLE_COPY(pannable)
	};

};

typedef karin::pannable MPannableWidget;

#endif
