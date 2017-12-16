#ifndef COURSENODE_H
#define COURSENODE_H

#include <vector>
#include <memory>

#include <QModelIndex>

typedef std::shared_ptr<bool> DeletionMark;

class CourseNode
{
public:
    enum class Type {
        COURSE,
        SECTION,
        TASK,
        TEST
    };
    
    virtual ~CourseNode();
    
    Type getType() const;
    
    int getRelativeNumber() const;
    
    CourseNode* getParent() const;
    
    CourseNode* getChild(size_t n) const;
    int getChildIndex(const CourseNode* child) const;
    
    size_t getChildCount() const;
    std::vector<CourseNode*> getChildren() const;
    
    DeletionMark getDeletionMark() const;
    
protected:
    void addChild(CourseNode* child);
    void removeChild(const CourseNode* child);
    
    CourseNode(Type type);
    
    CourseNode* m_parent;
    std::vector<CourseNode*> m_children;
    
private:
    void setParent(CourseNode* parent);
    
    Type m_type;
    
    DeletionMark m_isDeleted;
};

#endif // COURSENODE_H
