#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

#define MAX(a,b) (((a)>(b))?(a):(b))
// ���������Ľṹ����
template <class T>
struct BiTreeNode
{
	T data;
	BiTreeNode *lchild, *rchild;
};

// �������ඨ��
template <class T, class N = BiTreeNode<T> >
class BiTree
{
protected:
	N* root;
	string pre, mid;

public:
	BiTree();
	BiTree(string& pre);                    // ǰ׺����
	BiTree(string& pre, string& mid);       // ǰ׺����׺����
	virtual ~BiTree();

	BiTree(const BiTree &tree);             // ����������
	BiTree& operator = (const BiTree &tree);

	void CreateByPre(string& pre);          // ������չǰ׺���죨�����������Ϊ#��
	void CreateByPreMid(string& pre, string& mid); // ����ǰ׺����׺����

	void Release();                         // �ͷ��������Ŀռ�

	void PreOrder();                        // ǰ�����
	void InOrder();                         // �������
	void PostOrder();                       // �������
	void LevelOrder();                      // �������

	void PreOrderNoRecursion();             // ǰ��������ǵݹ飩
	void InOrderNoRecursion();              // ����������ǵݹ飩
	void PostOrderNoRecursion();            // ����������ǵݹ飩

	void PreOrderNoRecursion2();            // ǰ��������ǵݹ�2��
	void InOrderNoRecursion2();             // ����������ǵݹ�2��
	void PostOrderNoRecursion2();           // ����������ǵݹ�2��

	void Print();                           // ��ӡ����ͼ

	int Height();                           // ����߶�
	int Count();                            // ��������

	N* Find(T data);            // ����

protected:
	virtual N* New();
	N* Root() const;

	// �ݹ�����ڲ�����

	N* CreateByPre(int& ipre);
	N* CreateByPreMid(int ipre, int imid, int n);
	virtual void Release(N *bt);

	void PreOrder(N *bt);
	void InOrder(N *bt);
	void PostOrder(N *bt);

	void Print(N *bt, int level);

	int Height(N *bt);
	int Count(N *bt);

	N* Find(N *bt, T e);

	template<class X>
	N* Copy(X *bt)         // ģ�帴��
	{
		if (bt == NULL) return NULL;
		N *newp = New();
		newp->data = bt->data;
		newp->lchild = Copy(bt->lchild);  // ����������
		newp->rchild = Copy(bt->rchild);  // ����������
		return newp;
	}

	// �ǵݹ�
	enum NoRecursionType { PREORDER, INORDER, POSTORDER };
	void MainOrderNoRecursion(NoRecursionType type);    // �ǵݹ����ͨ�ð汾
};

/////////////////////////////////////////////////////////////////////

template <class T, class N>
BiTree<T, N>::BiTree() : root(NULL)
{
}

template <class T, class N>
BiTree<T, N>::BiTree(string& _pre) : root(NULL), pre(_pre)
{
	CreateByPre(_pre);
}

template <class T, class N>
void BiTree<T, N>::CreateByPre(string& _pre)
{
	pre = _pre;
	Release();
	int ipre = 0;
	if (pre.size() == 0) throw "ǰ׺����Ϊ�գ�";
	root = CreateByPre(ipre);
}

template <class T, class N>
BiTree<T, N>::BiTree(string& _pre, string& _mid) : root(NULL), pre(_pre), mid(_mid)
{
	CreateByPreMid(_pre, _mid);
}

template <class T, class N>
void BiTree<T, N>::CreateByPreMid(string& _pre, string& _mid)
{
	pre = _pre; mid = _mid;
	Release();
	int ipre = pre.size(), imid = mid.size();
	if (ipre == 0 || imid == 0) throw "ǰ׺����׺����Ϊ�գ�";
	if (ipre != imid) throw "ǰ׺����׺���Ȳ���ͬ��";
	root = CreateByPreMid(0, 0, ipre);
}

template <class T, class N>
N* BiTree<T, N>::New()
{
	N* newp = new N;
	newp->lchild = NULL;
	newp->rchild = NULL;
	return newp;
}

template <class T, class N>
BiTree<T, N>::~BiTree()
{
	Release();
}

template <class T, class N>
BiTree<T, N>::BiTree(const BiTree &tree)
{
	root = Copy(tree.root);
}

template <class T, class N>
BiTree<T, N>& BiTree<T, N>::operator=(const BiTree &tree)
{
	root = Copy(tree.root);
	return *this;
}

