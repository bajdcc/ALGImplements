#include "stdafx.h"
#include <stdio.h>
#include <stack>

/************************************************************************/
/* 最长公共子序列 / 最长公共子串                                        */
/*     lcseq      /     lcstr                                           */
/************************************************************************/

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

	int m, n, i, j;
	int **zmat, *zmat0;
	int **pmat, *pmat0;
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
	delete[] zmat;
	delete[] zmat0;
	delete[] pmat;
	delete[] pmat0;
}

void lcstr(char *x, char *y)
{

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

