#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <vector>

/************************************************************************/
/*     线段树/区间树                                                    */
/*     Interval Tree                                                    */
/************************************************************************/

typedef double TreeKeyType;

struct tree_node
{
	unsigned char type[4];
	TreeKeyType key[2];
	void* value[3];
};

#define TREE_NODE_UNUSED    0
#define TREE_NODE_VALUE     1
#define TREE_NODE_POINTER   2

#define TREE_NOT_FOUND		-1

#define TREE_PRINT_BLANK	0
#define TREE_PRINT_TRUNK	1
#define TREE_PRINT_BRANCH	2
#define TREE_PRINT_BRANCHD	3

void* tree_init_recursive(TreeKeyType* s, int count, int start, unsigned char* type)
{
	tree_node* node;

	if (count == 0)
	{
		if (type)
			*type = TREE_NODE_VALUE;
		return (void*)start;
	}

	node = new(tree_node);
	if (type)
		*type = TREE_NODE_POINTER;

	switch (count)
	{
	case 1:
	{
		node->type[0] = TREE_NODE_VALUE;
		node->type[1] = TREE_NODE_VALUE;
		node->type[2] = TREE_NODE_UNUSED;
		node->key[0] = s[0];
		node->value[0] = (void*)(start);
		node->value[1] = (void*)(start + 1);
	}
	break;
	case 2:
	{
		node->type[0] = TREE_NODE_VALUE;
		node->type[1] = TREE_NODE_VALUE;
		node->type[2] = TREE_NODE_VALUE;
		node->key[0] = s[0];
		node->key[1] = s[1];
		node->value[0] = (void*)(start);
		node->value[1] = (void*)(start + 1);
		node->value[2] = (void*)(start + 2);
	}
	break;
	default:
	{
		int a = count / 3;
		switch (count % 3)
		{
		case 0:
		{
			node->key[0] = s[a - 1];
			node->key[1] = s[a * 2];
			node->value[0] = tree_init_recursive(&s[0], a - 1, start, &node->type[0]);
			node->value[1] = tree_init_recursive(&s[a], a, start + a, &node->type[1]);
			node->value[2] = tree_init_recursive(&s[a * 2 + 1], a - 1, start + a * 2 + 1, &node->type[2]);
		}
		break;
		case 1:
		{
			node->key[0] = s[a - 1];
			node->key[1] = s[a * 2 + 1];
			node->value[0] = tree_init_recursive(&s[0], a - 1, start, &node->type[0]);
			node->value[1] = tree_init_recursive(&s[a], a + 1, start + a, &node->type[1]);
			node->value[2] = tree_init_recursive(&s[a * 2 + 2], a - 1, start + a * 2 + 2, &node->type[2]);
		}
		break;
		case 2:
		{
			node->key[0] = s[a];
			node->key[1] = s[a * 2 + 1];
			node->value[0] = tree_init_recursive(&s[0], a, start, &node->type[0]);
			node->value[1] = tree_init_recursive(&s[a + 1], a, start + a + 1, &node->type[1]);
			node->value[2] = tree_init_recursive(&s[a * 2 + 2], a, start + a * 2 + 2, &node->type[2]);
		}
		break;
		}
	}
	}

	return (void*)node;
}

tree_node* tree_init(TreeKeyType* s, int count, int start)
{
	return (tree_node*)tree_init_recursive(s, count, start, NULL);
}

int tree_find_recursive(tree_node* node, TreeKeyType s)
{
	if (s < node->key[0])
	{
		if (node->type[0] == TREE_NODE_VALUE)
			return (int)node->value[0];
		else if (node->type[0] == TREE_NODE_POINTER)
			return (int)tree_find_recursive((tree_node*)node->value[0], s);
		else
			return TREE_NOT_FOUND;
	}
	if (node->type[2] == TREE_NODE_UNUSED || s < node->key[1])
	{
		if (node->type[1] == TREE_NODE_VALUE)
			return (int)node->value[1];
		else if (node->type[1] == TREE_NODE_POINTER)
			return (int)tree_find_recursive((tree_node*)node->value[1], s);
		else
			return TREE_NOT_FOUND;
	}
	{
		if (node->type[2] == TREE_NODE_VALUE)
			return (int)node->value[2];
		else if (node->type[2] == TREE_NODE_POINTER)
			return (int)tree_find_recursive((tree_node*)node->value[2], s);
		else
			return TREE_NOT_FOUND;
	}
}

int tree_find(tree_node* node, TreeKeyType s)
{
	if (!node) return TREE_NOT_FOUND;
	return tree_find_recursive(node, s);
}

