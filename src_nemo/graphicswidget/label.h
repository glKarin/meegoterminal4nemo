#ifndef _KARIN_LABEL_H
#define _KARIN_LABEL_H

#include "rect.h"

namespace karin
{
	class label : public rect
	{
		Q_OBJECT
		public:
			label(QGraphicsWidget *parent = 0);
			virtual ~label();
			QString text() const { return m_text; }
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			QColor color() const { return m_color; }
			void setColor(const QColor &c);

			public Q_SLOTS:
			void setText(const QString &s);

		protected:
			QString m_text;
			QColor m_color;

			static const int M_Color = Qt::black;

			Q_DISABLE_COPY(label)
	};
};

typedef karin::label MLabel;

#endif
