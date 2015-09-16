#include "stdafx.h"
#include <iostream>

#include "btree.h"
#include "avltree.h"

using namespace std;

typedef BiTree<char> BinTree;
typedef AVLTree<int> AVL;

void PrintMenu();

void CreateByPre(BinTree& tree);
void CreateByPreMid(BinTree& tree);
void PrintOrders(BinTree& tree);
void PrintCountAndHeight(BinTree& tree);
void PrintThreadInOrder(BinTree& tree);

template<class T> void PrintStructure(T& tree)
{
	cout << "树形图：" << endl;
	tree.Print();
	cout << endl;
}

void BSTreeOper();
void BSTreeOper(AVL& avl, int type);

void PrintMenu()
{
	cout << "1. 扩展前缀构造" << endl;
	cout << "2. 前缀和中缀构造" << endl;
	cout << "3. 遍历" << endl;
	cout << "4. 打印树形图" << endl;
	cout << "5. 打印高度和结点数" << endl;
	cout << "6. 平衡二叉树" << endl;
	cout << "0. 退出" << endl;
	cout << endl;
}

int main(int argc, char* argv[])
{
	PrintMenu();

	AVL avl;
	BinTree tree;

	while (true)
	{
		cout << "输入序号：";
		try
		{
			char buf[3];
			cin.getline(buf, 3);
			cin.sync();

			switch (buf[0])
			{
			case '1': CreateByPre(tree); break;
			case '2': CreateByPreMid(tree); break;
			case '3': PrintOrders(tree); break;
			case '4': PrintStructure(tree); break;
			case '5': PrintCountAndHeight(tree); break;
			case '6': BSTreeOper(); break;
			case '0': exit(0);
			default: throw "请输入正确的序号！";
			}
		}
		catch (const char *pstr)
		{
			cerr << "错误： " << pstr << endl << endl;
		}

		cout << endl;
	}
	return 0;
}

void CreateByPre(BinTree& tree)
{
	cout << "请输入扩展前缀：" << endl;
	char buf[255];
	cin.getline(buf, 254);
	string pre = buf;
	tree.CreateByPre(pre);
}

void CreateByPreMid(BinTree& tree)
{
	cout << "请输入前缀：" << endl;
	char buf[255];
	cin.getline(buf, 254);
	string pre = buf;
	cout << "请输入中缀：" << endl;
	cin.getline(buf, 254);
	string mid = buf;
	tree.CreateByPreMid(pre, mid);
}

void PrintOrders(BinTree& tree)
{
	cout << "递归：";
	cout << endl << "前序遍历："; tree.PreOrder();
	cout << endl << "中序遍历："; tree.InOrder();
	cout << endl << "后序遍历："; tree.PostOrder();
	cout << endl << "非递归(Normal)：";
	cout << endl << "前序遍历："; tree.PreOrderNoRecursion2();
	cout << endl << "中序遍历："; tree.InOrderNoRecursion2();
	cout << endl << "后序遍历："; tree.PostOrderNoRecursion2();
	cout << endl << "层序遍历："; tree.LevelOrder();
	cout << endl << "非递归(IP)：";
	cout << endl << "前序遍历："; tree.PreOrderNoRecursion();
	cout << endl << "中序遍历："; tree.InOrderNoRecursion();
	cout << endl << "后序遍历："; tree.PostOrderNoRecursion();
	cout << endl;
	cout << endl;
}

void PrintCountAndHeight(BinTree& tree)
{
	cout << "结点数：" << tree.Count() << endl;
	cout << "高度：" << tree.Height() << endl;
	cout << endl;
}

void BSTreeOper()
{
	system("cls");
	cout << "平衡二叉树" << endl << endl;
	cout << "1. 输入数据" << endl;
	cout << "2. 删除数据" << endl;
	cout << "3. 打印树形图" << endl;
	cout << "4. 随机初始化数据" << endl;
	cout << "5. 随机初始化数据" << endl;
	cout << "0. 返回" << endl;
	cout << endl;

	AVL avl;

	while (true)
	{
		cout << "输入序号：";
		try
		{
			char buf[3];
			cin.getline(buf, 3);
			cin.sync();

			switch (buf[0])
			{
			case '1': BSTreeOper(avl, 1); break;
			case '2': BSTreeOper(avl, 2); break;
			case '3': PrintStructure(avl); break;
			case '4': BSTreeOper(avl, 3); break;
			case '5': BSTreeOper(avl, 4); break;
			case '0': system("cls"); PrintMenu(); return;
			default: throw "请输入正确的序号！";
			}
		}
		catch (const char *pstr)
		{
			cerr << pstr << endl << endl;
		}

		cout << endl;
	}
}

void BSTreeOper(AVL& avl, int type)
{
	if (type == 3)
	{
		int a[] = { 50, 20, 89, 19, 21, 80, 90, 22, 77, 88, 91, 87 };
		for (int i = 0; i < sizeof(a) / sizeof(int); i++)
		{
			avl.Insert(a[i]);
			PrintStructure(avl);
		}
		return;
	}

	if (type == 4)
	{
		int a[] = { 5, 4, 6, 3, 7, 2, 8, 1, 9 };
		for (int i = 0; i < sizeof(a) / sizeof(int); i++)
		{
			avl.Insert(a[i]);
			PrintStructure(avl);
		}
		return;
	}

	cout << "输入数据，以非数字结尾： ";

	int number;

	try
	{
		while (true)
		{
			cin >> number;
			if (cin.fail()) throw "输入完毕。";
			if (cin.bad())  throw "出现致命错误。";
			if (cin.eof())  throw "到达流末尾。";

			if (type == 1)
			{
				avl.Insert(number);
			}
			else if (type == 2)
			{
				avl.Delete(number);
			}
			PrintStructure(avl);
		}
	}
	catch (const char* pstr)
	{
		cerr << pstr << endl << endl;
		cin.clear();
		cin.sync();
		char buf[255];
		cin.getline(buf, 255);
		cin.sync();
		return;
	}
}