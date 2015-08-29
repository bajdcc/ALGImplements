#include "stdafx.h"
#include <stdio.h>
#include <vector>

/************************************************************************/
/* 最长公共子序列 / 最长公共子串                                        */
/*     lcseq      /     lcstr                                           */
/************************************************************************/

//注意！需要输出多组结果！

int length(char *pstr)
{
	int i;
	i = 0;
	while (*pstr++) i++;
	return i;
}

void print_matrix(int **mat, char *x, char *y, int m0, int n0, int m, int n)
{
	int i, j;
	printf("    ");
	for (j = 0; j < n; j++)
	{
		printf("%-3c", y[j]);
	}
	printf("\n");
	for (i = m0; i < m; i++)
	{
		printf("%-4c", x[i - m0]);
		for (j = n0; j < n; j++)
		{
			printf("%-3d", mat[i][j]);
		}
		printf("\n");
	}
}

/*
 * > 子串是串的一个连续的部分
 * > 子序列则是不改变序列的顺序，而从序列中去掉任意的元素而获得新的序列
 * > 【也就是说，子串中字符的位置必须是连续的，子序列则可以不必连续。】
 */

struct node
{
	int parent;
	char c;
};

//************************************
// Method:    构建二叉树
// FullName:  build_btree
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * x 原始字符串X
// Parameter: int * * mat 跟踪矩阵
// Parameter: int m 串X长度
// Parameter: int n 串Y长度
// Parameter: int parent 父结点ID
// Parameter: std::stack<node> & tree 树
//************************************
void build_btree(char *x, int **mat, int m, int n, int parent, std::vector<node>& tree)
{
	if (m == -1 || n == -1)//跟踪到叶子结点，可以打印
	{
		int i;
		for (i = parent; i != -1; i = tree[i].parent)
		{
			char c;
			c = tree[i].c;
			if (c)
			{
				printf("%c", c);
			}
		}
		printf("\n");
		return;
	}
	if (m == 0 && n == 0)//处理左上角结点的情况Z(0,0)
	{
		build_btree(x, mat, -1, -1, parent, tree);
		return;
	}
	int p = mat[m][n];
	if (mat[m][n] == 0)//防止访问重复结点
		return;
	mat[m][n] = 0;
	node newnode;
	newnode.c = '\0';
	newnode.parent = parent;
	tree.push_back(newnode);
	parent = tree.size() - 1;
	switch (p)
	{
	case 1://左上，相同
		tree.back().c = x[m];
		build_btree(x, mat, m, n - 1, parent, tree);
		break;
	case 2://上
		build_btree(x, mat, m - 1, n, parent, tree);
		break;
	case 3://左
		build_btree(x, mat, m, n - 1, parent, tree);
		break;
	case 4://左和上
		build_btree(x, mat, m, n - 1, parent, tree);
		build_btree(x, mat, m - 1, n, parent, tree);
		break;
	default:
		break;
	}
}