template <class T, class N>
N* BiTree<T, N>::Root() const
{
	return root;
}

template <class T, class N>
N *BiTree<T, N>::CreateByPre(int& ipre)
{
	if (ipre >= (int)pre.size())
		throw "���봮����";
	T e = pre[ipre++];
	if (e == '\0') return NULL;
	if (e == '#') return NULL;
	N *bt = New();
	bt->data = e;
	bt->lchild = CreateByPre(ipre);  // ��������
	bt->rchild = CreateByPre(ipre);  // ��������
	return bt;
}

template <class T, class N>
N* BiTree<T, N>::CreateByPreMid(int ipre, int imid, int n)
{
	if (n == 0) return NULL;
	N *p = New();
	p->data = pre[ipre];// ǰ׺Ϊ��-��-��
	int i;
	for (i = 0; i < n; i++)    // �����������ж�λ�����
	{
		if (pre[ipre] == mid[imid + i]) break;
	}
	if (i == n) throw "ǰ׺����׺�ַ���ƥ�䣡";
	p->lchild = CreateByPreMid(ipre + 1, imid, i);// ��������
	p->rchild = CreateByPreMid(ipre + i + 1, imid + i + 1, n - i - 1);// ��������
	return p;
}

template <class T, class N>
void BiTree<T, N>::Release()
{
	Release(root); root = NULL;
}

template <class T, class N>
void BiTree<T, N>::Release(N *bt)
{
	if (bt == NULL) return;
	Release(bt->lchild);  // �ͷ�������
	Release(bt->rchild);  // �ͷ�������
	delete bt;
}

template <class T, class N>
void BiTree<T, N>::PreOrder()
{
	PreOrder(root);
}

template <class T, class N>
void BiTree<T, N>::PreOrder(N *bt)
{
	if (bt == NULL) return;
	cout << bt->data;
	PreOrder(bt->lchild);
	PreOrder(bt->rchild);
}

template <class T, class N>
void BiTree<T, N>::InOrder()
{
	InOrder(root);
}

template <class T, class N>
void BiTree<T, N>::InOrder(N *bt)
{
	if (bt == NULL) return;
	InOrder(bt->lchild);
	cout << bt->data;
	InOrder(bt->rchild);
}

template <class T, class N>
void BiTree<T, N>::PostOrder()
{
	PostOrder(root);
}

template <class T, class N>
void BiTree<T, N>::PostOrder(N *bt)
{
	if (bt == NULL) return;
	PostOrder(bt->lchild);
	PostOrder(bt->rchild);
	cout << bt->data;
}

template <class T, class N>
void BiTree<T, N>::LevelOrder()
{
	if (root == NULL) return;
	N *p;
	queue<N*> Q;  // QΪѭ������
	Q.push(root);  // ��m_Root������
	while (!Q.empty())
	{
		p = Q.front();   // ������Ԫ�ظ���p������Ԫ�س�����
		Q.pop();
		cout << p->data;
		if (p->lchild)  Q.push(p->lchild);
		if (p->rchild)  Q.push(p->rchild);
	}
}

template <class T, class N>
void BiTree<T, N>::MainOrderNoRecursion(typename BiTree<T, N>::NoRecursionType type)
{
	if (root == NULL) return;

	//�ǵݹ�������ͨ�ð汾�����״̬��ָ��

	stack<N*> s;//���ջ
	stack<int> sip;//״̬��

	//ָ����ͣ�
	//visit(bt)
	//{
	//---- ins #0
	//    if(!bt)return //return=s.pop+sip.pop
	//---- ins #1 pre
	//    if(!bt.left)visit(bt.left) //visit=s.push(bt.left)+sip.push(0)
	//---- ins #2 in
	//    if(!bt.right)visit(bt.right) //visit=s.push(bt.right)+sip.push(0)
	//---- ins #3 post
	//}

	s.push(root);
	sip.push(0);

	while (!s.empty() || !sip.empty())
	{
		N* bt = s.top();//ȡ���ջ��

		switch (sip.top())//ȡָ��ջ��
		{
		case 0: sip.top()++;
			if (bt == NULL)//������NULL����ջ
			{
				s.pop();
				sip.pop();
				continue;
			}

		case 1: sip.top()++;
			if (type == PREORDER) cout << bt->data;
			if (bt->lchild != NULL)
			{
				s.push(bt->lchild);
				sip.push(0);
				continue;
			}

		case 2: sip.top()++;
			if (type == INORDER) cout << bt->data;
			if (bt->rchild != NULL)
			{
				s.push(bt->rchild);
				sip.push(0);
				continue;
			}

		case 3:
			if (type == POSTORDER) cout << bt->data;
			s.pop();
			sip.pop();
			continue;
		}

		throw "�Ƿ�IP��";
	}
}

