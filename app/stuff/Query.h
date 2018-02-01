#ifndef QUERY_H
#define QUERY_H

#include <vector>

#include <QString>

class Query
{
public:
    static QString escape(const QString& string);
    
    static QString create(const QString& sql, const std::vector<QString>& args);
};

#endif // QUERY_H
