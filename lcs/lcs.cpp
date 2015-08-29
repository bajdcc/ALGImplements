#include "stdafx.h"
#include <stdio.h>
#include <stack>

/************************************************************************/
/* ����������� / ������Ӵ�                                        */
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
 * > �Ӵ��Ǵ���һ�������Ĳ���
 * > ���������ǲ��ı����е�˳�򣬶���������ȥ�������Ԫ�ض�����µ�����
 * > ��Ҳ����˵���Ӵ����ַ���λ�ñ����������ģ�����������Բ�����������
 */

//************************************
// Method:    ����������У�DP��̬�滮��
// FullName:  lcseq
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char * x
// Parameter: char * y
//************************************
void lcseq(char *x, char *y)
{
	//��x��X[ 0 .. m-1 ] x.length=m
	//��y��Y[ 0 .. n-1 ] y.length=n
	//��z��Z[ 0 .. k-1 ] z.length=k, z = lcseq(x,y)

	//��z��x��y������������С�
	//
	//================ ���Ʒ��� ================
	//�����һλ����
	//
	// >>> ��һ�������
	// *     ��X[last]==Y[last]����z[last]=X[last]=Y[last]
	// *     ��lcseq(x,y) = lcseq(x-1,y-1) + (x[last] or y[last])
	//
	// >>> �ڶ��������
	// *     ��X[last]<>Y[last]�����ǣ�
	// *     1) ��Z[last]==X[last]����lcseq(x,y) = lcseq(x,y-1)
	// *     2) ��Z[last]==Y[last]����lcseq(x,y) = lcseq(x-1,y)
	// *     3) ����lcseq(x,y) = max_length{lcseq(x,y-1), lcseq(x-1,y)}
	// *     �ۺϵ�lcseq(x,y) = max_length{lcseq(x,y-1), lcseq(x-1,y)}
	//
	//ͼ��չʾ��
	//     �����������������ש��������������� 
	//     ��  Z(i-1,j-1)  ��   Z(i-1,j)   ��
	//     �ǩ��������������贈��������������
	//     ��  Z(i,j-1)    ��   Z(i,j)     ��
	//     �����������������ߩ���������������
	//
	//================ �ܽ���ƹ�ʽ ================
	//     ����֪Z(i-1,j-1)��Z(i-1,j)��Z(i,j-1)����Z(i,j)��
	//     ��ʼ��
	//     1) Z(0,0)=0
	//     2) Z(0,~)=0
	//     3) Z(~,0)=0
	//     ���ƣ�
	//     1) X[i] == Y[j] ==> Z(i,j) = Z(i-1,j-1) + 1
	//     2) X[i] <> Y[j] ==> Z(i,j) = max{Z(x,y-1), Z(x-1,y)}

	int m, n, i, j;
	int **zmat, *zmat0;
	int **pmat, *pmat0;
	//���㳤��
	m = length(x);
	n = length(y);
	//��������Z(i,j)����ֵ�Ķ�ά���飬������Ϊ{(i,j)|i in [0..m] and j in [0..n]}
	zmat = new int*[m + 1];//ָ������
	zmat0 = new int[(m + 1) * (n + 1)];//m+1��n+1�У�һά���齨��
	for (i = 0; i <= m; i++) zmat[i] = &zmat0[i * (n+1)];
	for (i = 0; i <= m; i++) zmat[i][0] = 0;//i��0����0
	for (j = 1; j <= n; j++) zmat[0][j] = 0;//0��j����0
	//�����������飬����Z(i,j)��ȡֵ���򣨵�һ�����Ϊ���Ͻ�ȡֵ���ڶ������Ϊ�ϱ߻����ȡֵ��
	pmat = new int*[m];//ָ������
	pmat0 = new int[m * n];//m��n�У�һά���齨��
	for (i = 0; i < m; i++) pmat[i] = &pmat0[i * n];
	//���ʣ���Z����
	for (i = 1; i <= m; i++)
	{
		for (j = 1; j <= n; j++)//ע��i,j��Χ
		{
			if (x[i - 1] == y[j - 1])//X[i] == Y[j] ==> Z(i,j) = Z(i-1,j-1) + 1
			{
				zmat[i][j] = zmat[i - 1][j - 1] + 1;
				pmat[i - 1][j - 1] = 1;//���Ͻ�ȡֵ·��
			}
			else//X[i] <> Y[j] ==> Z(i,j) = max{Z(x,y-1), Z(x-1,y)}
			{
				int sub;
				sub = zmat[i - 1][j] - zmat[i][j - 1];
				if (sub >= 0)
				{
					zmat[i][j] = zmat[i - 1][j];
					pmat[i - 1][j - 1] = sub == 0 ? 4 : 2;//�����ȡֵ·��
				}
				else
				{
					zmat[i][j] = zmat[i][j - 1];
					pmat[i - 1][j - 1] = 3;//���ȡֵ·��
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

