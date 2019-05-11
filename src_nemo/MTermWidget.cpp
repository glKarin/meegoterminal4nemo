/*
    This file is part of meego-terminal

    Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).

    Contact: Ruslan Mstoi <ruslan.mstoi@nokia.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

//for maliit inpit method
//k #include <maliit/inputmethod.h>
#include <maliit/namespace.h>

#include "MTermWidget.h"
#include "MTerminalDisplay.h"
#include "Session.h"
#include "ScreenWindow.h"
#include "Screen.h"
#include "karin_ut.h"
#include "scroll_view.h"
#include "toolbar.h"
#include "msg_box.h"
#include "k.h"

#include <QGraphicsSceneMouseEvent>
#include <QSwipeGesture>
#include <QPinchGesture>
#include <QGestureEvent>

/*
class QGraphicsItemPrivate {
public:
    void clearSubFocus(QGraphicsItem *rootItem = 0, QGraphicsItem *stopItem = 0);
};
*/

// multiplier that converts display lines into viewport values, purpose is to
// make panning easier to use (i.e. not too sensitive)
#define PANM 10 
#define SCALE_FACTOR 1.0

/**
 * [1] Display cannot be set as viewports widget because it handles history
 * itself, so it does not render all of its contents as viewport expects. Also,
 * viewport does not support abstract ranges but range is in actual
 * coordinates. So, fake widget is used to trick the viewport to handle the pan
 * gestures. This class does the conversion from viewport coordinates into
 * terminal lines.
 */
MTermWidget::MTermWidget(int startnow, QGraphicsWidget *parent, qint64 id):
    QGraphicsTermWidget(parent), // do custom session and display creation
    m_lastSetCurrentLine(0),
    m_colorScheme(1), // green on black
		m_selectionButton(false),
    m_display(0),
		mwId(id),
		autoSelectionEnabled(false),
		userDisabledAutoSelection(false),
		prevPosY(0),
		lastTotalScaleFactor(SCALE_FACTOR),
		m_viewport(new karin::scroll_view(this))
{
    construct(startnow);
    m_display = dynamic_cast<MTerminalDisplay*>(m_terminalDisplay);
    Q_ASSERT(m_display);
    //setupMenu();

    setAutoFillBackground(true); // see MTermWidget::setColorScheme [1]

    m_viewport->setAutoRange(false);
		m_viewport->setZValue(1);

    // note fake widget, see [1]
		/*
    QGraphicsWidget *w = new QGraphicsWidget(this);
    w->setObjectName("WV");
    w->setFlag(QGraphicsItem::ItemHasNoContents, true);
		*/

    connect(m_terminalDisplay, SIGNAL(scrollChanged(int, int)),
            this, SLOT(updateViewport(int, int)));

    connect(m_viewport, SIGNAL(positionChanged(const QPointF &)),
             this, SLOT(viewportPositionChanged(const QPointF &)));
    connect(m_terminalDisplay, SIGNAL(changedFontMetricSignal(int, int)),
            this, SLOT(displayFontChanged()));

#if 0
    connect(qApp->inputMethod(),
            SIGNAL(keyboardRectangleChanged()),
            this, SLOT(onInputMethodAreaChanged()),
            Qt::UniqueConnection);
#endif

    enableGestures();
    readSettings();

		// setAcceptTouchEvents(true);
}

/**
 * Creates session and display and does initialization. Basically does the same
 * as the non-default base constructor, but creates MTerminalDisplay instead of
 * TerminalDisplay.
 */
void MTermWidget::construct(int startnow)
{
    m_session = createSession();
    m_terminalDisplay = createTerminalDisplay(m_session);

    init();

    if (m_session) {
				connect(m_session, SIGNAL(titleChanged()), this, SLOT(doTitleChanged()));
				connect(m_session, SIGNAL(finished()), this, SLOT(doFinished()));
    }
		if(startnow && m_session)
        m_session->run();

    // set focus policy so that display gets focus, default focus policy is
    // Qt::NoFocus which means ItemIsFocusable flag is not set
     this->setFocusPolicy( m_terminalDisplay->focusPolicy() ); 

    this->setFocus( Qt::OtherFocusReason );
    m_terminalDisplay->resize(this->size());
    
    this->setFocusProxy(m_terminalDisplay);
}

