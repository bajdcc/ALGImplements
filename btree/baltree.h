#pragma once

#include <stack>
#include <vector>
#include "btree.h"

using namespace std;

const int T_OK = 0;
const int T_BREAK = 1;
const int T_CONTINUE = 2;
const int T_CONTINUE_STATUS = 3;
typedef int TStatus;

template<class T, class N>
class BALTree : public BiTree<T, N>
{
	public:
		BALTree();
		BALTree(const vector<T>& s);

		virtual bool Insert(T data);
		virtual bool Delete(T data);

	protected:
		virtual TStatus CheckPathAfterInsert(N *child, N *p, N *parent, N *ancestor) = 0;
		virtual TStatus CheckPathAfterDelete(N *child, N *p, N *parent, N *ancestor, TStatus status) = 0;
		virtual TStatus BalanceInternalAfterInsert(N *child, N *p, N *parent, N *ancestor) = 0;
		virtual TStatus BalanceInternalAfterDelete(N *child, N *p, N *parent, N *ancestor) = 0;
		void RotateLeft(N *p, N *parent, N *ancestor); //����
		void RotateRight(N *p, N *parent, N *ancestor); //����
		virtual void HandleRoot();
		void BalanceAfterInsert(stack<N*>& path);
		void BalanceAfterDelete(stack<N*>& path);
		virtual void Replace(stack<N*>& path) = 0;
};

template<class T, class N>
BALTree<T, N>::BALTree()
{

}

template<class T, class N>
BALTree<T, N>::BALTree(const vector<T>& s)
{
	if (s.empty()) throw "���鲻��Ϊ��";
	this->root = this->New();
	for (int i = 1; i < s.Length(); i++) this->Insert(s[i]);
}

template<class T, class N>
void BALTree<T, N>::HandleRoot()
{

}

template<class T, class N>
void BALTree<T, N>::RotateLeft(N *p, N *parent, N *ancestor)
{
	// ǰ
	//                  A(parent,root)
	//                  |
	//           AL_____|___________________|
	//                                      B(p)
	//                               BL_____|_____BR(X)

	// ��
	//                                      B(p,root)
	//                                      |
	//                  A(parent)___________|_____BR(X)
	//                  |
	//           AL_____|_____BL

	parent->rchild = p->lchild;
	p->lchild = parent;

	if (ancestor != NULL)
	{
		if (ancestor->lchild == parent) // �޸�p��˫��Ϊancestor
			ancestor->lchild = p;
		else
			ancestor->rchild = p;
	}
	else
	{
		this->root = p;
	}
}

template<class T, class N>
void BALTree<T, N>::RotateRight(N *p, N *parent, N *ancestor)
{
	// ǰ
	//                                      A(parent,root)
	//                                      |
	//                  B(p)________________|_____AR
	//                  |
	//        BL(X)_____|_____BR

	// ��
	//                  B(p,root)
	//                  |
	//        BL(X)_____|___________________A(parent)
	//                                      |
	//                               BR_____|_____AR

	parent->lchild = p->rchild;
	p->rchild = parent;

	if (ancestor != NULL)
	{
		if (ancestor->lchild == parent) // �޸�p��˫��Ϊancestor
			ancestor->lchild = p;
		else
			ancestor->rchild = p;
	}
	else
	{
		this->root = p;
	}
}

template<class T, class N>
void BALTree<T, N>::BalanceAfterInsert(stack<N*>& path)
{
	N *child = NULL;  // *child��Ϊ*p�ĺ��ӽ��
	N *p = path.top();
	path.pop();
	N *parent = path.top(); // *parent��*p�ĸ����
	path.pop();
	N *ancestor;
	while (true)
	{
		ancestor = path.empty() ? NULL : path.top();
		TStatus status = CheckPathAfterInsert(child, p, parent, ancestor);
		switch (status)
		{
			case T_OK:
				return;
			case T_BREAK:
				BalanceInternalAfterInsert(child, p, parent, ancestor);
				return;
			case T_CONTINUE:
				break;
		}

		if (path.empty())
			return;

		child = p;
		p = parent;
		parent = path.top(); // ��path���ϻ���
		path.pop();
	}
}

