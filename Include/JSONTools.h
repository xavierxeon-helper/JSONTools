#ifndef JsonH
#define JsonH

#include <QJsonObject>

namespace JSONTools
{
   struct Helper
   {
      static QJsonObject load(const QString& fileName, QString* error = nullptr);
      static bool save(const QJsonObject& data, const QString& fileName);

      static QStringList printable(const QJsonObject& object);
      static void debugPrint(const QJsonObject& object);
   };
}; // namespace JSONTools

#endif // JsonH
