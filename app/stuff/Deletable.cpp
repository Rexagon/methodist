#include "Deletable.h"

Deletable::Deletable()
{
    m_isDeleted = std::make_shared<bool>(false);
}

Deletable::~Deletable()
{
    *m_isDeleted = true;
}

DeletionMark Deletable::getDeletionMark()
{
    return m_isDeleted;
}
