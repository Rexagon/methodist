#include "Query.h"

QString Query::escapeHTML(const QString& html)
{
    QString result = html;
    return result
            .replace("&", "&#x26;")
            .replace("<", "&#x3C;")
            .replace(">", "&#x3E;")
            .replace("\"", "&#x22;")
            .replace("'", "&#39;")
            .replace("/", "&#x2F;")
            .replace("`", "&#x60;")
            .replace("=", "&#x3D;");
}

QString Query::unescapeHTML(const QString& html)
{
    QString result = html;
    return result
            .replace("&#x26;", "&")
            .replace("&#x3C;", "<")
            .replace("&#x3E;", ">")
            .replace("&#x22;", "\"")
            .replace("&#39;", "'")
            .replace("&#x2F;", "/")
            .replace("&#x60;", "`")
            .replace("&#x3D;", "=");
}
