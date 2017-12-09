#ifndef COURSENODE_H
#define COURSENODE_H

#include <vector>

#include <QModelIndex>

class CourseNode
{
public:
    enum class Type {
        COURSE,
        SECTION,
        TASK
    };
    
    virtual ~CourseNode();
    
    Type getType() const;
    
    int getRelativeNumber() const;
    
    CourseNode* getParent() const;
    
    void addChild(CourseNode* child);
    void removeChild(const CourseNode* child);
    CourseNode* getChild(size_t n) const;
    int getChildIndex(const CourseNode* child) const;
    
    size_t getChildCount() const;
    std::vector<CourseNode*> getChildren() const;
    
protected:
    CourseNode(Type type);
    
    CourseNode* m_parent;
    std::vector<CourseNode*> m_children;
    
private:
    void setParent(CourseNode* parent);
    
    Type m_type;
};

#endif // COURSENODE_H
