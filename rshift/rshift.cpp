#include "stdafx.h"
#include <stdio.h>


/************************************************************************/
/* ����ѭ�������㷨                                                     */
/************************************************************************/

/*
 * Ҫ��ֻ��һ��Ԫ�ش�С�ĸ����ռ䣬��ʱ�临�Ӷ�ΪO(n)
 */

//************************************
// Method:    �����Լ����շת�����
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
// Method:    ѭ�����ƽⷨһ��ͨ�׽ⷨ��
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
	//������ѭ���ƶ���������
	//��length=m,shift=n
	//��[0] -> [n%m] -> [2n%m] -> ... -> [mn%m]=[0]=��ʼ
	//����kn%m==0����m=gcd*X��n=gcd*Y����X��Y����
	//��k*Y%X==0��Y��X���⣬��X������k����k=m/gcd
	//k��һ�������ĳ��ȣ�������������Ϊm/k=gcd
	int least_movement = gcd(length, shift);//����ѭ���ƶ���������
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
// Method:    ѭ�����ƽⷨ������ת�ⷨ��
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
	//�������㷨
	//��length=m,shift=n(n=n%m)
	//�������ָ����� => array1[0 .. n-1] | array2[ n .. m-1 ]
	//array1��ת��array2��ת�������巭ת
	//
	//ԭ�ȣ�     1,2,3,4,..,n-1     | n,...,m-2,m-1
	//���Է�ת�� n-1,n-2,...,2,1    | m-1,m-2,...,n
	//ȫ����ת�� n,n+1,...,m-2,m-1  | 1,2,3,...,n-2,n-1
	//������������n��λ������
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

