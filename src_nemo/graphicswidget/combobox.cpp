#include "combobox.h"
#include "k.h"
#include "label.h"
#include "menu_item_group.h"

#include <QGraphicsAnchorLayout>
#include <QDebug>

karin::combobox::combobox(QGraphicsWidget *parent)
	: karin::abstract_menu_item(parent),
	m_title(new karin::label(this)),
	m_buttonGroup(new karin::menu_item_group(this))
{
	setObjectName(OBJECTNAME_COMBOBOX);
	setNeedToPaint(false);
	setFixedHeight(120);

	QFont f = font();
	f.setPixelSize(24);
	m_title->setFont(f);
	f.setPixelSize(18);
	m_buttonGroup->setFont(f);
	m_title->setBgColor(QColor::fromRgbF(0.9, 0.9, 0.9));

	QGraphicsAnchorLayout *m_layout = new QGraphicsAnchorLayout(this);
	m_title->setFixedHeight(48);
	m_title->setZValue(10);

	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);

	m_layout->addAnchor(m_title, Qt::AnchorTop, m_layout, Qt::AnchorTop);
	m_layout->addAnchor(m_title, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
	m_layout->addAnchor(m_title, Qt::AnchorRight, m_layout, Qt::AnchorRight);

	m_layout->addAnchor(m_buttonGroup, Qt::AnchorLeft, m_layout, Qt::AnchorLeft);
	m_layout->addAnchor(m_buttonGroup, Qt::AnchorRight, m_layout, Qt::AnchorRight);
	m_layout->addAnchor(m_buttonGroup, Qt::AnchorTop, m_title, Qt::AnchorBottom);
	m_layout->addAnchor(m_buttonGroup, Qt::AnchorBottom, m_layout, Qt::AnchorBottom);

	setLayout(m_layout);

	connect(m_buttonGroup, SIGNAL(triggered(const QString &, const QVariant &)), this, SIGNAL(triggered(const QString &, const QVariant &)));
	connect(m_buttonGroup, SIGNAL(checkedChanged()), this, SLOT(checkedChangedSlot()));

}

karin::combobox::~combobox()
{
}

QString karin::combobox::title() const
{
	return m_title->text();
}

int karin::combobox::count() const
{
	return m_buttonGroup->count();
}

bool karin::combobox::exclusive() const
{
	return m_buttonGroup->exclusive();
}

void karin::combobox::setExclusive(bool b)
{
	m_buttonGroup->setExclusive(b);
}

void karin::combobox::addItem(const QString &label, const QString &name, const QVariant &value, int i)
{
	m_buttonGroup->addAction(label, name, value, i);
}

QList<int> karin::combobox::currentIndex() const
{
	return m_buttonGroup->checkedIndexs();
}

void karin::combobox::setCurrentIndex(const QList<int> &indexs)
{
	m_buttonGroup->setCheckedIndexs(indexs);
}

void karin::combobox::setTitle(const QString &title)
{
	m_title->setText(title);
}

void karin::combobox::clear()
{
	m_buttonGroup->clear();
}

void karin::combobox::checkedChangedSlot()
{
	emit currentIndexChanged(m_buttonGroup->checkedIndexs());
}

void karin::combobox::removeItem(int index)
{
	m_buttonGroup->removeAction(index);
}

void karin::combobox::updateItem(int index, const QString &text)
{
	m_buttonGroup->updateAction(index, text);
}
