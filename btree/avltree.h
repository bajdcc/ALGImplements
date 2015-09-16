#pragma once

#include <stack>
#include <vector>
#include "baltree.h"

using namespace std;

const int T_DECL = 10;
const int T_DECR = 11;

template<class T>
struct AVLNode
{
	T data;
	AVLNode *lchild, *rchild;
	int BF; // 平衡因子
};

template<class T, class N = AVLNode<T> >
class AVLTree : public BALTree<T, N>
{
public:
	AVLTree();
	AVLTree(const vector<T>& s);

protected:
	virtual N* New();
	virtual TStatus CheckPathAfterInsert(N *child, N *p, N *parent, N *ancestor);
	virtual TStatus CheckPathAfterDelete(N *child, N *p, N *parent, N *ancestor, TStatus status);
	virtual TStatus BalanceInternalAfterInsert(N *child, N *p, N *parent, N *ancestor);
	virtual TStatus BalanceInternalAfterDelete(N *child, N *p, N *parent, N *ancestor);
	virtual void Replace(stack<N*>& path);
	virtual void PrintData(N *bt);

	TStatus RotateLL(N *p, N *parent, N *ancestor);
	TStatus RotateRR(N *p, N *parent, N *ancestor);
	TStatus RotateLR(N *p, N *parent, N *ancestor);
	TStatus RotateRL(N *p, N *parent, N *ancestor);
};

template<class T, class N>
AVLTree<T, N>::AVLTree() : BALTree<T, N>()
{

}

template<class T, class N>
AVLTree<T, N>::AVLTree(const vector<T>& s) : BALTree<T, N>(s)
{

}

template<class T, class N>
N* AVLTree<T, N>::New()
{
	N* newp = BiTree<T, N>::New();
	newp->BF = 0;
	return newp;
}
template<class T, class N>
void AVLTree<T, N>::PrintData(N *bt)
{
	cout << bt->data << ',' << bt->BF;
}

template<class T, class N>
TStatus AVLTree<T, N>::CheckPathAfterInsert(N *child, N *p, N *parent, N *ancestor)
{
	if (parent->lchild == p) // p是parent的左孩子，那么parent的BF++
		parent->BF++;  // BF的变化：-1->0->1->2
	else // p是parent的右孩子，那么parent的BF--
		parent->BF--;  // BF的变化：1->0->-1->-2

	if (parent->BF == 2 || parent->BF == -2) // *parent是失衡结点（第一个|BF|>=2）
		return T_BREAK; // 找到最小不平衡子树的根结点

	if (parent->BF == 0) // 在插入新结点后，*parent的左右子树高度相等（BF为0），说明以*parent为根的子树高度未增加
		return T_OK;       // 所以路径中的其余祖先结点无需调整BF

	return T_CONTINUE;
}

template<class T, class N>
TStatus AVLTree<T, N>::CheckPathAfterDelete(N *child, N *p, N *parent, N *ancestor, TStatus status)
{
	if (status == T_OK)
	{
		if (p && p->BF == 0)
		{
			if (parent->lchild == p) // p是parent的左孩子，p的BF为0，说明p层数减少一，故parent的BF--
				parent->BF--;
			else // p是parent的右孩子，p的BF为0，说明p层数减少一，故parent的BF++
				parent->BF++;
		}
	}
	else
	{
		if (status == T_DECL) //平衡后，子树高度减少，那么要向上检查平衡性
		{
			parent->BF--;
			return parent->BF == 0 ? T_CONTINUE_STATUS : T_OK;
		}
		else if (status == T_DECR) //平衡后，子树高度减少，那么要向上检查平衡性
		{
			parent->BF++;
			return parent->BF == 0 ? T_CONTINUE_STATUS : T_OK;
		}
	}

	if (parent->BF == 2 || parent->BF == -2) // *parent是失衡结点（第一个|BF|>=2）
		return T_BREAK; // 找到最小不平衡子树的根结点

	if (parent->BF != 0) // 在删除结点后，*parent的左右子树高度差绝对值为1（|BF|为1），说明以*parent为根的子树高度未改变
		return T_OK;       // 所以路径中的其余祖先结点无需调整BF

	return T_CONTINUE;
}