MTermWidget::~MTermWidget()
{
    //writeSettings(); // could do it also in sessionFinished
	/*
			m_session -> close();
			m_session -> removeView(m_terminalDisplay);
			*/

	DELETE_DEBUG(OBJECTNAME_MTERMWIDGET);
}

TerminalDisplay *MTermWidget::createTerminalDisplay(Session *session)
{
    TerminalDisplay* display = new MTerminalDisplay(this);
    display->setObjectName("MTerminalDisplay");

    display->setBellMode(TerminalDisplay::NotifyBell);
    display->setTerminalSizeHint(true);
    display->setTripleClickMode(TerminalDisplay::SelectWholeLine);
    display->setTerminalSizeStartup(true);

    display->setRandomSeed(session->sessionId() * 31);
    
    return display;
}

void MTermWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
#if 0
    QRect imArea = qApp->inputMethod()->keyboardRectangle().toRect();

    if (karin::ut::Instance()->getSetting<bool>(TRANSLUCENT_INPUTMETHOD) || imArea.isEmpty()) { // vkb is not up
        QGraphicsTermWidget::resizeEvent(event);
        m_viewport->resize(this->size());
    }
    // vkb is up
    else {
        int newHeight = 0; // = where vkb starts - status bar
        
        if (imArea.x() == 0) { // landscape
            newHeight = imArea.y() - scenePos().y();
        }

        else if (imArea.y() == 0) { // portrait
            newHeight = imArea.x() - scenePos().x();
        }

        if (newHeight > 0) {  // resize to occupy area not taken  by vkb
            m_terminalDisplay->resize(size().width(), newHeight);
            m_viewport->resize(size().width(), newHeight);
        }
    }
		QRectF range = m_viewport->range();
		m_viewport->setRange(QRectF(range.left(), range.top(), size().width(), range.height()));
#else
		QGraphicsTermWidget::resizeEvent(event);
		m_viewport->resize(this->size());
#endif
}

/**
 * At least when coming to portrait mode this method is called after
 * resizeEvent, so trigger resizeEvent. Parameter is null, it is not used
 * anyway.
 */
void MTermWidget::onInputMethodAreaChanged()
{
		if(isVisible())
			if(!karin::ut::Instance() -> getSetting<bool>(TRANSLUCENT_INPUTMETHOD))
				resizeEvent(0);
}

void MTermWidget::updateViewport(int currentLine, int maxLines)
{
    QRectF newRange = m_viewport->range();
    newRange.setHeight(maxLines * PANM);

    QPointF newPos(m_viewport->position().x(), currentLine * PANM);

    // avoid rounding error when called due to viewportPositionChanged
    if (m_lastSetCurrentLine == currentLine && maxLines) {
        newPos.setY(m_viewport->position().y());
    }

    if (newRange != m_viewport->range()) {
        m_viewport->setRange(newRange);
    }

    if (newPos != m_viewport->position()) {
        m_viewport->setPosition(newPos);
    }

    //static bool autoSelectionEnabled = false;
    //static bool userDisabledAutoSelection = false;

    // user disabled auto-enabled selection mode, maybe to pinch, or swipe...
    if (autoSelectionEnabled && !m_selectionButton) {
        userDisabledAutoSelection = true;
    }
    if (userDisabledAutoSelection)
        return;

    // disable selection mode if there is history and was auto-enabled
    if (maxLines && m_selectionButton && autoSelectionEnabled) {
        autoSelectionEnabled = false;
        toggleSelectionMode(false);
    }

    // auto-enable selection mode if there is no history
    else if (!maxLines && !m_selectionButton &&
             !autoSelectionEnabled) {
        autoSelectionEnabled = true;
        toggleSelectionMode(true);
    }
}

