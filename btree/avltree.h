#pragma once

#include <stack>
#include <vector>
#include "baltree.h"

using namespace std;

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
	virtual TStatus CheckPathAfterInsert(N *&child, N *p, N *parent, N *ancestor);
	virtual TStatus CheckPathAfterDelete(N *&child, N *p, N *parent, N *ancestor);
	virtual void BalanceInternal(N *child, N *p, N *parent, N *ancestor);
	virtual N* Replace(stack<N*>& path);
	virtual void PrintData(N *bt);
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
TStatus AVLTree<T, N>::CheckPathAfterInsert(N *&child, N *p, N *parent, N *ancestor)
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
TStatus AVLTree<T, N>::CheckPathAfterDelete(N *&child, N *p, N *parent, N *ancestor)
{
	if (p && p->BF == 0)
	{
		if (parent->lchild == p) // p是parent的左孩子，p的BF为0，说明p层数减少一，故parent的BF--
			parent->BF--;
		else // p是parent的右孩子，p的BF为0，说明p层数减少一，故parent的BF++
			parent->BF++;
	}

	if (parent->BF == 2) // *parent是失衡结点（第一个|BF|>=2）
	{
		child = parent->lchild; // 左树右旋
		return T_BREAK; // 找到最小不平衡子树的根结点
	}
	else if (parent->BF == -2) // *parent是失衡结点（第一个|BF|>=2）
	{
		child = parent->rchild; // 右树左旋
		return T_BREAK; // 找到最小不平衡子树的根结点
	}

	if (parent->BF != 0) // 在删除结点后，*parent的左右子树高度差绝对值为1（|BF|为1），说明以*parent为根的子树高未度
		return T_OK;       // 所以路径中的其余祖先结点无需调整BF

	return T_CONTINUE;
}

template<class T, class N>
N* AVLTree<T, N>::Replace(stack<N*>& path)
{
	N *p = path.top();
	path.pop();
	N *parent = path.empty() ? NULL : path.top();
	N *silbing = NULL; // 为p的兄弟结点，用于旋转
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
			silbing = parent->rchild;
		}
		else
		{
			parent->rchild = NULL;
			parent->BF++;
			silbing = parent->lchild;
		}
	}
	else if (p->lchild && p->rchild) // 双子树，转化为单子树
	{
		// 注意：替换时，BF也要替换
		if (p->lchild->rchild) // 替换左子树最右结点
		{
			path.push(p);
			path.push(p->lchild);
			N *lr = p->lchild->rchild;
			while (lr)
			{
				path.push(lr);
				lr = lr->rchild;
			}
			lr = path.top();
			lr->BF = p->BF;
			path.pop();
			if (!parent) // 根结点
				this->root = lr;
			else if (parent->lchild == p)
				parent->lchild = lr;
			else
				parent->rchild = lr;
			lr->lchild = p->lchild;
			lr->rchild = p->rchild;
			path.top()->rchild = NULL;
			path.top()->BF++;
		}
		else if (p->rchild->lchild) // 替换右子树最左结点
		{
			path.push(p);
			path.push(p->rchild);
			N *rl = p->rchild->lchild;
			while (rl)
			{
				path.push(rl);
				rl = rl->lchild;
			}
			rl = path.top();
			rl->BF = p->BF;
			path.pop();
			if (!parent) // 根结点
				this->root = rl;
			else if (parent->lchild == p)
				parent->lchild = rl;
			else
				parent->rchild = rl;
			rl->lchild = p->lchild;
			rl->rchild = p->rchild;
			path.top()->lchild = NULL;
			path.top()->BF--;
		}
		else // 替代孩子
		{
			if (!parent) // 根结点
			{
				this->root = p->lchild;
				this->root->rchild = p->rchild;
				this->root->BF = p->BF - 1;
				silbing = p->rchild;
			}
			else if (parent->lchild == p)
			{
				parent->lchild = p->rchild;
				parent->lchild->lchild = p->lchild;
				parent->lchild->BF = p->BF + 1;
				parent->lchild->BF++;
				silbing = p->lchild;
				path.push(parent->lchild);
			}
			else
			{
				parent->rchild = p->lchild;
				parent->rchild->rchild = p->rchild;
				parent->rchild->BF = p->BF - 1;
				silbing = p->rchild;
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
				parent->lchild = NULL;
			else
				parent->rchild = NULL;
			parent->lchild = p->lchild;
			parent->BF--;
			silbing = parent->rchild;
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
				parent->lchild = NULL;
			else
				parent->rchild = NULL;
			parent->rchild = p->rchild;
			parent->BF++;
			silbing = parent->lchild;
		}
	}
	delete p;
	return silbing;
}

template<class T, class N>
void AVLTree<T, N>::BalanceInternal(N *child, N *p, N *parent, N *ancestor)
{
	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == 2 && (!child || p->lchild == child))    // LL
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

		parent->BF = 0;
		p->BF = 0;
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == -2 && (!child || p->rchild == child))    // RR
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

		parent->BF = 0;
		p->BF = 0;
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == 2 && (!child || p->rchild == child))    // LR
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
		this->RotateLeft(pc, p, parent);
		this->RotateRight(pc, parent, ancestor);

		pc->BF = 0;
		p->BF = 0;
		parent->BF = -1;
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == -2 && (!child || p->lchild == child))    // RL
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
		this->RotateRight(pc, p, parent);
		this->RotateLeft(pc, parent, ancestor);

		pc->BF = 0;
		p->BF = 0;
		parent->BF = 1;
	}
}
