#include "stringutil.h"

StringUtil::StringUtil()
{

}

StringUtil::~StringUtil()
{

}

bool StringUtil::stringTest(int min, int max, QString testString) {
    QString pattern = QString("^[a-z0-9A-Z]{%1,%2}$").arg(min).arg(max);
    QRegExp rx(pattern);

    return rx.exactMatch(testString);
}
