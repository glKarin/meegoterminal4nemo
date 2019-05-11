#include "toolbar.h"
#include "key.h"
#include "karin_ut.h"
#include "k.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

#define JSON_HASOWNPROPERTY(T, name) (json->contains(name) && (*json)[name].is##T())
#define JSON_GETPROPERTY(T, name) ((*json)[name].to##T())

namespace karin
{
	static QStringList m_toolbars;

	struct toolkey_s
	{
		toolkey_s()
			: label(""),
			sym(0),
			name(""),
			modifier(0),
			type(karin::base_key::Key_General),
			repeat(false),
			data(QString())
		{
			/*
    name = QFileInfo(fileName).baseName();
    name[0] = name[0].toUpper();
		*/
		}
		QString label;
		unsigned sym;
		QString name;
		unsigned modifier;
		karin::base_key::Key_Type type;
		bool repeat;
		QString data;
	};

	struct toolbar_layout_s
	{
		QString name;
		QString desc;
		QString author;
		QString version;
		QString release;
		QList<toolkey_s> layout;
	};

	static QString read_string(const QJsonObject &json, const QString &name, bool *ok = 0)
	{
		QString r("");
		bool rb;

		rb = false;
		if(!json.contains(name))
		{
			rb = false;
		}
		else
		{
			const QJsonValue obj = json[name];
			if(obj.isString())
			{
				r = obj.toString();
				rb = true;
			}
			else if(obj.isArray())
			{
				QStringList sl;
				QJsonArray arr = obj.toArray();
				if(arr.isEmpty())
					rb = false;
				else
				{
					K_FOREACH_CONST(QJsonArray, arr)
					{
						sl.push_back(itor->toString());
					}
					r = String_ArrayToString(sl);
					rb = true;
				}
			}
			else
			{
				rb = false;
			}
		}
		if(ok)
			*ok = rb;
		return r;
	}

	static int parse_from_JSON(const QJsonObject &j, karin::toolbar_layout_s *ret)
	{
		int r = 0;
		QString s;
		bool ok;
		const QJsonObject *json = &j;

		if(!ret)
			return -1;

		ret->name = read_string(j, "name");
		ret->author = read_string(j, "author");
		ret->desc = read_string(j, "desc");
		ret->version = read_string(j, "version");
		ret->release = read_string(j, "release");
		if(!JSON_HASOWNPROPERTY(Array, "layout"))
			return -2;
		QJsonArray array = JSON_GETPROPERTY(Array, "layout");
		ret->layout.clear();
		K_FOREACH_CONST(QJsonArray, array)
		{
			if(!itor->isObject())
				continue;
			const QJsonObject obj = itor->toObject();
			json = &obj;
			if(!JSON_HASOWNPROPERTY(String, "sym"))
			{
				r++;
				continue;
			}
			toolkey_s key;
			s = JSON_GETPROPERTY(String, "sym");
			key.sym = String_StringToInt(s, &ok);

			if(!ok)
			{
				r++;
				continue;
			}
			s = JSON_GETPROPERTY(String, "modifier");
			key.modifier = String_StringToInt(s, &ok);
			if(!ok)
			{
				//r++;
				key.modifier = 0;
			}
			if(JSON_HASOWNPROPERTY(String, "name"))
				key.name = JSON_GETPROPERTY(String, "name");

			key.label = read_string(obj, "label");

			if(JSON_HASOWNPROPERTY(String, "type"))
			{
				s = JSON_GETPROPERTY(String, "type");
				if(s == "function")
					key.type = karin::base_key::Key_General;
				else if(s == "modifier")
					key.type = karin::base_key::Key_Modifier;
			}
			if(JSON_HASOWNPROPERTY(Bool, "repeat"))
				key.repeat = JSON_GETPROPERTY(Bool, "repeat");
			key.data = read_string(obj, "data");

			ret->layout.push_back(key);
		}

		return r;
	}

