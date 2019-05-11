TEMPLATE	= lib
VERSION		= 0.2.0

TARGET		= konsole

CONFIG		+= qt debug_and_release warn_on #plugin #staticlib #dll build_all

QT += core gui widgets

MOC_DIR 	= ../.moc

CONFIG(debug, debug|release) {
    OBJECTS_DIR = ../.objs_d
		CONFIG += staticlib
		DESTDIR 	= .
    TARGET 	= konsole
} else {
    OBJECTS_DIR = ../.objs
		CONFIG += plugin
    TARGET 	= konsole
		DESTDIR 	= .
}

# workaround for https://bugreports.qt.nokia.com/browse/QTBUG-3883
QMAKE_DISTCLEAN += ${DESTDIR}/${TARGET}

DEFINES 	+= HAVE_POSIX_OPENPT	    
#or DEFINES 	+= HAVE_GETPT

HEADERS 	= TerminalCharacterDecoder.h Character.h CharacterColor.h \
		KeyboardTranslator.h \
		ExtendedDefaultTranslator.h \
		Screen.h History.h BlockArray.h konsole_wcwidth.h \
		ScreenWindow.h \
		Emulation.h \
		Vt102Emulation.h TerminalDisplay.h Filter.h LineFont.h \
		Pty.h kpty.h kpty_p.h k3process.h k3processcontroller.h \
		Session.h ShellCommand.h \
		qgraphicstermwidget.h

SOURCES 	= TerminalCharacterDecoder.cpp \
		KeyboardTranslator.cpp \
		Screen.cpp History.cpp BlockArray.cpp konsole_wcwidth.cpp \
		ScreenWindow.cpp \
		Emulation.cpp \
		Vt102Emulation.cpp TerminalDisplay.cpp Filter.cpp \
		Pty.cpp kpty.cpp k3process.cpp k3processcontroller.cpp \
		Session.cpp ShellCommand.cpp \
		qgraphicstermwidget.cpp

lib.files = libkonsole.so
lib.path = /usr/lib

INSTALLS += lib
