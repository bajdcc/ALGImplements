#pragma once

#include <stack>
#include <vector>
#include "btree.h"

using namespace std;

template<class T>
struct AVLNode
{
	T data;
	AVLNode *lchild, *rchild;
	int BF; // 平衡因子
};

template<class T, class N = AVLNode<T> >
class AVLTree : public BiTree<T, N>
{
public:
	AVLTree();
	AVLTree(const vector<T>& s);

	bool Insert(T data);
	bool Delete(T data);

	bool Find(T data);

protected:
	virtual N* New();

	N* FindNode(T data);
};

template<class T, class N>
AVLTree<T, N>::AVLTree()
{

}

template<class T, class N>
AVLTree<T, N>::AVLTree(const vector<T>& s)
{
	if (s.empty()) throw "数组不能为空";
	root = new N;
	root->data = s[0];
	root->BF = 0;
	root->lchild = NULL;
	root->rchild = NULL;
	for (int i = 1; i < s.Length(); i++) Insert(s[i]);
}

template<class T, class N>
N* AVLTree<T, N>::New()
{
	N* newp = BiTree<T, N>::New();
	newp->BF = 0;
	return newp;
}

template<class T, class N>
bool AVLTree<T, N>::Insert(T data)
{
	N* newp = New();
	newp->data = data;

	if (root == NULL)
	{
		root = newp;
		return true;
	}

	stack<N*> path; // 存储插入前的查找路径（便于回溯）

	//////////////////////////////////////////////////////////////////////////
	// 插入操作
	N *p = root;
	while (true)
	{
		path.push(p);
		if (newp->data < p->data) // 插入值小于根结点，入左子树
		{
			if (p->lchild != NULL)
			{
				p = p->lchild; // 值小于LL，则递归入L
			}
			else
			{
				p->lchild = newp; break; // 根结点无左孩子，正好插入
			}
		}
		else if (newp->data > p->data) // 插入值大于根结点，入右子树
		{
			if (p->rchild != NULL)
			{
				p = p->rchild; // 值大于RR，则递归入R
			}
			else
			{
				p->rchild = newp; break; // 根结点无右孩子，正好插入
			}
		}
		else // 插入值等于根结点，返回
		{
			delete newp; return false;
		}
	}

	// 插入完毕

	//////////////////////////////////////////////////////////////////////////

	// 调整插入路径上结点的BF，定位失衡的结点*p及其父结点*parent

	N *child = NULL;  // *child作为*p的孩子结点
	p = newp;
	N *parent = path.top(); // *parent是*p的父结点
	path.pop();
	while (true)
	{
		if (parent->lchild == p) // p是parent的左孩子，那么parent的BF++
			parent->BF++;  // BF的变化：-1->0->1->2
		else // p是parent的右孩子，那么parent的BF--
			parent->BF--;  // BF的变化：1->0->-1->-2

		if (parent->BF == 2 || parent->BF == -2) // *parent是失衡结点（第一个|BF|>=2）
			break; // 找到最小不平衡子树的根结点

		if (parent->BF == 0) // 在插入新结点后，*parent的左右子树高度相等（BF为0），说明以*parent为根的子树高度未增加
			return true;       // 所以路径中的其余祖先结点无需调整BF

		if (path.empty()) // 直到树的根结点，在path中没有结点|BF|超出2，所以不必调整（导致有BF为+1,-1等下次插入再行调整）
			return true;

		child = p; p = parent; parent = path.top(); // 由path向上回溯
		path.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	// *parent失衡，以下代码进行调整
	N *ancestor = path.empty() ? NULL : path.top(); // ancestor为parent的双亲结点
	if (!path.empty()) path.pop();

	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == 2 && p->lchild == child)    // LL
	{
		// 前
		//                                      A(parent,root),BF(2)
		//                                      |
		//                  B(p),BF(1)__________|_____AR
		//                  |
		//        BL(X)_____|_____BR

		// 后
		//                  B(p,root),BF(0)
		//                  |                 
		//        BL(X)_____|___________________A(parent),BF(0)
		//                                      |
		//                               BR_____|_____AR

		parent->lchild = p->rchild;
		p->rchild = parent;

		parent->BF = 0;
		p->BF = 0;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // 修改p的双亲为ancestor
				ancestor->lchild = p;
			else
				ancestor->rchild = p;
		}
		else
		{
			root = p;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == -2 && p->rchild == child)    // RR
	{
		// 前
		//                  A(parent,root),BF(-2)
		//                  |                
		//           AL_____|___________________|
		//                                      B(p),BF(-1)
		//                               BL_____|_____BR(X)

		// 后
		//                                      B(p,root),BF(0)
		//                                      |
		//                  A(parent),BF(0)_____|_____BR(X)
		//                  |
		//           AL_____|_____BL

		parent->rchild = p->lchild; // 和LL只有这两句不一样
		p->lchild = parent;

		parent->BF = 0;
		p->BF = 0;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // 修改p的双亲为ancestor
				ancestor->lchild = p;
			else
				ancestor->rchild = p;
		}
		else
		{
			root = p;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == 2 && p->rchild == child)    // LR
	{
		// 前
		//                                      A(parent,root),BF(2)
		//                                      |
		//                  B(p),BF(-1)_________|_____AR
		//                  |
		//           BL_____|_____C(pc),BF(1)
		//                        |
		//             CL(X)______|______CR

		// 后（减少一层）
		//                                C(pc,root),BF(0)
		//                                |
		//                  B(p),BF(0)____|_____A(parent),BF(-1)
		//                  |                   |
		//           BL_____|_____CL(x)  CR_____|_____AR

		N *pc = p->rchild;
		parent->lchild = pc->rchild;
		p->rchild = pc->lchild;
		pc->lchild = p;
		pc->rchild = parent;

		pc->BF = 0;
		p->BF = 0;
		parent->BF = -1;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // 修改pc的双亲为ancestor
				ancestor->lchild = pc;
			else
				ancestor->rchild = pc;
		}
		else
		{
			root = pc;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == -2 && p->lchild == child)    // RL
	{
		// 前
		//                  A(parent,root),BF(-2)
		//                  |
		//           AL_____|___________________B(p),BF(1)
		//                                      |
		//                        C(pc),BF(-1)__|______BR
		//                        |
		//                 CL_____|_____CR(X)

		// 后（减少一层）
		//                                 C(pc,root),BF(0)
		//                                 |
		//                  A(parent),BF(1)|_____B(p),BF(0)
		//                  |                    |
		//           AL_____|_____CL   CR(x)_____|_____BR

		N *pc = p->lchild;
		parent->rchild = pc->lchild;
		p->lchild = pc->rchild;
		pc->lchild = parent;
		pc->rchild = p;

		pc->BF = 0;
		p->BF = 0;
		parent->BF = 1;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // 修改pc的双亲为ancestor
				ancestor->lchild = pc;
			else
				ancestor->rchild = pc;
		}
		else
		{
			root = pc;
		}

		return true;
	}

	return true;
}

template<class T, class N>
bool AVLTree<T, N>::Delete(T data)
{
	N *p = FindNode(data);

	return p != NULL;
}

template<class T, class N>
bool AVLTree<T, N>::Find(T data)
{
	return FindNode(data) != NULL;
}

template<class T, class N>
N* AVLTree<T, N>::FindNode(T data)
{
	N *p = root;
	while (p != NULL)
	{
		if (p->data > data)
			p = p->rchild;
		else if (p->data < data)
			p = p->lchild;
		else
			return p;
	}
	return NULL;
}