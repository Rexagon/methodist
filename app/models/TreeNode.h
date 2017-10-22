#ifndef TREENODE_H
#define TREENODE_H

#include <vector>

class TreeNode
{
public:
    enum class Type {
        COURSE,
        SECTION,
        TASK
    };
    
    virtual ~TreeNode();
    
    Type getType() const;
    
    int getRelativeNumber() const;
    
    TreeNode* getParent() const;
    
    void addChild(TreeNode* child);
    void removeChild(const TreeNode* child);
    void removeChild(size_t n);
    TreeNode* getChild(size_t n) const;
    int getChildIndex(const TreeNode* child) const;
    
    size_t getChildCount() const;
    std::vector<TreeNode*> getChildren() const;
    
protected:
    TreeNode(Type type);
    
    TreeNode* m_parent;
    std::vector<TreeNode*> m_children;
    
private:
    void setParent(TreeNode* parent);
    
    Type m_type;
};

#endif // TREENODE_H