template<class T, class N>
void BALTree<T, N>::BalanceAfterDelete(stack<N*>& path)
{
	// ������
	Replace(path);

	if (path.empty())
		return;

	// ɾ����㲢ƽ��

	N *p = NULL;
	N *child = NULL;  // *child��Ϊ*p�ĺ��ӽ��
	N *parent = path.top(); // *parent��*p�ĸ����
	path.pop();
	N *ancestor;
	TStatus status = T_OK;
	while (true)
	{
		ancestor = path.empty() ? NULL : path.top();
		if (status == T_CONTINUE_STATUS)
			status = ancestor ? parent == ancestor->lchild ? T_DECL : T_DECR : T_OK;
		status = CheckPathAfterDelete(child, p, parent, ancestor, status);
		switch (status)
		{
		case T_OK:
			return;
		case T_BREAK:
			status = BalanceInternalAfterDelete(child, p, parent, ancestor);
			if (status == T_OK)
				return;
			break;
		case T_CONTINUE:
			status = T_OK;
			break;
		case T_CONTINUE_STATUS:
			break;
		}

		if (path.empty())
			return;

		child = p;
		p = parent;
		parent = path.top(); // ��path���ϻ���
		path.pop();
	}
}

template<class T, class N>
bool BALTree<T, N>::Insert(T data)
{
	N* newp;

	if (this->root == NULL)
	{
		newp = this->New();
		newp->data = data;
		this->root = newp;
		this->HandleRoot();
		return true;
	}

	stack<N*> path; // �洢����ǰ�Ĳ���·�������ڻ��ݣ�

	//////////////////////////////////////////////////////////////////////////
	// �������
	N *p = this->root;
	while (true)
	{
		path.push(p);
		if (data < p->data) // ����ֵС�ڸ���㣬��������
		{
			if (p->lchild != NULL)
			{
				p = p->lchild; // ֵС��LL����ݹ���L
			}
			else
			{
				newp = this->New();
				newp->data = data;
				path.push(newp);
				p->lchild = newp;
				break; // ����������ӣ����ò���
			}
		}
		else if (data > p->data) // ����ֵ���ڸ���㣬��������
		{
			if (p->rchild != NULL)
			{
				p = p->rchild; // ֵ����RR����ݹ���R
			}
			else
			{
				newp = this->New();
				newp->data = data;
				path.push(newp);
				p->rchild = newp;
				break; // ��������Һ��ӣ����ò���
			}
		}
		else // ����ֵ���ڸ���㣬����
		{
			return false;
		}
	}

	// �������

	//////////////////////////////////////////////////////////////////////////

	// ��������·���ϵĽ��
	BalanceAfterInsert(path);
	return true;
}

template<class T, class N>
bool BALTree<T, N>::Delete(T data)
{
	if (this->root == NULL)
	{
		return false;
	}

	stack<N*> path; // �洢ɾ��ǰ�Ĳ���·�������ڻ��ݣ�

	//////////////////////////////////////////////////////////////////////////
	// ���Ҳ���
	N *p = this->root;
	while (true)
	{
		path.push(p);
		if (data < p->data) // ����ֵС�ڸ���㣬��������
		{
			if (p->lchild != NULL)
			{
				p = p->lchild; // ֵС��LL����ݹ���L
			}
			else
			{
				return false; // û�ҵ�
			}
		}
		else if (data > p->data) // ����ֵ���ڸ���㣬��������
		{
			if (p->rchild != NULL)
			{
				p = p->rchild; // ֵ����RR����ݹ���R
			}
			else
			{
				return false; // û�ҵ�
			}
		}
		else // �ҵ�
		{
			break;
		}
	}

	// ��λ�ɹ�

	//////////////////////////////////////////////////////////////////////////

	// ����ɾ��·���ϵĽ��
	BalanceAfterDelete(path);
	return true;
}