//************************************
// Method:    最长公共子序列（DP动态规划）
// FullName:  lcseq
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * x
// Parameter: char * y
//************************************
void lcseq(char *x, char *y)
{
	//串x：X[ 0 .. m-1 ] x.length=m
	//串y：Y[ 0 .. n-1 ] y.length=n
	//串z：Z[ 0 .. k-1 ] z.length=k, z = lcseq(x,y)

	//【z是x与y的最长公共子序列】
	//
	//================ 递推方法 ================
	//从最后一位看起：
	//
	// >>> 第一种情况：
	// *     若X[last]==Y[last]，则z[last]=X[last]=Y[last]
	// *     则lcseq(x,y) = lcseq(x-1,y-1) + (x[last] or y[last])
	//
	// >>> 第二种情况：
	// *     若X[last]<>Y[last]，考虑：
	// *     1) 若Z[last]==X[last]，则lcseq(x,y) = lcseq(x,y-1)
	// *     2) 若Z[last]==Y[last]，则lcseq(x,y) = lcseq(x-1,y)
	// *     3) 否则，lcseq(x,y) = max_length{lcseq(x,y-1), lcseq(x-1,y)}
	// *     综合得lcseq(x,y) = max_length{lcseq(x,y-1), lcseq(x-1,y)}
	//
	//图表展示：
	//     ┏━━━━━━━┳━━━━━━━┓ 
	//     ┃  Z(i-1,j-1)  ┃   Z(i-1,j)   ┃
	//     ┣━━━━━━━╋━━━━━━━┫
	//     ┃  Z(i,j-1)    ┃   Z(i,j)     ┃
	//     ┗━━━━━━━┻━━━━━━━┛
	//
	//================ 总结递推公式 ================
	//     【已知Z(i-1,j-1)、Z(i-1,j)、Z(i,j-1)，求Z(i,j)】
	//     初始：
	//     1) Z(0,0)=0
	//     2) Z(0,~)=0
	//     3) Z(~,0)=0
	//     递推：
	//     1) X[i] == Y[j] ==> Z(i,j) = Z(i-1,j-1) + 1
	//     2) X[i] <> Y[j] ==> Z(i,j) = max{Z(x,y-1), Z(x-1,y)}
	//
	//================ 跟踪输出所有LCS ================
	//由Z(m,n)可以向左上方遍历，所过所有路径即形成二叉树
	//故考虑建立二叉树，实行自叶子向根部的遍历

	int m, n, i, j;
	int **zmat, *zmat0;
	int **pmat, *pmat0;
	std::vector<node> tree;//保存二叉树的数组（栈式存储）
	//计算长度
	m = length(x);
	n = length(y);
	//建立保存Z(i,j)函数值的二维数组，定义域为{(i,j)|i in [0..m] and j in [0..n]}
	zmat = new int*[m + 1];//指针数组
	zmat0 = new int[(m + 1) * (n + 1)];//m+1行n+1列，一维数组建立
	for (i = 0; i <= m; i++) zmat[i] = &zmat0[i * (n+1)];
	for (i = 0; i <= m; i++) zmat[i][0] = 0;//i行0列置0
	for (j = 1; j <= n; j++) zmat[0][j] = 0;//0行j列置0
	//建立跟踪数组，跟踪Z(i,j)的取值方向（第一种情况为左上角取值，第二种情况为上边或左边取值）
	pmat = new int*[m];//指针数组
	pmat0 = new int[m * n];//m行n列，一维数组建立
	for (i = 0; i < m; i++) pmat[i] = &pmat0[i * n];
	//填充剩余的Z数组
	for (i = 1; i <= m; i++)
	{
		for (j = 1; j <= n; j++)//注意i,j范围
		{
			if (x[i - 1] == y[j - 1])//X[i] == Y[j] ==> Z(i,j) = Z(i-1,j-1) + 1
			{
				zmat[i][j] = zmat[i - 1][j - 1] + 1;
				pmat[i - 1][j - 1] = 1;//左上角取值路线
			}
			else//X[i] <> Y[j] ==> Z(i,j) = max{Z(x,y-1), Z(x-1,y)}
			{
				int sub;
				sub = zmat[i - 1][j] - zmat[i][j - 1];
				if (sub >= 0)
				{
					zmat[i][j] = zmat[i - 1][j];
					pmat[i - 1][j - 1] = sub == 0 ? 4 : 2;//左或上取值路线
				}
				else
				{
					zmat[i][j] = zmat[i][j - 1];
					pmat[i - 1][j - 1] = 3;//左边取值路线
				}
			}
		}
	}
	printf("Z matrix\n");
	print_matrix(zmat, x, y, 1, 1, m + 1, n + 1);
	printf("Trace matrix\n");
	print_matrix(pmat, x, y, 0, 0, m, n);

	//跟踪开始
	node root;
	root.parent = -1;
	root.c = '\0';
	tree.push_back(root);
	printf("------------- Result -------------\n");
	build_btree(x, pmat, m - 1, n - 1, 0, tree);
	//清理工作
	delete[] zmat;
	delete[] zmat0;
	delete[] pmat;
	delete[] pmat0;
}


//************************************
// Method:    最长公共子串（DP动态规划）
// FullName:  lcstr
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * x
// Parameter: char * y
//************************************
void lcstr(char *x, char *y)
{
	//以Y作参考串，建立参考数组Z，然后遍历X
	//即：对所有i，存在j，使X[i]==Y[j]，则Z'[j]=Z[j]+1
	//跟踪Z中最大值
	int i, j, m, n, *z, max;
	std::vector<int> str_indexes;//记录符合要求的lcstr起始位置（可能有多个）
	m = length(x);
	n = length(y);//Y作参考串
	z = new int[n + 1];
	for (j = 0; j <= n; j++) z[j] = 0;//初始化
	max = 0;
	printf("    ");
	for (j = 0; j < n; j++)
	{
		printf("%-3c", y[j]);
	}
	printf("\n");
	for (i = 0; i < m; i++)
	{
		char c;
		c = x[i];
		printf("%-4c", c);
		for (j = n - 1; j >= 0; j--)
		{
			if (c == y[j])
			{
				z[j + 1] = z[j] + 1;
				int zj = z[j + 1];
				if (zj > max)
				{
					max = zj;
					str_indexes.clear();
				}
				if (zj == max)
				{
					str_indexes.push_back(j - zj);
				}
			}
			else
			{
				z[j + 1] = 0;
			}
		}
		for (j = 1; j <= n; j++)
		{
			printf("%-3d", z[j]);
		}
		printf("\n");
	}
	printf("------------- Result -------------\n");
	for (auto& v : str_indexes)
	{
		for (j = v; j < max + v; j++)
		{
			printf("%c", y[j]);
		}
		printf("\n");
	}
	delete[] z;
}

int main(int argc, char* argv[])
{
	char *str1 = "BADCDCBA";
	char *str2 = "ABCDCDAB";
	printf("a: %s\n", str1);
	printf("b: %s\n", str2);
	printf("========= lcseq ==========\n");
	lcseq(str1, str2);
	printf("\n");
	printf("========= lcstr ==========\n");
	lcstr(str1, str2);
	printf("\n");
	return 0;
}

