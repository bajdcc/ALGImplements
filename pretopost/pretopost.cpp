#include "stdafx.h"
#include <stdio.h>
#include <stack>

/************************************************************************/
/* ǰ׺ת��׺                                                           */
/************************************************************************/

bool is_digitoralpha(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//************************************
// Method:    �ݹ�ת������
// FullName:  pretopost1
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * str
//************************************
char* pretopost1(char *str)
{
	char op = *str++;//��һλ�Ǹ����
	if (is_digitoralpha(*str))
		printf("%c", *str++);//�����ǲ�������ֱ�Ӵ�ӡ
	else
		str = pretopost1(str);//�ݹ鴦��������
	if (is_digitoralpha(*str))
		printf("%c", *str++);//�Һ����ǲ�������ֱ�Ӵ�ӡ
	else
		str = pretopost1(str);//�ݹ鴦��������
	printf("%c", op);//����ӡ�����
	return str;
}

struct node
{
	char data;
	node *left, *right;
};

void print_post(node *pnode)
{
	if (pnode)
	{
		if (pnode->left)
		{
			print_post(pnode->left);
		}
		if (pnode->right)
		{
			print_post(pnode->right);
		}
		printf("%c", pnode->data);
	}
}

void delete_tree(node *pnode)
{
	if (pnode)
	{
		if (pnode->left)
		{
			delete_tree(pnode->left);
		}
		if (pnode->right)
		{
			delete_tree(pnode->right);
		}
		delete pnode;
	}
}

//************************************
// Method:    ������ʽ������
// FullName:  pretopost2
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * str
//************************************
void pretopost2(char *str)
{
	std::stack<node*> stk;//���������ջ
	node *top, *root;
	top = root = NULL;
	while (*str)
	{
		bool num;//�Ƿ��ǲ�����
		char c;
		c = *str;
		if (!stk.empty())
		{
			top = stk.top();
			if (top->left && top->right)//�����ǰ�������������������򵯳��˽��
			{
				stk.pop();
				continue;
			}
		}
		str++;
		num = is_digitoralpha(c);
		if (num || top)
		{
			node *newnode;
			if (!top->left || !top->right)//�����µĽ�㣬���䰲����top�ĺ�����
			{
				newnode = new node();
				newnode->data = c;
				newnode->left = NULL;
				newnode->right = NULL;
				if (!top->left)
					top->left = newnode;
				else
					top->right = newnode;
			}			
			if (!num)//����ǲ�������������ǰtop��㣬ʹ��ָ���½�㣨��������
			{
				stk.push(newnode);
			}
		}
		else
		{
			top = new node();//���top�ǿգ���ζ��ջΪ�գ����ʼ��
			top->data = c;
			top->left = NULL;
			top->right = NULL;
			root = top;//���ø����
			stk.push(top);
		}
	}
	print_post(root);
	delete_tree(root);//�ݹ�ɾ��
}

int main(int argc, char* argv[])
{
	char *pre = "+*ab*-c/def";
	printf("pre: %s\n", pre);
	printf("====================\n");
	printf("post: ");
	pretopost1(pre);
	printf("\n");
	printf("====================\n");
	printf("post: ");
	pretopost2(pre);
	printf("\n");
	return 0;
}

