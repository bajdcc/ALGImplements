#include "stdafx.h"
#include <stdio.h>
#include <stack>

/************************************************************************/
/* 前缀转后缀                                                           */
/************************************************************************/

bool is_digitoralpha(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//************************************
// Method:    递归转换方法
// FullName:  pretopost1
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * str
//************************************
char* pretopost1(char *str)
{
	char op = *str++;//第一位是父结点
	if (is_digitoralpha(*str))
		printf("%c", *str++);//左孩子是操作数，直接打印
	else
		str = pretopost1(str);//递归处理左子树
	if (is_digitoralpha(*str))
		printf("%c", *str++);//右孩子是操作数，直接打印
	else
		str = pretopost1(str);//递归处理右子树
	printf("%c", op);//最后打印父结点
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
// Method:    构造表达式树方法
// FullName:  pretopost2
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * str
//************************************
void pretopost2(char *str)
{
	std::stack<node*> stk;//存放树结点的栈
	node *top, *root;
	top = root = NULL;
	while (*str)
	{
		bool num;//是否是操作数
		char c;
		c = *str;
		if (!stk.empty())
		{
			top = stk.top();
			if (top->left && top->right)//如果当前结点的左右子树均满，则弹出此结点
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
			if (!top->left || !top->right)//建立新的结点，将其安插在top的孩子上
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
			if (!num)//如果是操作符，则变更当前top结点，使其指向新结点（操作符）
			{
				stk.push(newnode);
			}
		}
		else
		{
			top = new node();//如果top是空，意味着栈为空，则初始化
			top->data = c;
			top->left = NULL;
			top->right = NULL;
			root = top;//放置根结点
			stk.push(top);
		}
	}
	print_post(root);
	delete_tree(root);//递归删除
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

