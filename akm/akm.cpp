#include "stdafx.h"
#include <stdio.h>
#include <stack>

/*
 * Ackerman function definition
 *
 *           {  n+1                    while m=0  } => Rule I
 * akm(m,n)= {  akm(m-1,1)             while n=0  } => Rule II
 *           {  akm(m-1,akm(m,n-1))    otherwise  } => Rule III
 */

 //************************************
 // Method:    akm1
 // FullName:  �ݹ�ⷨ
 // Access:    public 
 // Returns:   int
 // Qualifier: recursion
 // Parameter: int m
 // Parameter: int n
 //************************************
int akm1(int m, int n)
{
	printf("Compute akm(%d,%d)\n", m, n);
	if (m == 0)
	{
		printf("Compute akm(%d,%d) - val = %d\n", m, n, n + 1);
		return n + 1;
	}
	else if (n == 0)
	{
		return akm1(m - 1, 1);
	}
	else
	{
		return akm1(m - 1, akm1(m, n - 1));
	}
}

//************************************
// Method:    akm2
// FullName:  �ǵݹ�ⷨһ�����Ͻⷨ��
// Access:    public 
// Returns:   int
// Qualifier: non-recursion
// Parameter: int m
// Parameter: int n
//************************************
int akm2(int m, int n)
{
	//ջ�Ķ�����ֵ�����������ʽ�к��з���ֵakm���������¼���������е�λ��
	//���ڴ���m��n�����Լ�akm��������ֵ�����һ���������m,n,val
	struct akm_node
	{
		int m, n, val;
	};
	std::stack<akm_node> exp_stack;
	akm_node initial_val;//��ʼֵ
	initial_val.m = m;
	initial_val.n = n;
	initial_val.val = -1;//����ֵ��δ���
	exp_stack.push(initial_val);
	/* ע�⣺�����������akm��ֵΪ���� */
	while (true)//��ѭ��
	{
		akm_node& node = exp_stack.top();//��ǰ����ı��ʽ��ȡջ���Ҳ���ջ��ȡ����˵��Ҫ�޸�
		printf("Compute akm(%d,%d)\n", node.m, node.n);
		//ΪʲôΪtop�����ų�ջ��������Ϊ֮ǰ�ı��ʽҪ�õ���ǰֵ
		//��ʱ��ջ��֮ǰ��ʽ�Ӿ�û�л�÷���ֵ
		//��ջ�����ʱ������Ϊʽ��ȡ�������������ֵ����
		if (node.val != -1)//ֵ�����
		{
			//ʲôʱ����ǳ�ջ��ʱ�������ʱ��
			//��ʱ��ֵ�Ѿ���ã���Ϊ�ɽ�����
			//����֪���ɽ�����һ��Ϊ����Ҷ�ӽ��
			//������ֵ֮�󣬿�������������һ�ε��õĲ���
			//��ôӦ�������Σ��Ͻ�������һ�ε��õĲ���
			exp_stack.pop();//ֵ��node�У��ʿɳ�ջ
			if (exp_stack.empty())//Rule I
			{
				//��ʹ��ǰջ�գ���ô���node��������ֵ������Ƕ��ȫ��������ϣ����žŹ�һ
				return node.val;
			}
			else
			{
				//ջ���գ�˵�����϶������д�������
				//��Ϊ���ҽ���ջ��ʱ������ջΪ�գ�������Լ������㣬�õ����ս��
				akm_node& not_computed_exp = exp_stack.top();//ȡ����˵��Ҫ�޸�
				if (not_computed_exp.n == 1)//Rule II called Rule I
				{
					not_computed_exp.val = node.val;//give it computed value
				}
				else if (not_computed_exp.n == -1)//Rule III called Rule I
				{
					not_computed_exp.n = node.val;
				}
				else
				{
					not_computed_exp.val = node.val;
				}
			}
			continue;
		}
		if (node.m == 0)//Rule I: m+1 while m equals to zero
		{
			printf("Compute akm(%d,%d) - val = %d\n", node.m, node.n, node.n + 1);
			node.val = node.n + 1;
		}
		else if (node.n == 0)//Rule II akm(m-1,1) while n equals to zero
		{
			//��ʱֵ����ֱ����⣬����ҪǶ����ֵ��������ֵ��
			//�����ε��ò�����ջ
			akm_node secondary_call_exp;
			secondary_call_exp.m = node.m - 1;
			secondary_call_exp.n = 1;
			secondary_call_exp.val = -1;
			exp_stack.push(secondary_call_exp);
		}
		else//Rule III akm(m-1,akm(m,n-1)) otherwise		
		{
			//��ʱֵ����ֱ����⣬����ҪǶ����ֵ��������ֵ��
			akm_node secondary_call_exp;//���ε���
			secondary_call_exp.m = node.m - 1;
			secondary_call_exp.n = -1;//-1����δ֪
			secondary_call_exp.val = -1;
			exp_stack.push(secondary_call_exp);
			akm_node tertiary_call_exp;//���ε���
			tertiary_call_exp.m = node.m;
			tertiary_call_exp.n = node.n - 1;
			tertiary_call_exp.val = -1;
			exp_stack.push(tertiary_call_exp);
			continue;
		}
	}
	return 0;
}

int main()
{
	printf("============ �ݹ� ===========\n");
	printf("akm(2,1)=%d\n", akm1(2, 1));
	printf("\n");
	printf("============ �ǵݹ鷽��һ ===========\n");
	printf("akm(2,1)=%d\n", akm2(2, 1));
	printf("\n");
	printf("============ �ǵݹ鷽���� ===========\n");
	printf("akm(2,1)=%d\n", akm2(2, 1));
	printf("\n");
	return 0;
}

