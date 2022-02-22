//
// Created by Administrator on 2021/3/16.
//

#ifndef REDBLACKTREE_RBTREE_H
#define REDBLACKTREE_RBTREE_H
typedef enum {
    RED = 0, BLACK = 1
} rb_tree_color;
typedef int Type;

struct RBTreeNode {
    Type key;
    RBTreeNode *parent;
    RBTreeNode *left;
    RBTreeNode *right;
    rb_tree_color color;

    RBTreeNode(RBTreeNode* nil,Type val);

    RBTreeNode(Type a, RBTreeNode *p, RBTreeNode *l, RBTreeNode *r, rb_tree_color cl) : key(a), parent(p), left(l), right(r),
                                                                                color(cl) {}

};

class RBTree {
private:
    //nilÍ³Ò»¸ÅÄî
    RBTreeNode *root;

    void RBTree_DELETE_FIXUP(RBTreeNode *x);

    void transplant(RBTreeNode *u, RBTreeNode *v);

    void insertFixUp(RBTreeNode *z);

    void left_rotate(RBTreeNode *x);

    void right_rotate(RBTreeNode *x);

public:
    RBTree();

    void insert(RBTreeNode *z);

    template<typename... Args>
    void insert(Type val, Args... args) {
        insert(val);
        insert(args...);
    };

    void insert(Type val);

    void remove(RBTreeNode *z);

    void remove(Type val);

    RBTreeNode *minimumNode(RBTreeNode *z);

    RBTreeNode *search(RBTreeNode *root, Type val);

    void display(RBTreeNode *p);

    void display();

    ~RBTree() {
        delete nil;
        nil = nullptr;
        delete root;
        root = nullptr;
    };
    // each RB-Tree hold a nil
    RBTreeNode *nil;
private:

    void
    init_nil() {
        nil = new RBTreeNode{-1, nullptr, nullptr, nullptr, BLACK};
    }
};

#endif //REDBLACKTREE_RBTREE_H