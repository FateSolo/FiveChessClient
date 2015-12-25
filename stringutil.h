#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QString>
#include <QRegExp>

class StringUtil
{
public:
    StringUtil();
    ~StringUtil();

    static bool stringTest(int min, int max, QString testString);
};

#endif // STRINGUTIL_H
