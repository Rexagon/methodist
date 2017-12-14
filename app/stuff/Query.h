#ifndef QUERY_H
#define QUERY_H

#include <QString>

class Query
{
public:
    static QString escapeHTML(const QString& html);
    static QString unescapeHTML(const QString& html);
};

#endif // QUERY_H