	static bool load_from_JSON(const QString &src, karin::toolbar_layout_s *ret)
	{
		if(!ret)
			return false;

		QFile file(src);
		if(!file.open(QIODevice::ReadOnly))
		{
			qWarning() << "[Harmattan]: Open toolbar layout file failed." << src;
			return false;
		}

		QByteArray data = file.readAll();

		QJsonDocument doc(QJsonDocument::fromJson(data));

		int err = parse_from_JSON(doc.object(), ret);

		file.close();

		return(err == 0);
	}

};

bool karin::toolbar::loadLayout(const QString &src)
{
	toolbar_layout_s layout;
	bool b;
	karin::base_key *key;

	if(src.isEmpty())
		return false;

	b = load_from_JSON(src, &layout);
	if(!b)
		return false;
	clear();
	K_FOREACH_CONST(QList<toolkey_s>, layout.layout)
	{
		if(itor->type == karin::base_key::Key_Modifier)
			key = new karin::modifier_key();
		else
			key = new karin::general_key();

		key->setLabel(itor->label);
		key->setName(itor->name);
		key->setSym(itor->sym);
		key->setModifier(itor->modifier);
		if(itor->type == karin::base_key::Key_General)
		{
			karin::general_key *gkey = static_cast<karin::general_key *>(key);
			gkey->setRepeat(itor->repeat);
		}
		addAction(key);
	}
	invalidate();
	update();

	return true;
}

const QStringList & karin::toolbar::getAllToolbars()
{
	return m_toolbars;
}

void karin::toolbar::setActiveToolbarIndex(int index)
{
	if (m_activeToolbarIndex == index)
		return;
	// m_activeToolbarIndex = -1;
	if(index >= 0 && index < karin::toolbar::getAllToolbars().size())
	{
		//Q_ASSERT(index >= 0 && index < Toolbar::getAllToolbars().count());

		const QString file = karin::toolbar::getAllToolbars()[index];

		if(loadLayout(file))
			m_activeToolbarIndex = index;
	}
}

void karin::toolbar::setNextActiveToolbar()
{
	int i = m_activeToolbarIndex;
	++i;
	if (i >= karin::toolbar::getAllToolbars().count()) {
		i = 0;
	}
	setActiveToolbarIndex(i);
}

void karin::toolbar::setPrevActiveToolbar()
{
	int i = m_activeToolbarIndex;
	--i;
	if (i < 0) {
		i = karin::toolbar::getAllToolbars().count() - 1;
	}
	setActiveToolbarIndex(i);
}

int karin::toolbar::setActiveToolbar(QString fileName)
{
	if(fileName.isEmpty())
		return m_activeToolbarIndex;

	fileName = fileName.trimmed();
	const QStringList & tbs = karin::toolbar::getAllToolbars();

	for (int i = 0; i < tbs.size(); ++i) {
		if (tbs.at(i) == fileName) {
			setActiveToolbarIndex(i);
			break;
		}
	}

	return m_activeToolbarIndex;
}

void karin::toolbar::updateToolbarDirectory(const QString &path)
{
	Q_UNUSED(path);

	QStringList toolbars;

	QStringList paths = ToolbarDirectory();
	// toolbars are specified in xml files in this directory
	K_FOREACH_CONST(QStringList, paths)
	{
		QDir dir(*itor);
		dir.setFilter(QDir::Readable | QDir::Files);
		dir.setNameFilters(QStringList() << "*.layout.json");

		foreach(QFileInfo info, dir.entryInfoList()) {
			toolbars.append(info.absoluteFilePath());
		}
	}

	if(m_toolbars != toolbars)
	{
		m_toolbars = toolbars;
		emit toolbarLayoutListChanged(m_toolbars);
	}
}

QStringList karin::toolbar::ToolbarDirectory()
{
	return QStringList() << QString(_KARIN_PREFIX_) + IM_TOOLBARS_DIR << QDir::homePath() + "/" + QString(_KARINCONSOLE_HOME) + IM_TOOLBARS_DIR;
}