void MTermWidget::viewportPositionChanged(const QPointF &position)
{
    //static qreal prevPosY = 0;
    qreal posY = position.y();

    if (posY <  0) {// spring action this
        posY = 0;
    }

    if (posY > m_viewport->range().height()) {// spring action this
        posY = m_viewport->range().height();
    }
    
    if (prevPosY == posY) {
        return;
    }

    prevPosY = posY;
    m_lastSetCurrentLine = posY / PANM;

    const bool atEndOfOutput = (posY == m_viewport->range().height());
    m_terminalDisplay->viewportPositionChanged(m_lastSetCurrentLine,
                                               atEndOfOutput);
}

bool MTermWidget::event(QEvent *event)
{
	//k case QEvent::GestureOverride:
	if (event->type() == QEvent::Gesture)
		return gestureEvent(static_cast<QGestureEvent*>(event));
	return QGraphicsTermWidget::event(event);
}

bool MTermWidget::gestureEvent(QGestureEvent *event)
{
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
         swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    
    return true;
}

/**
 * The complexity in handling swipe directions is because QSwipeGesture does
 * not understand orientation change. For example, after a change from
 * landscape to portrait horizontal swipes are reported for vertical ones and
 * vice versa.
 */
void MTermWidget::swipeTriggered(QSwipeGesture *gesture)
{
    if (gesture->state() != Qt::GestureFinished) {
        return;
    }

    // defaults assume landscape orientation
    QSwipeGesture::SwipeDirection otherDirection =
        gesture->verticalDirection();
    QSwipeGesture::SwipeDirection currentDirection =
        gesture->horizontalDirection();
    QSwipeGesture::SwipeDirection prevDirection = QSwipeGesture::Left;
    QSwipeGesture::SwipeDirection nextDirection = QSwipeGesture::Right;

    // portrait (at the time of writing this QSystemDisplayInfo.orientation did
    // not work, it always returned QSystemDisplayInfo::Landscape)
    //k if (size().width() == 480) 
		Qt::ScreenOrientation screenOrientation = QGuiApplication::primaryScreen()->orientation();
		if(screenOrientation == Qt::PortraitOrientation || screenOrientation == Qt::InvertedPortraitOrientation)
		{
        otherDirection = gesture->horizontalDirection();
        currentDirection = gesture->verticalDirection();
        prevDirection = QSwipeGesture::Up;
        nextDirection = QSwipeGesture::Down;
    }

    // accept only clean swipes in the needed direction, i.e. no simultaneous
    // accidental swipes in both directions
    if (otherDirection != QSwipeGesture::NoDirection) {
        return;
    }

    if (karin::toolbar::getAllToolbars().isEmpty())
        return;

    if (currentDirection == prevDirection) {
        m_display->setPrevActiveToolbar();
    }
    else if (currentDirection == nextDirection) {
        m_display->setNextActiveToolbar();
    }
    //m_toolbarsComboBox->setCurrentIndex(m_display->activeToolbarIndex());
		if(karin::ut::Instance() -> getSetting<bool>(SHOW_BANNER))
			showBanner(tr("Active toolbar is now '%1'"
                   ).arg("m_display->activeToolbar()->name"));
}

void MTermWidget::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();

    // initially scaleFactor is 1.0
    static const qreal scaleFactorInit = SCALE_FACTOR;
    //static qreal lastTotalScaleFactor = scaleFactorInit;
        
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        qreal totalScaleFactor = gesture->totalScaleFactor();
        qreal diff = totalScaleFactor - lastTotalScaleFactor;

        if (diff > .7) {
            m_terminalDisplay->increaseTextSize();
            lastTotalScaleFactor = totalScaleFactor;
        }
        if (diff < -.3) {
            m_terminalDisplay->decreaseTextSize();
            lastTotalScaleFactor = totalScaleFactor;
        }
    }
    if (gesture->state() == Qt::GestureFinished) {
        lastTotalScaleFactor = scaleFactorInit;
    }
}

