#ifndef _KARIN_MENU_ITEM_H
#define _KARIN_MENU_ITEM_H\

#include "rect.h"

namespace karin
{
	class button;

	class abstract_menu_item : public rect
	{
		Q_OBJECT

		public:
			abstract_menu_item(QGraphicsWidget *parent = 0);
			virtual ~abstract_menu_item();
			void setName(const QString &name) { m_name = name; }
			void setValue(const QVariant &value) { m_value = value; }
			QString name() const { return m_name; }
			QVariant value() const { return m_value; }
			virtual bool isOn() const = 0;
			virtual void setOn(bool b) = 0;

Q_SIGNALS:
			void triggered(const QString &name, const QVariant &value = QVariant());

		protected:
			QString m_name;
			QVariant m_value;

		private:
			Q_DISABLE_COPY(abstract_menu_item)
	};

	class menu_item : public abstract_menu_item
	{
		Q_OBJECT

		public:
			menu_item(QGraphicsWidget *parent = 0);
			virtual ~menu_item();
			virtual bool isOn() const override;
			virtual void setOn(bool b) override;
			virtual void reset();
			void touchEvent(bool on);
			void cancel();
			void setAutoRelease(bool b);

		public Q_SLOTS:
		void setText(const QString &text);

Q_SIGNALS:
			void triggered(const QString &name, const QVariant &value = QVariant());
			void clicked(bool on = false);

		private Q_SLOTS:
			void clickedSlot(bool b = false);

		private:
		button *m_btn;

			Q_DISABLE_COPY(menu_item)
	};

	class menu_item_switcher : public menu_item
	{
		Q_OBJECT

		public:
			menu_item_switcher(QGraphicsWidget *parent = 0);
			virtual ~menu_item_switcher();
			bool isOn() const override { return m_on; }
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			virtual void reset() override;
			void setOn(bool b) override;

Q_SIGNALS:
			void valueChanged(bool on);

		protected:
			void resizeEvent(QGraphicsSceneResizeEvent* event);

		private Q_SLOTS:
			void clickedSlot(bool b = false);

		private:
		bool m_on;
		int m_ellipseRadius;
		QPointF m_ellipseCenter;
		int m_ellipseBorderWidth;
		QColor m_ellipseColor;

		static const int M_EllipseRadius = 60;
		static const int M_EllipseBorderWidth = 4;
		static const int M_EllipseColor = Qt::blue;

			Q_DISABLE_COPY(menu_item_switcher)
	};

	class menu_item_checkbox : public menu_item
	{
		Q_OBJECT

		public:
			menu_item_checkbox(QGraphicsWidget *parent = 0);
			virtual ~menu_item_checkbox();
			bool isOn() const override { return m_on; }
			virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
			virtual void reset() override;
			void setOn(bool b) override;

Q_SIGNALS:
			void valueChanged(bool on);

		protected:
			void resizeEvent(QGraphicsSceneResizeEvent* event);

		private Q_SLOTS:
			void clickedSlot(bool b = false);

		private:
		bool m_on;
		int m_ellipseRadius;
		QPointF m_ellipseCenter;
		int m_ellipseBorderWidth;
		QColor m_ellipseColor;

		static const int M_EllipseRadius = 60;
		static const int M_EllipseBorderWidth = 4;
		static const int M_EllipseColor = Qt::blue;

			Q_DISABLE_COPY(menu_item_checkbox)
	};

};

#endif