template <class T, class N>
void BiTree<T, N>::PreOrderNoRecursion()
{
	MainOrderNoRecursion(PREORDER);
}

template <class T, class N>
void BiTree<T, N>::InOrderNoRecursion()
{
	MainOrderNoRecursion(INORDER);
}

template <class T, class N>
void BiTree<T, N>::PostOrderNoRecursion()
{
	MainOrderNoRecursion(POSTORDER);
}

template <class T, class N>
void BiTree<T, N>::PreOrderNoRecursion2()
{
	stack<N*> s;
	N *p = root;
	while (p != NULL || !s.empty())
	{
		while (p != NULL)//�����������㣬ͬʱ��¼·�������·������-��=(��->����)·����
		{
			cout << p->data;
			s.push(p);
			p = p->lchild;
		}
		if (!s.empty())
		{
			p = s.top();
			s.pop();//���ʺ󵯳������㣬��ǰΪ���󸸽��
			p = p->rchild;//�������󸸽����Һ���
		}
	}
}

template <class T, class N>
void BiTree<T, N>::InOrderNoRecursion2()
{
	stack<N*> s;
	N *p = root;
	while (p != NULL || !s.empty())
	{
		while (p != NULL)//��λ��������
		{
			s.push(p);
			p = p->lchild;
		}
		if (!s.empty())
		{
			p = s.top();
			cout << p->data;//�������㿪ʼ����
			s.pop();
			p = p->rchild;//���������㣨���Σ��ĸ������Һ���
		}
	}
}

template <class T, class N>
void BiTree<T, N>::PostOrderNoRecursion2()
{
	stack<N*> s;
	N *cur;                      //��ǰ��� 
	N *pre = NULL;               //ǰһ�η��ʵĽ�� 
	s.push(root);
	while (!s.empty())
	{
		cur = s.top();
		if ((cur->lchild == NULL&&cur->rchild == NULL) ||
			(pre != NULL && (pre == cur->lchild || pre == cur->rchild)))
		{
			//��ǰΪҶ�ӽ�����һ�η���Ϊ���ӽ�㣬������-��-��������-����˳�򣬺���ȫ�����ʹ������ŷ��ʸ����
			cout << cur->data;
			s.pop();
			pre = cur;
		}
		else
		{
			//��ǰΪ���ϵ��·��ʣ�����û���ʹ���������ջ
			if (cur->rchild != NULL) s.push(cur->rchild);
			if (cur->lchild != NULL) s.push(cur->lchild);//��-��-������ջ˳��Ϊ(��)-��-��
		}
	}
}

template <class T, class N>
void BiTree<T, N>::Print()
{
	Print(root, 0);
}

template <class T, class N>
void BiTree<T, N>::Print(N *bt, int level)
{
	if (bt == NULL) return;

	Print(bt->rchild, level + 1);
	{
		for (int i = 0; i < level; i++) cout << "    |"; //����
		cout << "��<" << bt->data << ">" << endl; //��ӡ"|��<data>"��ʽ
	}
	Print(bt->lchild, level + 1);
}

template <class T, class N>
int BiTree<T, N>::Height()
{
	return Height(root);
}

template <class T, class N>
int BiTree<T, N>::Height(N *bt)
{
	if (bt == NULL) return 0;
	int height1 = Height(bt->lchild), height2 = Height(bt->rchild);
	return MAX(height1, height2) + 1;
}

template <class T, class N>
int BiTree<T, N>::Count()
{
	return Count(root);
}

template <class T, class N>
int BiTree<T, N>::Count(N *bt)
{
	if (bt == NULL) return 0;
	return Count(bt->lchild) + Count(bt->rchild) + 1;
}

template <class T, class N>
N* BiTree<T, N>::Find(T data)
{
	return Find(root, data);
}

template <class T, class N>
N* BiTree<T, N>::Find(N *bt, T data)
{
	if (bt == NULL) return NULL;
	if (bt->data == data) return bt;
	N *f = Find(bt->lchild, data);
	if (f != NULL) return f;
	return Find(bt->rchild, data);
}
