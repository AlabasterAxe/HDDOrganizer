#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "tag.h"

class Operation;

class Expression : public Tag
{
public:
    Expression(Tag* parent);
    ~Expression();
    QSet<QString> allFiles();
    void setFirst(Tag* firstTag);
    Tag* first();
    void setSecond(Tag* secondTag);
    Tag* second();
    void setTags(Tag* first, Tag* second);
    QChar operation();
    void setOperation(Operation *op);

    bool isExpression();
    bool isImplicit();
    void setImplicity(bool implicity);
private:
    Tag* first_;
    Tag* second_;
    Operation* op_;
    bool implicity_;
};

#endif // EXPRESSION_H
