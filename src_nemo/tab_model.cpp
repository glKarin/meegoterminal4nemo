#include "tab_model.h"
#include "k.h"

#include <QDateTime>

karin::tab_model::tab_model(QObject *parent)
	:QAbstractListModel(parent)
{
	setObjectName(OBJECTNAME_TAB_MODEL);
}

karin::tab_model::~tab_model()
{
}

QVariant karin::tab_model::data(const QModelIndex &index, int role) const
{
	QString data;
	if(role == Qt::DisplayRole)
	{
		int i = index.row();
		if(i >= 0 && i < tabList.size())
		{
			data = tabMap[tabList[i]];
			if(data == "Karin Console")
				data = QString("%1 - %2").arg(data).arg(i);
		}
	}
	return QVariant(data);
}

int karin::tab_model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return tabList.size();
}

void karin::tab_model::insertTab(qint64 id, const QString &title, int index)
{
	if(tabList.contains(id))
		return;
	if(index < 0)
		index = 0;
	else if(index > tabList.size())
		index = tabList.size();
	tabList.insert(index, id);
	tabMap.insert(id, title);
	emit layoutChanged();
}

void karin::tab_model::removeTab(qint64 id)
{
	if(tabList.removeOne(id))
	{
		if(tabMap.remove(id))
			emit layoutChanged();
	}
}

void karin::tab_model::updateTab(qint64 id, const QString &title)
{
	if(!tabList.contains(id))
		return;
	tabMap[id] = title;
	emit layoutChanged();
}

qint64 karin::tab_model::valueTab(int i) const
{
	if(i >= 0 && i < tabList.size())
		return tabList.at(i);
	else
		return -1;
}

qint64 karin::tab_model::genTabId() const
{
	qint64 id;
	
	id = QDateTime::currentMSecsSinceEpoch();
	while(hasTab(id))
		id = QDateTime::currentMSecsSinceEpoch();

	return id;
}

QPair<qint64, QString> karin::tab_model::getTabInfo(int i) const
{
	QPair<qint64, QString> p = qMakePair<qint64, QString>(-1, QString::null);
	if(i >= 0 && i < tabList.size())
	{
		p.first = tabList.at(i);
		p.second = tabMap[p.first];
	}
	return p;
}
