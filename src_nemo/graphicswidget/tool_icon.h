#ifndef _KARIN_TOOL_ICON_H
#define _KARIN_TOOL_ICON_H

#include "button.h"

class QPixmap;

namespace karin
{
	class tool_icon : public button
	{
		Q_OBJECT
		public:
			tool_icon(QGraphicsWidget *parent = 0);
			tool_icon(const QString &img, QGraphicsWidget *parent = 0);
			virtual ~tool_icon();
			QString image() const { return m_image; }
			void setImage(const QString &s);
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

		protected:
			QString m_image;
			QPixmap *m_pixmap;

			Q_DISABLE_COPY(tool_icon)
	};

};

#endif
