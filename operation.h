#ifndef OPERATION_H
#define OPERATION_H

#include <QSet>
#include <QString>

#include "tag.h"

class Operation
{
public:
    Operation();
    virtual QSet<QString> execute(Tag* a, Tag* b) = 0;
    QChar name();
    void setName(QChar str);

private:
    QChar name_;
};

////////// Difference Operator ////////////

class Difference : public Operation
{
public:
    Difference();
    QSet<QString> execute(Tag* a, Tag* b);
};

////////// Expansion Operator //////////

class Expansion : public Operation
{
public:
    Expansion();
    QSet<QString> execute(Tag* a, Tag* b);
};

/////////// Intersection Operator ////////

class Intersection : public Operation
{
public:
    Intersection();
    QSet<QString> execute(Tag* a, Tag* b);
};

////////// Union Operator /////////

class Union : public Operation
{
public:
    Union();
    QSet<QString> execute(Tag* a, Tag* b);
};

//////////// Unary Operator ////////

class Unary : public Operation
{
public:
    Unary();
    QSet<QString> execute(Tag* a, Tag* b);
};
#endif // OPERATION_H

