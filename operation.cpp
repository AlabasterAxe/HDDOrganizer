#include "operation.h"

Operation::Operation()
{
}

QChar Operation::name()
{
    return this->name_;
}

void Operation::setName(QChar str)
{
    this->name_ = str;
}

/////////// Difference Operator ////////////

Difference::Difference()
{
}

QSet<QString> Difference::execute(Tag *a, Tag *b)
{
    return a->allFiles().subtract(b->allFiles());
}

///////////// Expansion Operator ////////////

Expansion::Expansion()
{
}

QSet<QString> Expansion::execute(Tag *a, Tag *b)
{
    return b->allFiles();
}

////////////// Intersection Operator ////////

Intersection::Intersection()
{
}

QSet<QString> Intersection::execute(Tag *a, Tag *b)
{
    return a->allFiles().intersect(b->allFiles());
}

///////////// Union Operator //////////////

Union::Union()
{
}

QSet<QString> Union::execute(Tag *a, Tag *b)
{
    return a->allFiles().unite(b->allFiles());
}

//////////// Unary Operator //////////////

Unary::Unary()
{
}

QSet<QString> Unary::execute(Tag* a, Tag* b){
    return a->allFiles();
}