void MTermWidget::displayFontChanged() const
{
	QFont font = m_terminalDisplay->getVTFont();
	karin::ut::Instance() -> setSetting<QString>(FONT, font.toString());

	if(karin::ut::Instance() -> getSetting<bool>(SHOW_BANNER))
    showBanner(tr("Font size is now %1 ").arg(font.pointSize()));
}

void MTermWidget::showBanner(const QString &title) const
{
    MBanner *banner = new MBanner(const_cast<MTermWidget *>(this));
    // InformationBanner takes up too much space
    // banner->setStyleName("InformationBanner"); 
    //k banner->setStyleName("ShortEventBanner");
    banner->setTitle(title);
    //k banner->appear(scene(), MSceneWindow::DestroyWhenDone);
		banner->appear(karin::timer_popup::DestroyWhenDone);
}

void MTermWidget::readSettings()
{
	karin::ut * const kut = karin::ut::Instance();
    QFont font;
    font.fromString(kut -> getSetting<QString>(FONT));
    m_colorScheme = kut -> getSetting<int>(COLOR_SCHEME_SETTING);
		if(m_colorScheme < 1 || m_colorScheme > 4)
			m_colorScheme = 1;
    QString activeToolbar = kut -> getSetting<QString>(ACTIVE_TOOLBAR);
    setTerminalFont(font);
    setColorScheme(m_colorScheme);
    m_display -> setActiveToolbar(kut -> getSetting<QString>(ACTIVE_TOOLBAR));
		setTranslucency(kut -> getSetting<bool>(TRANSLUCENT_INPUTMETHOD));
		setSuppression(kut -> getSetting<bool>(ENABLE_VKB));
		int c = kut -> getSetting<int>(CURSOR_TYPE);
		if(c >= 0 && c < 3)
			m_display -> setKeyboardCursorShape(static_cast<Konsole::TerminalDisplay::KeyboardCursorShape>(c));
		m_display -> setBlinkingCursor(kut -> getSetting<bool>(BLINKING_CURSOR));
}


/**
 * [1] Background color is set to the one in the scheme of the terminal
 * display. This is to avoid flicker when display is resized due to vkb
 * show/hide events. The display is resized in resizeEvent, but vkb is still
 * animating, though it reports full size, so for a while white default
 * background of this widget becomes visible: hence the flicker.
 */
void MTermWidget::setColorScheme(int scheme)
{
    QGraphicsTermWidget::setColorScheme(scheme);
    m_colorScheme = scheme;

    QColor bgColor = m_terminalDisplay->colorTable()[DEFAULT_BACK_COLOR].color;

    QPalette p = palette(); // see [1]
    p.setColor(QPalette::Window, bgColor);
    setPalette(p);

    // set color of UI widgets opposite of the color scheme background
    // FIXME: check for actual bkg color instead of schemes
    QString piStyle;
    // bright background
    if (m_colorScheme == COLOR_SCHEME_BLACK_ON_LIGHT_YELLOW) {
        piStyle = "CommonPositionIndicator";
    }
    else {  // dark background
        piStyle = "CommonPositionIndicatorInverted";
    }
    //k if (piStyle != m_viewport->positionIndicator()->styleName())
        //k m_viewport->positionIndicator()->setStyleName(piStyle);

    // update if changed not by combo box, i.e. by swiping
		/*
    if (m_colorSchemeComboBox->currentIndex() != scheme - 1)
        m_colorSchemeComboBox->setCurrentIndex(scheme -1);
		*/
}

/**
 * Overridden to check if the session changes the program in its run method. If
 * it does, then the user has specified invalid program name with the -e option
 * and the session has changed it to a shell program. In that case, shows a
 * message box explaining the case.
 */
