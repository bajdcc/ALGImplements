#pragma once

#include <stack>
#include <vector>
#include "baltree.h"

using namespace std;

const int RED = 0;
const int BLACK = 1;

/**
 *
 *		������Ĳ�������������������
 *
 *			1. ����			 -> ������Ϊ��
 *			2. ����			 -> ����Ϊ��
 *			3. �������		 -> ���Ϊ�ڣ���Ϊ�죬��������
 *			4. ��������⺢��-> ��תһ��
 *			5. ��������ں���-> ��ת����
 */

template<class T>
struct RBNode
{
	T data;
	RBTree *lchild, *rchild;
	char color; // ��ɫ
};

template<class T, class N = RBTree<T> >
class RBTree : public BALTree<T, N>
{
	public:
		RBTree();
		RBTree(const vector<T>& s);

		virtual bool Delete(T data);

	protected:
		virtual N* New();
		virtual TStatus CheckPathAfterInsert(N *child, N *p, N *parent, N *ancestor);
		virtual void InsertInternal(N *child, N *p, N *parent, N *ancestor);
		virtual void HandleRoot();
};

template<class T, class N>
RBTree<T, N>::RBTree(): BALTree<T, N>()
{

}

template<class T, class N>
RBTree<T, N>::RBTree(const vector<T>& s): BALTree<T, N>(s)
{

}

template<class T, class N>
N* RBTree<T, N>::New()
{
	N* newp = BiTree<T, N>::New();
	newp->color = RED;
	return newp;
}

template<class T, class N>
void BALTree<T, N>::HandleRoot()
{
	this->root->color = BLACK; // �����Ϊ��ɫ���������1
}

template<class T, class N>
TStatus RBTree<T, N>::CheckPathAfterInsert(N *child, N *p, N *parent, N *ancestor)
{
	if (child != NULL)
	{
		// child -> p -> parent
		if (p->color == BLACK) // ��Ϊ�ڣ��������2������Ϊ��
			return T_OK;
		else // ��Ϊ�죬��ͻ
		{
			bool child_L = p->lchild == child;
			bool p_L = parent->lchild == p;
			N *uncle = p_L ? parent->lchild : parent->rchild;
			if (uncle->color == RED) // ������죬�������3
			{
			}
		}
	}

	return T_CONTINUE;
}

template<class T, class N>
void RBTree<T, N>::InsertInternal(N *child, N *p, N *parent, N *ancestor)
{
	//////////////////////////////////////////////////////////////////////////

	if (parent->BF == 2 && p->lchild == child)    // LL
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

		parent->BF = 0;
		p->BF = 0;
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == -2 && p->rchild == child)    // RR
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

		parent->BF = 0;
		p->BF = 0;
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == 2 && p->rchild == child)    // LR
	{
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

		N *pc = p->rchild;
		this->RotateLeft(pc, p, parent);
		this->RotateRight(pc, parent, ancestor);

		pc->BF = 0;
		p->BF = 0;
		parent->BF = -1;
	}

	//////////////////////////////////////////////////////////////////////////

	else if (parent->BF == -2 && p->lchild == child)    // RL
	{
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

		N *pc = p->rchild;
		this->RotateRight(pc, p, parent);
		this->RotateLeft(pc, parent, ancestor);

		pc->BF = 0;
		p->BF = 0;
		parent->BF = 1;
	}
}

template<class T, class N>
bool RBTree<T, N>::Delete(T data)
{
	return false;
}