template<class T, class N>
void AVLTree<T, N>::Replace(stack<N*>& path)
{
	N *p = path.top();
	path.pop();
	N *parent = path.empty() ? NULL : path.top();
	if (!p->lchild && !p->rchild) // p为叶子结点，直接删除
	{
		if (!parent) // 根结点
		{
			this->root = NULL;
		}
		else if (parent->lchild == p)
		{
			parent->lchild = NULL;
			parent->BF--;
		}
		else
		{
			parent->rchild = NULL;
			parent->BF++;
		}
	}
	else if (p->lchild && p->rchild) // 双子树，转化为单子树
	{
		// 注意：替换时，BF也要替换
		if (p->lchild->rchild) // 替换左子树最右结点
		{
			vector<N*> new_path;
			new_path.push_back(p->lchild);
			N *lr = p->lchild->rchild;
			while (lr)
			{
				new_path.push_back(lr);
				lr = lr->rchild;
			}
			path.push(new_path.back());
			lr = path.top();
			new_path.pop_back();
			for (vector<N*>::iterator it = new_path.begin(); it != new_path.end(); it++)
			{
				path.push(*it);
			}
			lr->BF = p->BF;
			if (!parent) // 根结点
				this->root = lr;
			else if (parent->lchild == p)
				parent->lchild = lr;
			else
				parent->rchild = lr;
			path.top()->rchild = lr->lchild;
			path.top()->BF++;
			lr->lchild = p->lchild;
			lr->rchild = p->rchild;
		}
		else if (p->rchild->lchild) // 替换右子树最左结点
		{
			vector<N*> new_path;
			new_path.push_back(p->rchild);
			N *rl = p->rchild->lchild;
			while (rl)
			{
				new_path.push_back(rl);
				rl = rl->lchild;
			}
			path.push(new_path.back());
			rl = path.top();
			new_path.pop_back();
			for (vector<N*>::iterator it = new_path.begin(); it != new_path.end(); it++)
			{
				path.push(*it);
			}
			rl->BF = p->BF;
			if (!parent) // 根结点
				this->root = rl;
			else if (parent->lchild == p)
				parent->lchild = rl;
			else
				parent->rchild = rl;
			path.top()->lchild = rl->rchild;
			path.top()->BF--;
			rl->lchild = p->lchild;
			rl->rchild = p->rchild;
		}
		else // 替代孩子
		{
			if (!parent) // 根结点
			{
				if (this->root->BF != -1)
				{
					this->root = p->lchild;
					this->root->rchild = p->rchild;
					this->root->BF = p->BF - 1;
				}
				else
				{
					this->root = p->rchild;
					this->root->lchild = p->lchild;
					this->root->BF = p->BF + 1;
				}
			}
			else if (parent->lchild == p)
			{
				if (parent->BF != -1)
				{
					parent->lchild = p->lchild;
					parent->lchild->rchild = p->rchild;
					parent->lchild->BF = p->BF - 1;
				}
				else
				{
					parent->lchild = p->rchild;
					parent->lchild->lchild = p->lchild;
					parent->lchild->BF = p->BF + 1;
				}
				path.push(parent->lchild);
			}
			else
			{
				if (parent->BF != -1)
				{
					parent->rchild = p->lchild;
					parent->rchild->rchild = p->rchild;
					parent->rchild->BF = p->BF - 1;
				}
				else
				{
					parent->rchild = p->rchild;
					parent->rchild->lchild = p->lchild;
					parent->rchild->BF = p->BF + 1;
				}
				path.push(parent->rchild);
			}
		}
	}
	else if (p->lchild) // 左单子树
	{
		if (!parent) // 根结点
		{
			this->root = p->lchild;
		}
		else
		{
			if (parent->lchild == p)
			{
				parent->lchild = p->lchild;
				parent->BF--;
			}
			else
			{
				parent->rchild = p->lchild;
				parent->BF++;
			}
		}
	}
	else // 右单子树
	{
		if (!parent) // 根结点
		{
			this->root = p->rchild;
		}
		else
		{
			if (parent->lchild == p)
			{
				parent->lchild = p->rchild;
				parent->BF--;
			}
			else
			{
				parent->rchild = p->rchild;
				parent->BF++;
			}
		}
	}
	delete p;
}

template<class T, class N>
TStatus AVLTree<T, N>::BalanceInternalAfterInsert(N *child, N *p, N *parent, N *ancestor)
{
	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == 2 && p->lchild == child)    // LL
	{
		RotateLL(p, parent, ancestor);
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == -2 && p->rchild == child)    // RR
	{
		RotateRR(p, parent, ancestor);
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == 2 && p->rchild == child)    // LR
	{
		RotateLR(p, parent, ancestor);
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == -2 && p->lchild == child)    // RL
	{
		RotateRL(p, parent, ancestor);
	}

	//////////////////////////////////////////////////////////////////////////

	return T_OK;
}