int tree_size_recursive(tree_node* node)
{
	int size = 1;
	if (node->type[0] == TREE_NODE_POINTER)
		size += tree_size_recursive((tree_node*)node->value[0]);
	if (node->type[1] == TREE_NODE_POINTER)
		size += tree_size_recursive((tree_node*)node->value[1]);
	if (node->type[2] == TREE_NODE_POINTER)
		size += tree_size_recursive((tree_node*)node->value[2]);
	return size;
}

int tree_size(tree_node* node)
{
	if (!node) return TREE_NOT_FOUND;
	return tree_size_recursive(node);
}

void tree_destroy_recursive(tree_node* node)
{
	if (node->type[0] == TREE_NODE_POINTER)
		tree_destroy_recursive((tree_node*)node->value[0]);
	if (node->type[1] == TREE_NODE_POINTER)
		tree_destroy_recursive((tree_node*)node->value[1]);
	if (node->type[2] == TREE_NODE_POINTER)
		tree_destroy_recursive((tree_node*)node->value[2]);
	delete (node);
}

void tree_destroy(tree_node* node)
{
	if (!node) return;
	tree_destroy_recursive(node);
}

void tree_print_helper(const std::vector<int>& mark)
{
	for (auto m : mark)
	{
		switch (m)
		{
		case TREE_PRINT_BLANK:
			printf("        ");
			break;
		case TREE_PRINT_TRUNK:
			printf("│      ");
			break;
		case TREE_PRINT_BRANCH:
			printf("├───");
			break;
		case TREE_PRINT_BRANCHD:
			printf("└───");
			break;
		default:
			break;
		}		
	}	
}

void tree_print_recursive(tree_node* node, std::vector<int>& mark)
{
	if (node == NULL) return;
	tree_print_helper(mark);
	int last_branch = node->type[2] != TREE_NODE_UNUSED ? 2 :
		node->type[1] != TREE_NODE_UNUSED ? 1 : 0;
	if (last_branch == 2)
		printf("<%f, %f>\n", node->key[0], node->key[1]);
	else
		printf("<%f>\n", node->key[0]);
	int last_mark = *mark.rbegin();
	if (last_mark != TREE_PRINT_BLANK)
	{
		mark.pop_back();
		mark.push_back(last_mark == TREE_PRINT_BRANCHD ? TREE_PRINT_BLANK : TREE_PRINT_TRUNK);
	}
	for (int i = 0; i <= last_branch; i++)
	{
		if (last_branch == i)
		{
			mark.push_back(TREE_PRINT_BRANCHD);
		}
		else
		{
			mark.push_back(TREE_PRINT_BRANCH);
		}
		if (node->type[i] == TREE_NODE_POINTER)
		{
			tree_print_recursive((tree_node*)node->value[i], mark);
		}
		else if (node->type[i] == TREE_NODE_VALUE)
		{
			tree_print_helper(mark);
			printf("<%d>\n", (int)node->value[i]);
		}
		if (last_branch != i)
		{
			mark.pop_back();
		}
	}
	mark.pop_back();
}

void tree_print(tree_node* node)
{
	if (!node) return;
	std::vector<int> mark;
	mark.push_back(TREE_PRINT_BLANK);
	tree_print_recursive(node, mark);
}

int main(int argc, char* argv[])
{
	//要求：对第i的页面的访问概率正比于1/sqrt(i+1)
	const int count = 12;
	TreeKeyType* sum = new TreeKeyType[count];
	sum[0] = 1;
	//sum[0]=1
	//sum[1]=sum[0]+1/sqrt(2)
	//sum[2]=sum[1]+1/sqrt(3)
	//...
	//sum[n-1]=sum[n-2]+1/sqrt(n)
	for (int i = 1; i < count; i++)
	{
		sum[i] = sum[i - 1] + (double)(1 / sqrt(i + 1));
	}
	TreeKeyType MaxRandValue = sum[count - 1] - 0.00001;
	tree_node* search_node = tree_init(sum, count, 0);
	printf("Search node size: %d\n", tree_size(search_node) * sizeof(search_node));
	printf("========== tree ==========\n");
	tree_print(search_node);
	delete[] (sum);
// 	printf("a: %s\n", str1);
// 	printf("b: %s\n", str2);
// 	printf("========= lcseq ==========\n");
// 	lcseq(str1, str2);
// 	printf("\n");
// 	printf("========= lcstr ==========\n");
// 	lcstr(str1, str2);
// 	printf("\n");
	return 0;
}

