TEMPLATE        = app
#DESTDIR         = ..
DESTDIR         = ./

CONFIG          += qt debug_and_release warn_on # build_all

CONFIG += link_pkgconfig
PKGCONFIG += maliit-framework

QT += core gui widgets


DEFINES += APP_NAME=\\\"Karin-Console\\\"
DEFINES += APP_NAME_SETTINGS=\\\"karin-console\\\"
DEFINES += DEVELOPER=\\\"Karin\\\"
DEFINES += VERSION=\\\"0.2.5harmattan3\\\"

CONFIG(debug, debug|release) {
    OBJECTS_DIR = ../.objs_d/src_nemo
MOC_DIR         = ../.moc_d/src_nemo
    TARGET      = karin-console_d
		#LIBS            += -L../lib ../lib/libkonsole.a
    #PRE_TARGETDEPS += ../lib/libkonsole.a
		DEFINES += _KARIN_LOCAL_
		DEFINES += _KARIN_PREFIX_=\\\".\\\"
		DEFINES 	+= HAVE_POSIX_OPENPT	    
		#or DEFINES 	+= HAVE_GETPT

		HEADERS 	= ../lib/TerminalCharacterDecoder.h ../lib/Character.h ../lib/CharacterColor.h \
		../lib/KeyboardTranslator.h \
		../lib/ExtendedDefaultTranslator.h \
		../lib/Screen.h ../lib/History.h ../lib/BlockArray.h ../lib/konsole_wcwidth.h \
		../lib/ScreenWindow.h \
		../lib/Emulation.h \
		../lib/Vt102Emulation.h ../lib/TerminalDisplay.h ../lib/Filter.h ../lib/LineFont.h \
		../lib/Pty.h ../lib/kpty.h ../lib/kpty_p.h ../lib/k3process.h ../lib/k3processcontroller.h \
		../lib/Session.h ../lib/ShellCommand.h \
		../lib/qgraphicstermwidget.h

		SOURCES 	= ../lib/TerminalCharacterDecoder.cpp \
		../lib/KeyboardTranslator.cpp \
		../lib/Screen.cpp ../lib/History.cpp ../lib/BlockArray.cpp ../lib/konsole_wcwidth.cpp \
		../lib/ScreenWindow.cpp \
		../lib/Emulation.cpp \
		../lib/Vt102Emulation.cpp ../lib/TerminalDisplay.cpp ../lib/Filter.cpp \
		../lib/Pty.cpp ../lib/kpty.cpp ../lib/k3process.cpp ../lib/k3processcontroller.cpp \
		../lib/Session.cpp ../lib/ShellCommand.cpp \
		../lib/qgraphicstermwidget.cpp

		} else {
		OBJECTS_DIR = ../.objs/src_nemo
MOC_DIR         = ../.moc/src_nemo
		TARGET      = karin-console
		LIBS            += -L../lib -lkonsole
		PRE_TARGETDEPS += ../../lib/libkonsole.so
		DEFINES += _KARIN_INSTALL_
		DEFINES += _KARIN_PREFIX_=\\\"/usr/share/karin-console\\\"
		}

		#LMTP_HEADERS    = lmtp/mtopleveloverlay.h lmtp/meditortoolbararrow.h \
		#                  lmtp/meditortoolbar_p.h lmtp/meditortoolbar.h
		#LMTP_SOURCES    = lmtp/mtopleveloverlay.cpp lmtp/meditortoolbararrow.cpp \
		#                  lmtp/meditortoolbar.cpp

		#HEADERS         = tab_model.h button_with_label.h tab_group.h karin_ut.h tab_button.h terminal.h tab_bar.h MTermWidget.h MTerminalDisplay.h $$LMTP_HEADERS
		#SOURCES         = main.cpp tab_model.cpp button_with_label.cpp tab_group.cpp karin_ut.cpp tab_button.cpp terminal.cpp tab_bar.cpp MTermWidget.cpp MTerminalDisplay.cpp $$LMTP_SOURCES

		HEADERS         += \
		karin_ut.h \
		tab_model.h \
		terminal.h \
		tab_group.h

		SOURCES         += \
		karin_ut.cpp  \
		tab_model.cpp \
		terminal.cpp \
		tab_group.cpp

		HEADERS         += MTerminalDisplay.h MTermWidget.h
		SOURCES         += MTerminalDisplay.cpp MTermWidget.cpp

		HEADERS += \
		k.h \
		main_window.h \
		toolbar.h \
		overlay_toolbar.h \
		menu.h \
		key.h

		SOURCES += \
		main_window.cpp \
		key.cpp \
		general_key.cpp \
		modifier_key.cpp \
		toolbar.cpp \
		toolbar_load.cpp \
		overlay_toolbar.cpp \
		k.cpp \
		k.inc.cpp \
		menu.cpp \
		main.cpp

		SOURCES += \
		graphicswidget/pannable.cpp \
		graphicswidget/list_view.cpp \
		graphicswidget/button.cpp \
		graphicswidget/menu_item.cpp \
		graphicswidget/label.cpp \
		graphicswidget/menu_item_group.cpp \
		graphicswidget/combobox.cpp \
		graphicswidget/popup.cpp \
		graphicswidget/msg_box.cpp \
		graphicswidget/button_group.cpp \
		graphicswidget/dialog.cpp \
		graphicswidget/text_browser.cpp \
		graphicswidget/tool_icon.cpp \
		graphicswidget/scroll_view.cpp \
		graphicswidget/rect.cpp

		HEADERS += \
		graphicswidget/pannable.h \
		graphicswidget/list_view.h \
		graphicswidget/button.h \
		graphicswidget/menu_item.h \
		graphicswidget/label.h \
		graphicswidget/menu_item_group.h \
		graphicswidget/combobox.h \
		graphicswidget/popup.h \
		graphicswidget/msg_box.h \
		graphicswidget/button_group.h \
		graphicswidget/dialog.h \
		graphicswidget/text_browser.h \
		graphicswidget/tool_icon.h \
		graphicswidget/scroll_view.h \
		graphicswidget/rect.h

		INCLUDEPATH     = . ../lib ./lmtp graphicswidget

		#LIBS           += -L.. -lqgraphicstermwidget

		desktop_entry.path = $$[QT_INSTALL_PREFIX]/share/applications
		desktop_entry.files = karin-console.desktop

		imtoolbars.path = $$[QT_INSTALL_PREFIX]/share/karin-console/toolbars
		imtoolbars.files = toolbars/shell.xml toolbars/arrows.xml toolbars/function.xml toolbars/karin.xml

		target.path = $$[QT_INSTALL_PREFIX]/bin

		style.files = style/karin_mstyle.css
		style.path = $$[QT_INSTALL_PREFIX]/share/karin-console/style

		i18n.files = i18n/karin-console.zh_CN.ts i18n/karin-console.zh_CN.qm
		i18n.path = $$[QT_INSTALL_PREFIX]/share/karin-console/i18n

		rsc.files = resource/toolbarxml.html
		rsc.path = $$[QT_INSTALL_PREFIX]/share/karin-console/resource

		icon.files = karin-console.png
		icon.path = $$[QT_INSTALL_PREFIX]/share/karin-console/icon

		INSTALLS        += target desktop_entry imtoolbars style i18n rsc icon
