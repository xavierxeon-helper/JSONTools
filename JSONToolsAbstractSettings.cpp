#include <JSONToolsAbstractSettings.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

JSONTools::AbstractSettings::AbstractSettings(const QString& groupName)
   : groupName(groupName)
{
}

void JSONTools::AbstractSettings::write(const QString& key, const QVariant& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue::fromVariant(value));
}

void JSONTools::AbstractSettings::write(const QString& key, const QVariantList& value, bool init)
{
   if (init && hasValue(key))
      return;

   QJsonArray array;
   for (const QVariant& content : value)
      array.append(QJsonValue::fromVariant(content));

   setValue(key, array);
}

void JSONTools::AbstractSettings::write(const QString& key, const QString& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void JSONTools::AbstractSettings::write(const QString& key, const QStringList& value, bool init)
{
   if (init && hasValue(key))
      return;

   QJsonArray array;
   for (const QString& content : value)
      array.append(content);

   setValue(key, array);
}

void JSONTools::AbstractSettings::write(const QString& key, const QList<int>& value, bool init)
{
   if (init && hasValue(key))
      return;

   QJsonArray array;
   for (const int& content : value)
      array.append(content);

   setValue(key, array);
}

void JSONTools::AbstractSettings::write(const QString& key, const QByteArray& value, bool init)
{
   if (init && hasValue(key))
      return;

   QByteArray asciiValue = value.toBase64();
   QJsonValue jv(QString::fromUtf8(asciiValue));
   setValue(key, jv);
}

void JSONTools::AbstractSettings::write(const QString& key, const int& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void JSONTools::AbstractSettings::write(const QString& key, const double& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void JSONTools::AbstractSettings::write(const QString& key, bool value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void JSONTools::AbstractSettings::write(const QString& key, const QJsonObject& object, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, object);
}

QVariant JSONTools::AbstractSettings::variant(const QString& key, const QVariant defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toVariant();
}

QVariantList JSONTools::AbstractSettings::variantList(const QString& key) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return QVariantList();

   QVariantList list;
   for (const QJsonValue& value : value.toArray())
      list.append(value.toVariant());

   return list;
}

QString JSONTools::AbstractSettings::string(const QString& key, const QString defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toString();
}

QStringList JSONTools::AbstractSettings::stringList(const QString& key) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return QStringList();

   QStringList list;
   for (const QJsonValue& value : value.toArray())
      list.append(value.toString());

   return list;
}

QList<int> JSONTools::AbstractSettings::intList(const QString& key) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return QList<int>();

   QList<int> list;
   for (const QJsonValue& value : value.toArray())
      list.append(value.toInt());

   return list;
}

QByteArray JSONTools::AbstractSettings::bytes(const QString& key, const QByteArray defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   QByteArray asciiValue = value.toString().toUtf8();
   return QByteArray::fromBase64(asciiValue);
}

int JSONTools::AbstractSettings::integer(const QString& key, const int defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toInt();
}

double JSONTools::AbstractSettings::real(const QString& key, const double defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toDouble();
}

bool JSONTools::AbstractSettings::boolean(const QString& key, const bool defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toBool();
}

QJsonObject JSONTools::AbstractSettings::object(const QString& key, const QJsonObject& defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toObject();
}

bool JSONTools::AbstractSettings::hasValue(const QString& key) const
{
   if (groupName.isEmpty())
      return content().contains(key);

   if (content()[groupName].isNull() || !content()[groupName].isObject())
      return false;

   QJsonObject groupObject = content()[groupName].toObject();
   return groupObject.contains(key);
}

QJsonValue JSONTools::AbstractSettings::getValue(const QString& key) const
{
   if (groupName.isEmpty())
      return content()[key];

   if (content()[groupName].isNull() || !content()[groupName].isObject())
      return QJsonValue();

   QJsonObject groupObject = content()[groupName].toObject();
   return groupObject[key];
}

void JSONTools::AbstractSettings::setValue(const QString& key, const QJsonValue& value)
{
   if (groupName.isEmpty())
   {
      contentRef()[key] = value;
      return;
   }

   if (content()[groupName].isNull())
      contentRef()[groupName] = QJsonObject();

   QJsonObject groupObject = content()[groupName].toObject();
   groupObject[key] = value;

   contentRef()[groupName] = groupObject; // write object back, since it is not a ref!
}
