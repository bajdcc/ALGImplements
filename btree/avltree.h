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
TStatus AVLTree<T, N>::CheckPathAfterDelete(N *&child, N *p, N *parent, N *ancestor)
{
	if (p && p->BF == 0)
	{
		if (parent->lchild == p) // p��parent�����ӣ�p��BFΪ0��˵��p��������һ����parent��BF--
			parent->BF--;
		else // p��parent���Һ��ӣ�p��BFΪ0��˵��p��������һ����parent��BF++
			parent->BF++;
	}

	if (parent->BF == 2) // *parent��ʧ���㣨��һ��|BF|>=2��
	{
		child = parent->lchild; // ��������
		return T_BREAK; // �ҵ���С��ƽ�������ĸ����
	}
	else if (parent->BF == -2) // *parent��ʧ���㣨��һ��|BF|>=2��
	{
		child = parent->rchild; // ��������
		return T_BREAK; // �ҵ���С��ƽ�������ĸ����
	}

	if (parent->BF != 0) // ��ɾ������*parent�����������߶Ȳ����ֵΪ1��|BF|Ϊ1����˵����*parentΪ����������δ��
		return T_OK;       // ����·���е��������Ƚ���������BF

	return T_CONTINUE;
}

template<class T, class N>
N* AVLTree<T, N>::Replace(stack<N*>& path)
{
	N *p = path.top();
	path.pop();
	N *parent = path.empty() ? NULL : path.top();
	N *silbing = NULL; // Ϊp���ֵܽ�㣬������ת
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
			silbing = parent->rchild;
		}
		else
		{
			parent->rchild = NULL;
			parent->BF++;
			silbing = parent->lchild;
		}
	}
	else if (p->lchild && p->rchild) // ˫������ת��Ϊ������
	{
		// ע�⣺�滻ʱ��BFҲҪ�滻
		if (p->lchild->rchild) // �滻���������ҽ��
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
			if (!parent) // �����
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
		else if (p->rchild->lchild) // �滻������������
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
			if (!parent) // �����
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
		else // �������
		{
			if (!parent) // �����
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
	else if (p->lchild) // ������
	{
		if (!parent) // �����
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
	else // �ҵ�����
	{
		if (!parent) // �����
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

	else if (parent->BF == -2 && (!child || p->rchild == child))    // RR
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

	else if (parent->BF == 2 && (!child || p->rchild == child))    // LR
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

	else if (parent->BF == -2 && (!child || p->lchild == child))    // RL
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

		N *pc = p->lchild;
		this->RotateRight(pc, p, parent);
		this->RotateLeft(pc, parent, ancestor);

		pc->BF = 0;
		p->BF = 0;
		parent->BF = 1;
	}
}
