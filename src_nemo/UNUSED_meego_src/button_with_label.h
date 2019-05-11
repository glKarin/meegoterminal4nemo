#ifndef _KARIN_BUTTONWITHLABEL_H
#define _KARIN_BUTTONWITHLABEL_H

#include <QGraphicsWidget>

class MButton;
class MLabel;
class QFont;

namespace karin
{
	class button_with_label : public QGraphicsWidget
	{
		Q_OBJECT

		public:
			enum karin_ViewType
			{
				checkboxType = 0,
				switchType
			};

			button_with_label(karin_ViewType type = checkboxType, QGraphicsItem *parent = 0);
			virtual ~button_with_label();
			void setTitle(const QString &title);
			void setTitleColor(const QString &color);
			void setTitleFont(const QFont &font);
			QFont titleFont() const;
			void setCheckable(bool b);
			void setChecked(bool b);
			void setEnabled(bool b);
			inline MButton * getButton()
			{
				return button;
			}

Q_SIGNALS:
			void pressed();
			void released();
			void clicked(bool checked = false);
			void toggled(bool checked);

		private:
			MLabel *label;
			MButton *button;

			Q_DISABLE_COPY(button_with_label)
	};
}

#endif