template<class T, class N>
TStatus AVLTree<T, N>::BalanceInternalAfterDelete(N *child, N *p, N *parent, N *ancestor)
{
	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == 2 && (!p || parent->rchild == p))    // L
	{
		p = parent->lchild;

		if (p->BF == -1) // LR
		{
			return RotateLR(p, parent, ancestor);
		}
		else // LL
		{
			return RotateLL(p, parent, ancestor);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == -2 && (!p || parent->lchild == p))    // R
	{
		p = parent->rchild;

		if (p->BF == 1) // RL
		{
			return RotateRL(p, parent, ancestor);
		}
		else // RR
		{
			return RotateRR(p, parent, ancestor);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	return T_OK;
}

template<class T, class N>
inline TStatus AVLTree<T, N>::RotateLL(N *p, N *parent, N *ancestor)
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

	this->RotateRight(p, parent, ancestor);

	parent->BF = p->BF == 1 ? 0 : 1;
	p->BF--;
	return ancestor ? p == ancestor->lchild ? T_DECL : T_DECR : T_OK;
}

template<class T, class N>
inline TStatus AVLTree<T, N>::RotateRR(N *p, N *parent, N *ancestor)
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

	this->RotateLeft(p, parent, ancestor);

	parent->BF = p->BF == -1 ? 0 : -1;
	p->BF++;
	return ancestor ? p == ancestor->lchild ? T_DECL : T_DECR : T_OK;
}

template<class T, class N>
inline TStatus AVLTree<T, N>::RotateLR(N *p, N *parent, N *ancestor)
{
	//////////////////////////////////////////////////////////////////////////

	// >>>> 情况一

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

	//////////////////////////////////////////////////////////////////////////

	// >>>> 情况二

	// 前
	//                                      A(parent,root),BF(2)
	//                                      |
	//                  B(p),BF(-1)_________|_____AR
	//                  |
	//           BL_____|_____C(pc),BF(-1)
	//                        |
	//                CL______|______CR(X)

	// 后（减少一层）
	//                                C(pc,root),BF(0)
	//                                |
	//                  B(p),BF(1)____|_____A(parent),BF(0)
	//                  |                   |
	//           BL_____|_____CL  CR(x)_____|_____AR

	N *pc = p->rchild;
	this->RotateLeft(pc, p, parent);
	this->RotateRight(pc, parent, ancestor);

	if (pc->BF == 1)
	{
		p->BF = 0;
		parent->BF = -1;
		pc->BF = 0;
	}
	else if (pc->BF == -1)
	{
		p->BF = 1;
		parent->BF = 0;
		pc->BF = 0;
	}
	else
	{
		parent->BF = 0;
		p->BF = 0;
	}
	return ancestor ? pc == ancestor->lchild ? T_DECL : T_DECR : T_OK;
}

template<class T, class N>
inline TStatus AVLTree<T, N>::RotateRL(N *p, N *parent, N *ancestor)
{
	//////////////////////////////////////////////////////////////////////////

	// >>>> 情况一

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

	//////////////////////////////////////////////////////////////////////////

	// >>>> 情况二

	// 前
	//                  A(parent,root),BF(-2)
	//                  |
	//           AL_____|___________________B(p),BF(1)
	//                                      |
	//                        C(pc),BF(1)___|______BR
	//                        |
	//              CL(X)_____|_____CR

	// 后（减少一层）
	//                                 C(pc,root),BF(0)
	//                                 |
	//                  A(parent),BF(0)|_____B(p),BF(-1)
	//                  |                    |
	//           AL_____|_____CL(x)   CR_____|_____BR

	N *pc = p->lchild;
	this->RotateRight(pc, p, parent);
	this->RotateLeft(pc, parent, ancestor);

	if (pc->BF == -1)
	{
		parent->BF = 1;
		p->BF = 0;
		pc->BF = 0;
	}
	else if (pc->BF == 1)
	{
		parent->BF = 0;
		p->BF = -1;
		pc->BF = 0;
	}
	else
	{
		parent->BF = 0;
		p->BF = 0;
	}
	return ancestor ? pc == ancestor->lchild ? T_DECL : T_DECR : T_OK;
}
