#ifndef _KARIN_TABBUTTON_H
#define _KARIN_TABBUTTON_H

#include <MListItem>

class MButton;
class MLabel;
class MCommonLabelHighlighter;
class QGraphicsAnchorLayout;

namespace karin
{
	class tab_bar;

	class tab_button : public MListItem
	{
		Q_OBJECT

		public:
			tab_button(QGraphicsItem *parent = 0);
			virtual ~tab_button();
			void setLabel(const QString &name);

			public Q_SLOTS:
				void setLabel(const QString &name, qint64 id);
			void setHighlight(bool b);
			void doSelectionModeChange(bool b, qint64 id);

Q_SIGNALS:
			void showTab(qint64 id);
			void closeTab(qint64 id);
			void requestSelectionMode(bool b, qint64 id);

			private Q_SLOTS:
				void doClicked();
			void doClicked2(bool b);
			void doClicked3(const QString &text);
			void doLongTapped(bool b);

		private:
			MLabel *label;
			MCommonLabelHighlighter *hler;
			MButton *button;
			MButton *button2;
			qint64 widget;
			QGraphicsAnchorLayout *layout;

			friend class tab_bar;
			Q_DISABLE_COPY(tab_button)
	};
}

#endif

