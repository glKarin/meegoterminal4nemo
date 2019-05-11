#include "key.h"
#include "k.h"

#include <QDebug>

// base key

karin::base_key::base_key(QGraphicsWidget *parent)
	: rect(parent),
	m_label(""),
	m_sym(0),
	m_name(""),
	m_modifier(0),
	m_state(Key_Released),
	m_pressed(false)
{
	setRadius(karin::rect::M_Radius);
	setObjectName(OBJECTNAME_BASE_KEY);
	setNeedToPaint(false);
}

karin::base_key::~base_key()
{
}

void karin::base_key::touchEvent(bool b)
{
	if(b)
		m_pressed = true;
	else
	{
		if(m_pressed)
			m_pressed = false;
	}
}

void karin::base_key::reset()
{
	m_pressed = false;
}

