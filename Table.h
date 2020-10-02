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
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q1,
        Q2,
        Q3,
        Q4,
        Q5,
        Q6,
        Min = A,
        Max = Q6,
        Count
    };

    static QString FieldText[Field::Count];
    static QString Name;
    static QString RowNumber;
    static QString PrimaryKey;
};

#endif // TABLE_H
