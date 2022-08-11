#include <JSONTools.h>

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>

QJsonObject JSONTools::Helper::load(const QString& fileName, QString* error)
{
   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly))
   {
      if (error)
         *error = "Unable to open file";
      return QJsonObject();
   }

   const QByteArray data = file.readAll();
   file.close();

   return convert(data, error);
}

bool JSONTools::Helper::save(const QJsonObject& data, const QString& fileName)
{
   if (data.isEmpty())
      return false;

   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly))
      return false;

   const QByteArray content = QJsonDocument(data).toJson();
   file.write(content);
   file.close();

   return true;
}

QJsonObject JSONTools::Helper::convert(const QByteArray& data, QString* error)
{
   QJsonParseError parseError;
   QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
   if (QJsonParseError::NoError != parseError.error)
   {
      if (error)
         *error = parseError.errorString();
      return QJsonObject();
   }

   return doc.object();
}

QByteArray JSONTools::Helper::convert(const QJsonObject& object)
{
   const QByteArray data = QJsonDocument(object).toJson(QJsonDocument::Compact);
   return data;
}

QStringList JSONTools::Helper::printable(const QJsonObject& object)
{
   QJsonDocument doc(object);
   const QString content = QString::fromUtf8(doc.toJson());

   return content.split("\n", Qt::SkipEmptyParts);
}

void JSONTools::Helper::debugPrint(const QJsonObject& object)
{
   for (const QString& line : printable(object))
   {
      qDebug() << line;
   }
}
