#ifndef _KARIN_K_H
#define _KARIN_K_H

#include <QPointF>
#include <QFont>
#include <QStringList>

#define OBJECTNAME_BASE_KEY "karin::rect::base_key"
#define OBJECTNAME_GENERAL_KEY "karin::rect::base_key::general_key"
#define OBJECTNAME_MODIFIER_KEY "karin::rect::base_key::modifier_key"

#define OBJECTNAME_TERM_CONTAINER "karin::term_container"
#define OBJECTNAME_MAIN_WINDOW "karin::main_window"
#define OBJECTNAME_VIEWPORT "karin::viewport"

#define OBJECTNAME_PANNABLE "karin::rect::pannable"
#define OBJECTNAME_LIST_VIEW "karin::rect::pannable::list_view"
#define OBJECTNAME_TOOLBAR "karin::rect::pannable::list_view::toolbar"
#define OBJECTNAME_MENU_ITEM_GROUP "karin::rect::pannable::list_view::menu_item_group"
#define OBJECTNAME_MENU "karin::rect::pannable::list_view::menu"
#define OBJECTNAME_LIST_VIEW_LAYOUT "karin::list_view_layout"

#define OBJECTNAME_BUTTON "karin::rect::button"
#define OBJECTNAME_TOOL_ICON "karin::rect::button:tool_icon"
#define OBJECTNAME_ABSTRACT_MENU_ITEM "karin::rect::button::abstract_menu_item"
#define OBJECTNAME_MENU_ITEM "karin::rect::abstract_menu_item::menu_item"
#define OBJECTNAME_MENU_ITEM_SWITCHER "karin::rect::abstract_menu_item::menu_item::menu_item_switcher"
#define OBJECTNAME_MENU_ITEM_CHECKBOX "karin::rect::abstract_menu_item::menu_item::menu_item_checkbox"

#define OBJECTNAME_RECT "karin::rect"
#define OBJECTNAME_LABEL "karin::rect::label"
#define OBJECTNAME_SCROLL_VIEW "karin::rect::scroll_view"
#define OBJECTNAME_SCROLL_BAR "karin::rect::scroll_bar"

#define OBJECTNAME_POPUP "karin::rect::popup"
#define OBJECTNAME_TIMER_POPUP "karin::rect::popup::timer_popup"
#define OBJECTNAME_MAIN_MENU "karin::rect::popup::main_menu"
#define OBJECTNAME_MSG_BOX "karin::rect::popup::timer_popup::msg_box"
#define OBJECTNAME_INFO_BANNER "karin::rect::popup::timer_popup::info_banner"
#define OBJECTNAME_DIALOG "karin::rect::popup::dialog"
#define OBJECTNAME_MSG_DIALOG "karin::rect::popup::dialog::msg_dialog"

#define OBJECTNAME_DIALOG_TITLE_BAR "karin::rect::dialog_title_bar"
#define OBJECTNAME_DIALOG_TOOL_BAR "karin::rect::dialog_tool_bar"

#define OBJECTNAME_COMBOBOX "karin::rect::abstract_menu_item::combobox"
#define OBJECTNAME_BUTTON_GROUP "karin::rect::button_group"
#define OBJECTNAME_OVERLAY_TOOLBAR "karin::rect::overlay_toolbar"

#define OBJECTNAME_TERMINAL "karin::terminal"
#define OBJECTNAME_TAB_BUTTON "karin::tab_button"
#define OBJECTNAME_TAB_GROUP "karin::tab_group"
#define OBJECTNAME_TAB_MODEL "karin::tab_model"

#define OBJECTNAME_MTERMWIDGET "MTermWidget"
#define OBJECTNAME_MTERMINALDISPLAY "MTerminalDisplay"

#define OBJECTNAME_TEXT_BROWSER "karin::text_browser"
#define OBJECTNAME_GRAPHICS_TEXT_BROWSER "karin::graphics_text_browser"

// without Q_FOREACH and C++11 for-in
#define K_FOREACH(C, c) for(C::iterator itor = (c).begin(); itor != (c).end(); ++itor)
#define K_FOREACH_CONST(C, c) for(C::const_iterator itor = (c).begin(); itor != (c).end(); ++itor)
#define K_SUB_FOREACH(C, c) for(C::iterator sub_itor = (c).begin(); sub_itor != (c).end(); ++sub_itor)
#define K_SUB_FOREACH_CONST(C, c) for(C::const_iterator sub_itor = (c).begin(); sub_itor != (c).end(); ++sub_itor)

#define K_FOREACH_INDEX(c) for(int i = 0; i < (c).size(); i++)

#define K_SET_WIDTH(item, w) (item)->resize(w, (item)->size().height())
#define K_SET_HEIGHT(item, h) (item)->resize((item)->size().width(), h)
#define K_GET_WIDTH(item) (item)->size().width()
#define K_GET_HEIGHT(item) (item)->size().height()

#define K_DELETE_DEBUG() qDebug() << QString("[%1]: %2").arg("Destroyed").arg(this->objectName())
#define DELETE_DEBUG(c) qDebug() << QString("[%1]: %2").arg("Destroyed").arg(c)

#define K_MASK(n) (1 << (n))
#define K_LONG_MASK(n) (1L << (n))
#define K_MASK_ALL (~0)
#define K_LONG_MASK_ALL (~0L)

#define KARIN_COLOR "#9E1B29"

typedef unsigned mask_t;
typedef unsigned int enum_t;


class QGraphicsWidget;
class QGraphicsLayout;
class QGraphicsScene;

namespace karin
{
	QPointF Font_GetStringCenter(const QString &str, const QFont &font);
	int String_StringToInt(const QString &str, bool *ok = 0);
	QString String_ArrayToString(const QStringList &str, int *ok = 0);
	bool Object_IsType(const QObject *obj, const QString &type);

	inline bool Math_IsZero(float f);
	inline void Widget_SetFixedWidth(QGraphicsWidget *widget, qreal w);
	inline void Widget_SetFixedHeight(QGraphicsWidget *widget, qreal h);
	inline void Widget_SetFixedSize(QGraphicsWidget *widget, qreal w, qreal h);
	inline void Widget_SetFixedSize(QGraphicsWidget *widget, const QSizeF &s);
	inline void Widget_LayoutClear(QGraphicsLayout *layout);
	inline float Math_ComputeAngle(int x, int y);
};

#include "k.inc.cpp"

#endif
