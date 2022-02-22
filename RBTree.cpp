#include <iostream>
#include <iomanip>
#include "RBTree.h"

using namespace std;

void RBTree::display() {
    cout << "\nTraversal Order:" << endl;
    display(root);
}

RBTree::RBTree() {
    init_nil();
    root = nil;
}

RBTreeNode::RBTreeNode(RBTreeNode *nil, Type val) : key(val), color(RED), left(nil), right(nil), parent(nil) {}

inline void RBTree::display(RBTreeNode *p) {
    if (p == nil) return;
    display(p->left);
    cout << p->key << setw(5);
    display(p->right);
}

RBTreeNode *RBTree::search(RBTreeNode *p, Type val) {
    RBTreeNode *z = nil;
    while (p != nil) {
        if (p->key < val) {
            p = p->right;
        } else if (p->key == val) {
            return p;
        } else p = p->left;
    }
    return z;
}

void RBTree::remove(Type val) {
    RBTreeNode *z = search(root, val);
    if (z != nil) remove(z);
    else cout << "\nThe " << val << " is not in RedBlackTree.";
};


void RBTree::left_rotate(RBTreeNode *x) {
    //x的right顶替x的位置，x变为其原来left的位置,x的右孩子域变为原先right(y)的左孩子，x成为y(其right)的左孩子
    RBTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != nil)
        y->left->parent = x;//设置左孩子域，现在它接到x孩子之下了
    y->parent = x->parent;
    if (x->parent == nil) {
        root = y;
    }//y顶替x后，重新链接parent域
    else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RBTree::right_rotate(RBTreeNode *x) {
    //y(x的left)顶替x的位置，x变为其原来right(x->right)的位置,x的左孩子域变为原先left(y)的右孩子，x成为y(其left)的右孩子
    RBTreeNode *y = x->left;
    x->left = y->right;
    if (y->right != nil)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

//insert
void RBTree::insert(RBTreeNode *z) {
    //先将z视为普通节点，像AVL树那样插入
    RBTreeNode *y = nil;
    //x表明当前路径上的节点
    RBTreeNode *x = root;
    while (x != nil) {
        y = x;
        if (x->key > z->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    //x到叶子了，z插入到y的孩子
    if (y == nil)
        root = z;//树空
    else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left = nil;
    z->right = nil;
    z->color = RED;
    //cout<<z->key;
    //cout<<"zparent"<<z->parent->key<<endl;
    insertFixUp(z);
    cout << z->key << " has inserted.\n";
}

void RBTree::insert(int val) {
    RBTreeNode *z = new RBTreeNode(nil, val);
    insert(z);
}

//INSERT_FIXUP
void RBTree::insertFixUp(RBTreeNode *z) {
    /*
     * 待插的z是一个红节点
     * 如果z的父亲是黑色节点，可以直接插入。但红红节点不能相邻
     * 如果要进入while,那么z.p是红色，知z.p一定不是根节点，那么z.p.p一定存在且z.p.p.color=BLACK,既然z.p为z.p.p的孩子，
     * 那么z.p.p一定不是叶节点。但他却是黑色，说明z.p.p满孩子，而不得不被染成黑色。因此，z的叔节点一定存在
    */
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            //cout<<"父节点是左孩子"<<endl;
            RBTreeNode *y = z->parent->parent->right;
            if (y->color == RED) {
                //cout<<"case a1:"<<endl;
                //case a1: z的叔节点y是红色的
                //从z.p的兄弟为红得知，z.p.p为黑，那么将这个黑色转移到其两侧的孩子，也可以继续保持bh一致
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                continue;
                //需要回溯调整
            } else if (z == z->parent->right) {
                //if(y->color==BLACK)
                //case a2，可转化为case 3
                //cout<<"case a2:";
                z = z->parent;
                left_rotate(z);
            }
            //case 3
            //对现在的z.p.p进行右旋
            z->parent->color = BLACK;//右旋后他被提升一层。必须画图理解
            z->parent->parent->color = RED;
            right_rotate(z->parent->parent);

            //无需回溯，这为调整终止情况
        } else {
            //z->parent==z->parent->parent->right
            //cout<<"父节点是右孩子"<<endl;
            RBTreeNode *y = z->parent->parent->left;
            if (y->color == RED) {
                //cout<<"case b1:"<<endl;
                //case b1: z的叔节点y是红色的
                //从z.p的兄弟为红得知，z.p.p为黑，那么将这个黑色转移到其两侧的孩子，也可以继续保持bh一致
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                continue;
                //需要回溯调整
            }
                //if(y->color==BLACK)
                //case b2，可转化为case 3
            else if (z == z->parent->left) {
                //cout<<"case b2:";
                z = z->parent;
                right_rotate(z);
            }
            //case b3
            //对现在的z.p.p进行右旋
            z->parent->color = BLACK;//右旋后他被提升一层。必须画图理解
            z->parent->parent->color = RED;
            left_rotate(z->parent->parent);
        }
    }
    root->color = BLACK;
    //处理插入新节点，或者退出情况1的情形
}

RBTreeNode *RBTree::minimumNode(RBTreeNode *z) {
    while (z->left != nil) {
        z = z->left;
    }
    return z;
}

void
RBTree::transplant(RBTreeNode *u, RBTreeNode *v) {
    /** let v be the son of u->parent, and unlink u */
    if (u->parent == nil)
        root = v;
    if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void RBTree::remove(RBTreeNode *z) {
    //y总指向被替代的那个节点,x指向替代y的那个节点
    RBTreeNode *y = z;
    RBTreeNode *x = nil;
    rb_tree_color y_oriColor = y->color;
    if (z->left == nil) {
        //用z的右子树顶替
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        //z有满孩子
        y = minimumNode(z->right);
        y_oriColor = y->color;
        x = y->right;
        if (y->parent == z) {
            //那么y就为z的直系右孩子，且一定没有左孩子。不需要被顶替，直接将y连同其子树顶替z
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            //将z->right父亲链接到y
            y->right->parent = y;
        }
        //用y顶替z
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    //非常一绝的思想，将y的BLACK属性下推给x，现在x为红黑或双重黑色节点。想办法将这种非黑非红的颜色甩出去
    if (y_oriColor == BLACK)
        RBTree_DELETE_FIXUP(x); /* now, node x is double BLACK or RED BLACK */
}

void RBTree::RBTree_DELETE_FIXUP(RBTreeNode *x) {
    //由于x的原父亲是黑色，那么现在始终牢记x.color==RED表明x是个红黑节点，x.color==BLACK表明x是个双重黑色节点
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBTreeNode *w = x->parent->right;
            if (w->color == RED) {
                cout << "case 1:";
                //这暗含了w有两个孩子，且都为BLACK
                //那么我们可以对更小的范围进行调整
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }//w.color==BLACK
            if (w->left->color == BLACK && w->right->color == BLACK) {
                cout << "case 2";
                //那么w可以染成RED,这相当于给x和w同时减一层黑色。太巧妙了
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    cout << "case 3:";
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    //即便这是个nil也可以设置
                    w = x->parent->right;
                }

                cout << "case 4:";
                //此时x->parent的颜色，和w->left的颜色无所谓
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(x->parent);
                x = root;
            }
        } else {
            //x==x->parent->right
            RBTreeNode *w = x->parent->left;
            if (w->color == RED) {
                cout << "case 1:";

                //这暗含了w有两个孩子，且都为BLACK
                //那么我们可以对更小的范围进行调整
                x->parent->color = RED;
                w->color = BLACK;
                right_rotate(x->parent);
                w = x->parent->left;
            }//w.color==BLACK
            if (w->left->color == BLACK && w->right->color == BLACK) {
                cout << "case 2";

                //那么w可以染成RED,这相当于给x和w同时减一层黑色。太巧妙了
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    cout << "case 3:";
                    w->color = RED;
                    w->right->color = BLACK;
                    left_rotate(w);

                    //即便这是个nil也可以设置
                    w = x->parent->left;
                }

                cout << "case 4:";

                //此时x->parent的颜色，和w->left的颜色无所谓
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}