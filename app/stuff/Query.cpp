#include "Query.h"

#include <QDebug>

QString Query::escape(const QString& string)
{
    QString result = string;
    result.replace(QLatin1Char('\''), QLatin1String("''"));
    
    return result;
}

QString Query::create(const QString& sql, const std::vector<QString>& args)
{
    QString result = sql;
    
    size_t currentArg = 0;
    
    for (int i = 0; i < result.size() - 1 && currentArg < args.size(); ++i) {
        if (result[i] == '@' && result[i + 1] == '@') {
            result.remove(i, 2);
            
            QString arg = escape(args[currentArg++]);
            result.insert(i, arg);
            i += arg.size() - 1;
        }
    }
    
    return result;
}
