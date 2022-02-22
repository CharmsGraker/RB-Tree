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
    //x��right����x��λ�ã�x��Ϊ��ԭ��left��λ��,x���Һ������Ϊԭ��right(y)�����ӣ�x��Ϊy(��right)������
    RBTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != nil)
        y->left->parent = x;//�����������������ӵ�x����֮����
    y->parent = x->parent;
    if (x->parent == nil) {
        root = y;
    }//y����x����������parent��
    else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RBTree::right_rotate(RBTreeNode *x) {
    //y(x��left)����x��λ�ã�x��Ϊ��ԭ��right(x->right)��λ��,x���������Ϊԭ��left(y)���Һ��ӣ�x��Ϊy(��left)���Һ���
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
    //�Ƚ�z��Ϊ��ͨ�ڵ㣬��AVL����������
    RBTreeNode *y = nil;
    //x������ǰ·���ϵĽڵ�
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
    //x��Ҷ���ˣ�z���뵽y�ĺ���
    if (y == nil)
        root = z;//����
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
     * �����z��һ����ڵ�
     * ���z�ĸ����Ǻ�ɫ�ڵ㣬����ֱ�Ӳ��롣�����ڵ㲻������
     * ���Ҫ����while,��ôz.p�Ǻ�ɫ��֪z.pһ�����Ǹ��ڵ㣬��ôz.p.pһ��������z.p.p.color=BLACK,��Ȼz.pΪz.p.p�ĺ��ӣ�
     * ��ôz.p.pһ������Ҷ�ڵ㡣����ȴ�Ǻ�ɫ��˵��z.p.p�����ӣ������ò���Ⱦ�ɺ�ɫ����ˣ�z����ڵ�һ������
    */
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            //cout<<"���ڵ�������"<<endl;
            RBTreeNode *y = z->parent->parent->right;
            if (y->color == RED) {
                //cout<<"case a1:"<<endl;
                //case a1: z����ڵ�y�Ǻ�ɫ��
                //��z.p���ֵ�Ϊ���֪��z.p.pΪ�ڣ���ô�������ɫת�Ƶ�������ĺ��ӣ�Ҳ���Լ�������bhһ��
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                continue;
                //��Ҫ���ݵ���
            } else if (z == z->parent->right) {
                //if(y->color==BLACK)
                //case a2����ת��Ϊcase 3
                //cout<<"case a2:";
                z = z->parent;
                left_rotate(z);
            }
            //case 3
            //�����ڵ�z.p.p��������
            z->parent->color = BLACK;//��������������һ�㡣���뻭ͼ���
            z->parent->parent->color = RED;
            right_rotate(z->parent->parent);

            //������ݣ���Ϊ������ֹ���
        } else {
            //z->parent==z->parent->parent->right
            //cout<<"���ڵ����Һ���"<<endl;
            RBTreeNode *y = z->parent->parent->left;
            if (y->color == RED) {
                //cout<<"case b1:"<<endl;
                //case b1: z����ڵ�y�Ǻ�ɫ��
                //��z.p���ֵ�Ϊ���֪��z.p.pΪ�ڣ���ô�������ɫת�Ƶ�������ĺ��ӣ�Ҳ���Լ�������bhһ��
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
                continue;
                //��Ҫ���ݵ���
            }
                //if(y->color==BLACK)
                //case b2����ת��Ϊcase 3
            else if (z == z->parent->left) {
                //cout<<"case b2:";
                z = z->parent;
                right_rotate(z);
            }
            //case b3
            //�����ڵ�z.p.p��������
            z->parent->color = BLACK;//��������������һ�㡣���뻭ͼ���
            z->parent->parent->color = RED;
            left_rotate(z->parent->parent);
        }
    }
    root->color = BLACK;
    //��������½ڵ㣬�����˳����1������
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
    //y��ָ��������Ǹ��ڵ�,xָ�����y���Ǹ��ڵ�
    RBTreeNode *y = z;
    RBTreeNode *x = nil;
    rb_tree_color y_oriColor = y->color;
    if (z->left == nil) {
        //��z������������
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    } else {
        //z��������
        y = minimumNode(z->right);
        y_oriColor = y->color;
        x = y->right;
        if (y->parent == z) {
            //��ôy��Ϊz��ֱϵ�Һ��ӣ���һ��û�����ӡ�����Ҫ�����棬ֱ�ӽ�y��ͬ����������z
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            //��z->right�������ӵ�y
            y->right->parent = y;
        }
        //��y����z
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    //�ǳ�һ����˼�룬��y��BLACK�������Ƹ�x������xΪ��ڻ�˫�غ�ɫ�ڵ㡣��취�����ַǺڷǺ����ɫ˦��ȥ
    if (y_oriColor == BLACK)
        RBTree_DELETE_FIXUP(x); /* now, node x is double BLACK or RED BLACK */
}

void RBTree::RBTree_DELETE_FIXUP(RBTreeNode *x) {
    //����x��ԭ�����Ǻ�ɫ����ô����ʼ���μ�x.color==RED����x�Ǹ���ڽڵ㣬x.color==BLACK����x�Ǹ�˫�غ�ɫ�ڵ�
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBTreeNode *w = x->parent->right;
            if (w->color == RED) {
                cout << "case 1:";
                //�ⰵ����w���������ӣ��Ҷ�ΪBLACK
                //��ô���ǿ��ԶԸ�С�ķ�Χ���е���
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }//w.color==BLACK
            if (w->left->color == BLACK && w->right->color == BLACK) {
                cout << "case 2";
                //��ôw����Ⱦ��RED,���൱�ڸ�x��wͬʱ��һ���ɫ��̫������
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    cout << "case 3:";
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    //�������Ǹ�nilҲ��������
                    w = x->parent->right;
                }

                cout << "case 4:";
                //��ʱx->parent����ɫ����w->left����ɫ����ν
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

                //�ⰵ����w���������ӣ��Ҷ�ΪBLACK
                //��ô���ǿ��ԶԸ�С�ķ�Χ���е���
                x->parent->color = RED;
                w->color = BLACK;
                right_rotate(x->parent);
                w = x->parent->left;
            }//w.color==BLACK
            if (w->left->color == BLACK && w->right->color == BLACK) {
                cout << "case 2";

                //��ôw����Ⱦ��RED,���൱�ڸ�x��wͬʱ��һ���ɫ��̫������
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    cout << "case 3:";
                    w->color = RED;
                    w->right->color = BLACK;
                    left_rotate(w);

                    //�������Ǹ�nilҲ��������
                    w = x->parent->left;
                }

                cout << "case 4:";

                //��ʱx->parent����ɫ����w->left����ɫ����ν
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