void MTermWidget::startShellProgram()
{
    if ( m_session->isRunning() )
        return;

    QString oldProgram = m_session->program();
    QGraphicsTermWidget::startShellProgram();
    QString newProgram = m_session->program();

    if (oldProgram != newProgram) {
        QString title = tr("Warning!");
        QString text = tr("Could not find '%1', starting '%2' instead."
                               ).arg(oldProgram).arg(newProgram);

        MMessageBox *messageBox = new MMessageBox(title, text, this);
        //messageBox->setIconId("icon-l-error");
        messageBox->appear(karin::timer_popup::DestroyWhenDone);
    }
}

void MTermWidget::enableGestures()
{
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PinchGesture);
}

void MTermWidget::disableGestures()
{
    ungrabGesture(Qt::SwipeGesture);
    ungrabGesture(Qt::PinchGesture);
}

/**
 * If selection mode is enabled viewport panning and gestures are disabled.
 * That is because they interfere with selections mouse move events.
 */
void MTermWidget::toggleSelectionMode(bool selection)
{
    if (selection) {
        m_viewport->ungrabGesture(Qt::PanGesture);
        disableGestures();
        m_display->enableSelectionMode();
    }
    else {
        m_viewport->grabGestureWithCancelPolicy( Qt::PanGesture, Qt::GestureFlags()
            /*, MWidget::MouseEventCancelOnGestureStarted */ );

        enableGestures();
        m_display->disableSelectionMode();
    }
		if(m_selectionButton == selection)
			return;
		m_selectionButton = selection;

		emit selectionModeChanged(m_selectionButton, mwId);
}

void MTermWidget::toggleSelectionModeWithBanner(bool selection, qint64 id)
{
	if(id >= 0 && id == mwId)
	{
		toggleSelectionMode(selection);
		if(karin::ut::Instance()->getSetting<bool>(SHOW_BANNER))
			showBanner(tr("Text selection mode is now %1"
						).arg(selection ? tr("enabled") : tr("disabled")));
	}
}

void MTermWidget::doTitleChanged()
{
	if(m_session && mwId >= 0)
		emit titleChanged(m_session -> userTitle().isEmpty() ? "Karin Console" : m_session -> userTitle(), mwId);
}

void MTermWidget::doFinished()
{
	if(mwId >= 0)
		emit sessionFinished(mwId);
}

void MTermWidget::setSuppression(bool value)
{
	if(m_display)
	{
		m_display -> setProperty(Maliit::InputMethodQuery::suppressInputMethod, QVariant(!value));
#if 0
		QInputContext *ic = qApp -> inputContext();
		if(ic)
			ic -> update();
#else
		QInputMethod *im = qApp->inputMethod();
		im->update(Qt::ImQueryAll);
#endif
	}
}

void MTermWidget::setTranslucency(bool value)
{
	if(m_display)
	{
		m_display -> setProperty(Maliit::InputMethodQuery::translucentInputMethod, QVariant(value));
#if 0
		QInputContext *ic = qApp -> inputContext();
		if(ic)
			ic -> update();
#else
		QInputMethod *im = qApp->inputMethod();
		im->update(Qt::ImQueryAll);
#endif
	}
}

void MTermWidget::clearScreen()
{
	if(m_display && m_display -> screenWindow() && m_display -> screenWindow() -> screen())
	{
		m_display -> screenWindow() -> screen() -> clear();
	}
	if(m_session)
		m_session -> clearHistory();
}

void MTermWidget::resetScreen()
{
	if(m_display && m_display -> screenWindow() && m_display -> screenWindow() -> screen())
		m_display -> screenWindow() -> screen() -> reset(true);
	if(m_session)
		m_session -> clearHistory();
}

MTerminalDisplay * MTermWidget::terminalDisplay()
{
	return m_display;
}
