#include <QGraphicsWidget>
#include <QGraphicsLayout>
#include <QGraphicsScene>

#define K_ZERO 0.00001

namespace karin
{
	bool Math_IsZero(float f)
	{
		return f < K_ZERO && f > -K_ZERO;
	}

	float Math_ComputeAngle(int x, int y)
	{
		float a = (float)x;
		float b = (float)y;
		if(a == 0 && b > 0)
			return 270.0;
		if(a == 0 && b < 0)
			return 90.0;
		if(b == 0 && a > 0)
			return 0.0;
		if(b == 0 && a < 0)
			return 180.0;
		float c = sqrt(a * a + b * b);
		//bxb+bxb/2bc
		float arg = acos((float)(abs(a)) / c) * (180.0 / M_PI);
		if(a > 0 && b > 0)
		{
			return 360.0 - arg;
		}
		else if(a > 0 && b < 0)
		{
			return arg;
		}
		else if(a < 0 && b > 0)
		{
			return 180.0 + arg;
		}
		else if(a < 0 && b < 0)
		{
			return 180.0 - arg;
		}
		else
			return 0.0;
	}

	void Widget_SetFixedWidth(QGraphicsWidget *widget, qreal w)
	{
		if(!widget)
			return;
		widget->setMaximumWidth(w);
		widget->setMinimumWidth(w);
		widget->setPreferredWidth(w);
		K_SET_WIDTH(widget, w);
	}

	void Widget_SetFixedHeight(QGraphicsWidget *widget, qreal h)
	{
		if(!widget)
			return;
		widget->setMaximumHeight(h);
		widget->setMinimumHeight(h);
		widget->setPreferredHeight(h);
		K_SET_HEIGHT(widget, h);
	}

	void Widget_SetFixedSize(QGraphicsWidget *widget, qreal w, qreal h)
	{
		if(!widget)
			return;
		Widget_SetFixedSize(widget, QSizeF(w, h));
	}

	void Widget_SetFixedSize(QGraphicsWidget *widget, const QSizeF &s)
	{
		if(!widget)
			return;
		widget->setMaximumSize(s);
		widget->setMinimumSize(s);
		widget->setPreferredSize(s);
		widget->resize(s);
	}

	void Widget_LayoutClear(QGraphicsLayout *layout)
	{
		if(!layout)
			return;
		while(layout->count())
			layout->removeAt(0);
	}

};
