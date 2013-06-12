#include "expression.h"
#include "operation.h"

#include <QDomElement>

Expression::Expression(Tag* parent)
{
    this->children_.clear();
    this->parent_ = parent;
    this->domNode_ = new QDomElement();
    this->data_.append(QString("New Expression"));
    this->data_.append(0);
    this->implicity_ = false;
}

Expression::~Expression() {
    this->children_.clear();
}

QSet<QString> Expression::allFiles()
{
    return this->op_->execute(this->first_,this->second_);
}

void Expression::setFirst(Tag *firstTag)
{
    this->first_ = firstTag;
}

Tag *Expression::first()
{
    return this->first_;
}

Tag *Expression::second()
{
    return this->second_;
}

bool Expression::isExpression(){
    return true;
}

bool Expression::isImplicit(){
    return this->implicity_;
}

void Expression::setImplicity(bool implicity){
    this->implicity_ = implicity;
}

void Expression::setSecond(Tag *secondTag)
{
    this->second_ = secondTag;
}

void Expression::setTags(Tag *first, Tag *second)
{
    setFirst(first);
    setSecond(second);
}

QChar Expression::operation()
{
    return this->op_->name();
}

void Expression::setOperation(Operation* op)
{
    this->op_ = op;
}


