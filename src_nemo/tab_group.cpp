#include "tab_group.h"
#include "MTermWidget.h"
#include "k.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsScene>

karin::tab_group::tab_group(QGraphicsWidget *parent)
	: QGraphicsWidget(parent),
	layout(new QGraphicsAnchorLayout),
	current(-1),
	m_currentWidget(0),
	m_currentIndex(-1)
{
	setObjectName(OBJECTNAME_TAB_GROUP);
	layout->setContentsMargins(0.0, 0.0, 0.0, 0.0);
	setContentsMargins(0.0, 0.0, 0.0, 0.0);
	//setFocusProxy(centralWidget());
	setLayout(layout);
}

karin::tab_group::~tab_group()
{
}

MTermWidget * karin::tab_group::addTab(qint64 id, int *r_index)
{
	if(id < 0)
		return 0;
	MTermWidget *wid;
	MTermWidget *w;
	
	wid = new MTermWidget(false, this, id);

	layout->addAnchor(wid, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout->addAnchor(wid, Qt::AnchorTop, layout, Qt::AnchorTop);
	layout->addAnchor(wid, Qt::AnchorBottom, layout, Qt::AnchorBottom);
	layout->addAnchor(wid, Qt::AnchorRight, layout, Qt::AnchorRight);

	int i;
	if(m_currentWidget)
	{
		for(i = 0; i < m_tabs.size(); i++)
		{
			w = static_cast<MTermWidget *>(m_tabs.at(i));

			if(w && w == m_currentWidget)
			{
				break;
			}
		}
	}
	else
		i = -1;
	m_tabs.insert(i + 1, wid);
	tabMap.insert(id, wid);

	if(r_index)
		*r_index = i + 1;

	wid->hide();
	//showTab(id);

	return wid;
}

MTermWidget * karin::tab_group::take(qint64 id)
{
#if 0
	int i;
	for(i = 0; i < layout->count(); i++)
		if(static_cast<MTermWidget *>(layout->itemAt(i))->mwId == id)
			break;
	if(i >= 0 && i < layout->count())
	{
		MTermWidget *wid = static_cast<MTermWidget *>(layout->itemAt(i));
		wid->clearFocus();
		wid->hide();
		layout->removeAt(i);
		if(wid->scene())
			wid->scene()->removeItem(wid);
		wid->setParent(0);
		wid->setParentItem(0);
		m_tabs.removeOne(wid);
		return wid;
	}
	return 0;
#endif
	MTermWidget *wid = 0;
	int i;

	if(tabMap.contains(id))
	{
		wid = tabMap.take(id);
		m_tabs.removeOne(wid);
		if(wid)
		{
			wid->clearFocus();
			wid->hide();
			for(i = 0; i < layout->count(); i++)
			{
				if(static_cast<MTermWidget *>(layout->itemAt(i))->mwId == id)
				{
					layout->removeAt(i);
					break;
				}
			}
			if(wid->scene())
				wid->scene()->removeItem(wid);
			wid->setParent(0);
			wid->setParentItem(0);
		}
	}
	return wid;
}

void karin::tab_group::removeTab(qint64 id)
{
	MTermWidget *wid = take(id);
	if(wid)
	{
		wid->disconnect();
		delete wid;
	}
}

void karin::tab_group::showTab(qint64 id)
{
	if(id >= 0 && current != id)
	{
		if(m_currentWidget)
		{
			m_currentWidget->clearFocus();
			m_currentWidget->hide();
		}
		current = id;
		m_currentWidget = getCurrentWidget();
		m_currentIndex = getCurrentIndex();
		if(m_currentWidget)
		{
			setFocusProxy(m_currentWidget);
			m_currentWidget->show();
			//centralWidget() -> setFocusProxy(currentWidget());
		}
	}
}

int karin::tab_group::getCurrentIndex() const
{
	for(int i = 0; i < m_tabs.size(); i++)
	{
		if(m_tabs.at(i) == m_currentWidget)
		{
			return i;
		}
	}
	return -1;
}

MTermWidget * karin::tab_group::getCurrentWidget()
{
	return operator[](current);
}

MTermWidget * karin::tab_group::operator[] (qint64 id)
{
#if 0
	MTermWidget *wid = 0;
	for(int i = 0; i < layout->count(); i++)
	{
		wid = static_cast<MTermWidget *>(layout->itemAt(i));
		if(wid->mwId == id)
			break;
	}
	return wid;
#endif
	if(tabMap.contains(id))
	{
		return tabMap[id];
	}
	return 0;
}

QVector<MTermWidget *> karin::tab_group::getAllTabs()
{
	QVector<MTermWidget *> vec;
	for(int i = 0; i < layout->count(); i++)
		vec.push_back(static_cast<MTermWidget *>(layout->itemAt(i)));
	return vec;
}

qint64 karin::tab_group::nextTab(qint64 widget)
{
	const MTermWidget *wid;
	if(widget < 0)
		widget = current;
	if(m_tabs.size() > 1)
	{
		if(m_currentIndex == m_tabs.size() - 1)
		{
			wid = m_tabs.at(m_currentIndex - 1);
		}
		else
		{
			wid = m_tabs.at(m_currentIndex + 1);
		}
		return getWidgetId(wid);
	}
	else
		return -1;
}

qint64 karin::tab_group::getWidgetId(const MTermWidget *wid)
{
	if(!wid)
		return -1;
	for(QHash<qint64, MTermWidget *>::const_iterator itor = tabMap.cbegin();
			itor != tabMap.cend();
			++itor)
	{
		if(itor.value() == wid)
			return itor.key();
	}
	return -1;
}
