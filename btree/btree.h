#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

#define MAX(a,b) (((a)>(b))?(a):(b))
// 二叉树结点的结构定义
template <class T>
struct BiTreeNode
{
	T data;
	BiTreeNode *lchild, *rchild;
};

// 二叉树类定义
template <class T, class N = BiTreeNode<T> >
class BiTree
{
protected:
	N* root;
	string pre, mid;

public:
	BiTree();
	BiTree(string& pre);                    // 前缀构造
	BiTree(string& pre, string& mid);       // 前缀和中缀构造
	virtual ~BiTree();

	BiTree(const BiTree &tree);             // 复制树对象
	BiTree& operator = (const BiTree &tree);

	void CreateByPre(string& pre);          // 根据扩展前缀构造（层序遍历，空为#）
	void CreateByPreMid(string& pre, string& mid); // 根据前缀和中缀构造

	void Release();                         // 释放整个树的空间

	void PreOrder();                        // 前序遍历
	void InOrder();                         // 中序遍历
	void PostOrder();                       // 后序遍历
	void LevelOrder();                      // 层序遍历

	void PreOrderNoRecursion();             // 前序遍历（非递归）
	void InOrderNoRecursion();              // 中序遍历（非递归）
	void PostOrderNoRecursion();            // 后序遍历（非递归）

	void PreOrderNoRecursion2();            // 前序遍历（非递归2）
	void InOrderNoRecursion2();             // 中序遍历（非递归2）
	void PostOrderNoRecursion2();           // 后序遍历（非递归2）

	void Print();                           // 打印树形图

	int Height();                           // 计算高度
	int Count();                            // 计算结点数

	N* Find(T data);            // 查找

protected:
	virtual N* New();
	N* Root() const;

	// 递归调用内部函数

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
	N* Copy(X *bt)         // 模板复制
	{
		if (bt == NULL) return NULL;
		N *newp = New();
		newp->data = bt->data;
		newp->lchild = Copy(bt->lchild);  // 复制左子树
		newp->rchild = Copy(bt->rchild);  // 复制右子树
		return newp;
	}

	// 非递归
	enum NoRecursionType { PREORDER, INORDER, POSTORDER };
	void MainOrderNoRecursion(NoRecursionType type);    // 非递归遍历通用版本
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
	if (pre.size() == 0) throw "前缀不能为空！";
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
	if (ipre == 0 || imid == 0) throw "前缀或中缀不能为空！";
	if (ipre != imid) throw "前缀和中缀长度不相同！";
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
		throw "输入串错误";
	T e = pre[ipre++];
	if (e == '\0') return NULL;
	if (e == '#') return NULL;
	N *bt = New();
	bt->data = e;
	bt->lchild = CreateByPre(ipre);  // 建左子树
	bt->rchild = CreateByPre(ipre);  // 建右子树
	return bt;
}

template <class T, class N>
N* BiTree<T, N>::CreateByPreMid(int ipre, int imid, int n)
{
	if (n == 0) return NULL;
	N *p = New();
	p->data = pre[ipre];// 前缀为根-左-右
	int i;
	for (i = 0; i < n; i++)    // 在中序序列中定位根结点
	{
		if (pre[ipre] == mid[imid + i]) break;
	}
	if (i == n) throw "前缀和中缀字符不匹配！";
	p->lchild = CreateByPreMid(ipre + 1, imid, i);// 建左子树
	p->rchild = CreateByPreMid(ipre + i + 1, imid + i + 1, n - i - 1);// 建右子树
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
	Release(bt->lchild);  // 释放左子树
	Release(bt->rchild);  // 释放右子树
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
	queue<N*> Q;  // Q为循环队列
	Q.push(root);  // 将m_Root进队列
	while (!Q.empty())
	{
		p = Q.front();   // 将队首元素赋给p，队首元素出队列
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

	//非递归树遍历通用版本，结合状态机指令

	stack<N*> s;//结点栈
	stack<int> sip;//状态机

	//指令解释：
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
		N* bt = s.top();//取结点栈顶

		switch (sip.top())//取指令栈顶
		{
		case 0: sip.top()++;
			if (bt == NULL)//遍历到NULL，出栈
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

		throw "非法IP！";
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
		while (p != NULL)//遍历到最左结点，同时记录路径，输出路径（根-左=(根->最左)路径）
		{
			cout << p->data;
			s.push(p);
			p = p->lchild;
		}
		if (!s.empty())
		{
			p = s.top();
			s.pop();//访问后弹出最左结点，当前为最左父结点
			p = p->rchild;//访问最左父结点的右孩子
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
		while (p != NULL)//定位到最左结点
		{
			s.push(p);
			p = p->lchild;
		}
		if (!s.empty())
		{
			p = s.top();
			cout << p->data;//从最左结点开始访问
			s.pop();
			p = p->rchild;//访问最左结点（依次）的父结点的右孩子
		}
	}
}

template <class T, class N>
void BiTree<T, N>::PostOrderNoRecursion2()
{
	stack<N*> s;
	N *cur;                      //当前结点 
	N *pre = NULL;               //前一次访问的结点 
	s.push(root);
	while (!s.empty())
	{
		cur = s.top();
		if ((cur->lchild == NULL&&cur->rchild == NULL) ||
			(pre != NULL && (pre == cur->lchild || pre == cur->rchild)))
		{
			//当前为叶子结点或上一次访问为孩子结点，即按左-右-根（孩子-根）顺序，孩子全部访问过，接着访问父结点
			cout << cur->data;
			s.pop();
			pre = cur;
		}
		else
		{
			//当前为从上到下访问，孩子没访问过，则孩子入栈
			if (cur->rchild != NULL) s.push(cur->rchild);
			if (cur->lchild != NULL) s.push(cur->lchild);//左-右-根，入栈顺序为(根)-右-左
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
		for (int i = 0; i < level; i++) cout << "    |"; //缩进
		cout << "—<" << bt->data << ">" << endl; //打印"|—<data>"形式
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
