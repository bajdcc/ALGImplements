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
	int BF; // ƽ������
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
	if (parent->lchild == p) // p��parent�����ӣ���ôparent��BF++
		parent->BF++;  // BF�ı仯��-1->0->1->2
	else // p��parent���Һ��ӣ���ôparent��BF--
		parent->BF--;  // BF�ı仯��1->0->-1->-2

	if (parent->BF == 2 || parent->BF == -2) // *parent��ʧ���㣨��һ��|BF|>=2��
		return T_BREAK; // �ҵ���С��ƽ�������ĸ����

	if (parent->BF == 0) // �ڲ����½���*parent�����������߶���ȣ�BFΪ0����˵����*parentΪ���������߶�δ����
		return T_OK;       // ����·���е��������Ƚ���������BF

	return T_CONTINUE;
}

template<class T, class N>
TStatus AVLTree<T, N>::CheckPathAfterDelete(N *child, N *p, N *parent, N *ancestor, TStatus status)
{
	if (status == T_OK)
	{
		if (p && p->BF == 0)
		{
			if (parent->lchild == p) // p��parent�����ӣ�p��BFΪ0��˵��p��������һ����parent��BF--
				parent->BF--;
			else // p��parent���Һ��ӣ�p��BFΪ0��˵��p��������һ����parent��BF++
				parent->BF++;
		}
	}
	else
	{
		if (status == T_DECL) //ƽ��������߶ȼ��٣���ôҪ���ϼ��ƽ����
		{
			parent->BF--;
			return parent->BF == 0 ? T_CONTINUE_STATUS : T_OK;
		}
		else if (status == T_DECR) //ƽ��������߶ȼ��٣���ôҪ���ϼ��ƽ����
		{
			parent->BF++;
			return parent->BF == 0 ? T_CONTINUE_STATUS : T_OK;
		}
	}

	if (parent->BF == 2 || parent->BF == -2) // *parent��ʧ���㣨��һ��|BF|>=2��
		return T_BREAK; // �ҵ���С��ƽ�������ĸ����

	if (parent->BF != 0) // ��ɾ������*parent�����������߶Ȳ����ֵΪ1��|BF|Ϊ1����˵����*parentΪ���������߶�δ�ı�
		return T_OK;       // ����·���е��������Ƚ���������BF

	return T_CONTINUE;
}

template<class T, class N>
void AVLTree<T, N>::Replace(stack<N*>& path)
{
	N *p = path.top();
	path.pop();
	N *parent = path.empty() ? NULL : path.top();
	if (!p->lchild && !p->rchild) // pΪҶ�ӽ�㣬ֱ��ɾ��
	{
		if (!parent) // �����
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
	else if (p->lchild && p->rchild) // ˫������ת��Ϊ������
	{
		// ע�⣺�滻ʱ��BFҲҪ�滻
		if (p->lchild->rchild) // �滻���������ҽ��
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
			if (!parent) // �����
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
		else if (p->rchild->lchild) // �滻������������
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
			if (!parent) // �����
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
		else // �������
		{
			if (!parent) // �����
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
	else if (p->lchild) // ������
	{
		if (!parent) // �����
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
	else // �ҵ�����
	{
		if (!parent) // �����
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
	// ǰ
	//                                      A(parent,root),BF(2)
	//                                      |
	//                  B(p),BF(1)__________|_____AR
	//                  |
	//        BL(X)_____|_____BR

	// ��
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
	// ǰ
	//                  A(parent,root),BF(-2)
	//                  |
	//           AL_____|___________________|
	//                                      B(p),BF(-1)
	//                               BL_____|_____BR(X)

	// ��
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

	// >>>> ���һ

	// ǰ
	//                                      A(parent,root),BF(2)
	//                                      |
	//                  B(p),BF(-1)_________|_____AR
	//                  |
	//           BL_____|_____C(pc),BF(1)
	//                        |
	//             CL(X)______|______CR

	// �󣨼���һ�㣩
	//                                C(pc,root),BF(0)
	//                                |
	//                  B(p),BF(0)____|_____A(parent),BF(-1)
	//                  |                   |
	//           BL_____|_____CL(x)  CR_____|_____AR

	//////////////////////////////////////////////////////////////////////////

	// >>>> �����

	// ǰ
	//                                      A(parent,root),BF(2)
	//                                      |
	//                  B(p),BF(-1)_________|_____AR
	//                  |
	//           BL_____|_____C(pc),BF(-1)
	//                        |
	//                CL______|______CR(X)

	// �󣨼���һ�㣩
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

	// >>>> ���һ

	// ǰ
	//                  A(parent,root),BF(-2)
	//                  |
	//           AL_____|___________________B(p),BF(1)
	//                                      |
	//                        C(pc),BF(-1)__|______BR
	//                        |
	//                 CL_____|_____CR(X)

	// �󣨼���һ�㣩
	//                                 C(pc,root),BF(0)
	//                                 |
	//                  A(parent),BF(1)|_____B(p),BF(0)
	//                  |                    |
	//           AL_____|_____CL   CR(x)_____|_____BR

	//////////////////////////////////////////////////////////////////////////

	// >>>> �����

	// ǰ
	//                  A(parent,root),BF(-2)
	//                  |
	//           AL_____|___________________B(p),BF(1)
	//                                      |
	//                        C(pc),BF(1)___|______BR
	//                        |
	//              CL(X)_____|_____CR

	// �󣨼���һ�㣩
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
