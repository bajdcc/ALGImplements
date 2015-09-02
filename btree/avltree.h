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
	int BF; // ƽ������
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
	if (s.empty()) throw "���鲻��Ϊ��";
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

	stack<N*> path; // �洢����ǰ�Ĳ���·�������ڻ��ݣ�

	//////////////////////////////////////////////////////////////////////////
	// �������
	N *p = root;
	while (true)
	{
		path.push(p);
		if (newp->data < p->data) // ����ֵС�ڸ���㣬��������
		{
			if (p->lchild != NULL)
			{
				p = p->lchild; // ֵС��LL����ݹ���L
			}
			else
			{
				p->lchild = newp; break; // ����������ӣ����ò���
			}
		}
		else if (newp->data > p->data) // ����ֵ���ڸ���㣬��������
		{
			if (p->rchild != NULL)
			{
				p = p->rchild; // ֵ����RR����ݹ���R
			}
			else
			{
				p->rchild = newp; break; // ��������Һ��ӣ����ò���
			}
		}
		else // ����ֵ���ڸ���㣬����
		{
			delete newp; return false;
		}
	}

	// �������

	//////////////////////////////////////////////////////////////////////////

	// ��������·���Ͻ���BF����λʧ��Ľ��*p���丸���*parent

	N *child = NULL;  // *child��Ϊ*p�ĺ��ӽ��
	p = newp;
	N *parent = path.top(); // *parent��*p�ĸ����
	path.pop();
	while (true)
	{
		if (parent->lchild == p) // p��parent�����ӣ���ôparent��BF++
			parent->BF++;  // BF�ı仯��-1->0->1->2
		else // p��parent���Һ��ӣ���ôparent��BF--
			parent->BF--;  // BF�ı仯��1->0->-1->-2

		if (parent->BF == 2 || parent->BF == -2) // *parent��ʧ���㣨��һ��|BF|>=2��
			break; // �ҵ���С��ƽ�������ĸ����

		if (parent->BF == 0) // �ڲ����½���*parent�����������߶���ȣ�BFΪ0����˵����*parentΪ���������߶�δ����
			return true;       // ����·���е��������Ƚ���������BF

		if (path.empty()) // ֱ�����ĸ���㣬��path��û�н��|BF|����2�����Բ��ص�����������BFΪ+1,-1���´β������е�����
			return true;

		child = p; p = parent; parent = path.top(); // ��path���ϻ���
		path.pop();
	}

	//////////////////////////////////////////////////////////////////////////
	// *parentʧ�⣬���´�����е���
	N *ancestor = path.empty() ? NULL : path.top(); // ancestorΪparent��˫�׽��
	if (!path.empty()) path.pop();

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

		parent->lchild = p->rchild;
		p->rchild = parent;

		parent->BF = 0;
		p->BF = 0;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // �޸�p��˫��Ϊancestor
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

		parent->rchild = p->lchild; // ��LLֻ�������䲻һ��
		p->lchild = parent;

		parent->BF = 0;
		p->BF = 0;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // �޸�p��˫��Ϊancestor
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
		parent->lchild = pc->rchild;
		p->rchild = pc->lchild;
		pc->lchild = p;
		pc->rchild = parent;

		pc->BF = 0;
		p->BF = 0;
		parent->BF = -1;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // �޸�pc��˫��Ϊancestor
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
		parent->rchild = pc->lchild;
		p->lchild = pc->rchild;
		pc->lchild = parent;
		pc->rchild = p;

		pc->BF = 0;
		p->BF = 0;
		parent->BF = 1;

		if (ancestor != NULL)
		{
			if (ancestor->lchild == parent) // �޸�pc��˫��Ϊancestor
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