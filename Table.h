#ifndef TABLE_H
#define TABLE_H

#include <QObject>

class Table
{
public:
    enum Field : uint8_t {
        A = 0,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        Min = A,
        Max = H,
        Count
    };

    static QString Name;
    static QString PrimaryKey;
    static QString FieldText[Field::Count];
};

#endif // TABLE_H
