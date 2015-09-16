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
	cout << "����ͼ��" << endl;
	tree.Print();
	cout << endl;
}

void BSTreeOper();
void BSTreeOper(AVL& avl, int type);

void PrintMenu()
{
	cout << "1. ��չǰ׺����" << endl;
	cout << "2. ǰ׺����׺����" << endl;
	cout << "3. ����" << endl;
	cout << "4. ��ӡ����ͼ" << endl;
	cout << "5. ��ӡ�߶Ⱥͽ����" << endl;
	cout << "6. ƽ�������" << endl;
	cout << "0. �˳�" << endl;
	cout << endl;
}

int main(int argc, char* argv[])
{
	PrintMenu();

	AVL avl;
	BinTree tree;

	while (true)
	{
		cout << "������ţ�";
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
			default: throw "��������ȷ����ţ�";
			}
		}
		catch (const char *pstr)
		{
			cerr << "���� " << pstr << endl << endl;
		}

		cout << endl;
	}
	return 0;
}

void CreateByPre(BinTree& tree)
{
	cout << "��������չǰ׺��" << endl;
	char buf[255];
	cin.getline(buf, 254);
	string pre = buf;
	tree.CreateByPre(pre);
}

void CreateByPreMid(BinTree& tree)
{
	cout << "������ǰ׺��" << endl;
	char buf[255];
	cin.getline(buf, 254);
	string pre = buf;
	cout << "��������׺��" << endl;
	cin.getline(buf, 254);
	string mid = buf;
	tree.CreateByPreMid(pre, mid);
}

void PrintOrders(BinTree& tree)
{
	cout << "�ݹ飺";
	cout << endl << "ǰ�������"; tree.PreOrder();
	cout << endl << "���������"; tree.InOrder();
	cout << endl << "���������"; tree.PostOrder();
	cout << endl << "�ǵݹ�(Normal)��";
	cout << endl << "ǰ�������"; tree.PreOrderNoRecursion2();
	cout << endl << "���������"; tree.InOrderNoRecursion2();
	cout << endl << "���������"; tree.PostOrderNoRecursion2();
	cout << endl << "���������"; tree.LevelOrder();
	cout << endl << "�ǵݹ�(IP)��";
	cout << endl << "ǰ�������"; tree.PreOrderNoRecursion();
	cout << endl << "���������"; tree.InOrderNoRecursion();
	cout << endl << "���������"; tree.PostOrderNoRecursion();
	cout << endl;
	cout << endl;
}

void PrintCountAndHeight(BinTree& tree)
{
	cout << "�������" << tree.Count() << endl;
	cout << "�߶ȣ�" << tree.Height() << endl;
	cout << endl;
}

void BSTreeOper()
{
	system("cls");
	cout << "ƽ�������" << endl << endl;
	cout << "1. ��������" << endl;
	cout << "2. ɾ������" << endl;
	cout << "3. ��ӡ����ͼ" << endl;
	cout << "4. �����ʼ������" << endl;
	cout << "5. �����ʼ������" << endl;
	cout << "0. ����" << endl;
	cout << endl;

	AVL avl;

	while (true)
	{
		cout << "������ţ�";
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
			default: throw "��������ȷ����ţ�";
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

	cout << "�������ݣ��Է����ֽ�β�� ";

	int number;

	try
	{
		while (true)
		{
			cin >> number;
			if (cin.fail()) throw "������ϡ�";
			if (cin.bad())  throw "������������";
			if (cin.eof())  throw "������ĩβ��";

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