#include "stdafx.h"
#include <stdio.h>


/************************************************************************/
/* 数组循环右移算法                                                     */
/************************************************************************/

/*
 * 要求：只用一个元素大小的辅助空间，且时间复杂度为O(n)
 */

//************************************
// Method:    求最大公约数（辗转相除）
// FullName:  gcd
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: int m
// Parameter: int n
//************************************
int gcd(int m, int n)
{
	return n ? gcd(n, m % n) : m;
}

//************************************
// Method:    循环右移解法一（通俗解法）
// FullName:  rshift1
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int array[]
// Parameter: int length
// Parameter: int shift
//************************************
void rshift1(int array[], int length, int shift)
{
	//求最少循环移动链的数量
	//设length=m,shift=n
	//则[0] -> [n%m] -> [2n%m] -> ... -> [mn%m]=[0]=起始
	//假设kn%m==0，令m=gcd*X，n=gcd*Y，则X与Y互斥
	//则k*Y%X==0，Y与X互斥，故X能整除k，故k=m/gcd
	//k是一条链条的长度，故链条的数量为m/k=gcd
	int least_movement = gcd(length, shift);//最少循环移动链的数量
	int i;
	for (i = 0; i < least_movement; i++)
	{
		int swap_a = i;
		int swap_b = (i + shift) % length;
		int tmp = array[swap_a];
		while (swap_b != i)
		{
			array[swap_a] = array[swap_b];
			swap_a = swap_b;
			swap_b = (swap_b + shift) % length;
		}
		array[swap_a] = tmp;
	}
}

//************************************
// Method:    循环右移解法二（翻转解法）
// FullName:  rshift2
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int array[]
// Parameter: int length
// Parameter: int shift
//************************************
void rshift2(int array[], int length, int shift)
{
	//翻手掌算法
	//设length=m,shift=n(n=n%m)
	//方法：分割数组 => array1[0 .. n-1] | array2[ n .. m-1 ]
	//array1翻转，array2翻转，再整体翻转
	//
	//原先：     1,2,3,4,..,n-1     | n,...,m-2,m-1
	//各自翻转： n-1,n-2,...,2,1    | m-1,m-2,...,n
	//全部翻转： n,n+1,...,m-2,m-1  | 1,2,3,...,n-2,n-1
	//这就完成了右移n单位的任务
	int i;
	shift %= length;
	int tmp;
	for (i = 0; i < (shift - 1) / 2; i++)
	{
		tmp = array[i];
		array[i] = array[shift - i - 1];
		array[shift - i - 1] = tmp;
	}
	for (i = shift; i < shift + (length - shift) / 2; i++)
	{
		tmp = array[i];
		array[i] = array[length + shift - i - 1];
		array[length + shift - i - 1] = tmp;
	}
	for (i = 0; i < length / 2; i++)
	{
		tmp = array[i];
		array[i] = array[length - i - 1];
		array[length - i - 1] = tmp;
	}
}

void print_array(int array[], int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	int a[] = { 1,2,3,4,5,6 };
	printf("=====================\n");
	print_array(a, 6);
	printf("========== rshift ==========\n");
	rshift1(a, 6, 3);
	print_array(a, 6);
	printf("========== rshift ==========\n");
	rshift2(a, 6, 3);
	print_array(a, 6);
	return 0;
}

