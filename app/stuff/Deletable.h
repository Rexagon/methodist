#ifndef DELETABLE_H
#define DELETABLE_H

#include <memory>

typedef std::shared_ptr<bool> DeletionMark;

class Deletable
{
public:    
    Deletable();
    virtual ~Deletable();
    
    DeletionMark getDeletionMark();
    
private:
    DeletionMark m_isDeleted;
};

#endif // DELETABLE_H
