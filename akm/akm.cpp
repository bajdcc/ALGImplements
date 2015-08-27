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
 // FullName:  递归解法
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
// FullName:  非递归解法一（书上解法）
// Access:    public 
// Returns:   int
// Qualifier: non-recursion
// Parameter: int m
// Parameter: int n
//************************************
int akm2(int m, int n)
{
	//栈的惰性求值方法，若表达式中含有非求值akm函数，则记录它在链表中的位置
	//由于存在m和n参数以及akm函数返回值，因此一个结点中有m,n,val
	struct akm_node
	{
		int m, n, val;
	};
	std::stack<akm_node> exp_stack;
	akm_node initial_val;//初始值
	initial_val.m = m;
	initial_val.n = n;
	initial_val.val = -1;//返回值尚未求得
	exp_stack.push(initial_val);
	/* 注意：这个方法假设akm的值为正数 */
	while (true)//主循环
	{
		akm_node& node = exp_stack.top();//当前处理的表达式，取栈顶且不出栈，取引用说明要修改
		printf("Compute akm(%d,%d)\n", node.m, node.n);
		//为什么为top不急着出栈，这是因为之前的表达式要用到当前值
		//此时出栈后，之前的式子就没有获得返回值
		//出栈的最佳时机，即为式子取得其参数（返回值）后
		if (node.val != -1)//值被求出
		{
			//什么时候才是出栈的时机，这个时候！
			//此时，值已经求得，即为可解问题
			//我们知道可解问题一般为树的叶子结点
			//求得这个值之后，可能这个结果是上一次调用的参数
			//那么应该亡羊补牢，赶紧补上上一次调用的参数
			exp_stack.pop();//值在node中，故可出栈
			if (exp_stack.empty())//Rule I
			{
				//假使当前栈空，那么这个node就是最终值（所有嵌套全部计算完毕），九九归一
				return node.val;
			}
			else
			{
				//栈不空，说明【肯定】含有待解问题
				//因为当且仅当栈空时，调用栈为空，即树归约到根结点，得到最终结果
				akm_node& not_computed_exp = exp_stack.top();//取引用说明要修改
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
			//此时值不可直接求解，故需要嵌套求值（惰性求值）
			//将二次调用参数进栈
			akm_node secondary_call_exp;
			secondary_call_exp.m = node.m - 1;
			secondary_call_exp.n = 1;
			secondary_call_exp.val = -1;
			exp_stack.push(secondary_call_exp);
		}
		else//Rule III akm(m-1,akm(m,n-1)) otherwise		
		{
			//此时值不可直接求解，故需要嵌套求值（惰性求值）
			akm_node secondary_call_exp;//二次调用
			secondary_call_exp.m = node.m - 1;
			secondary_call_exp.n = -1;//-1代表未知
			secondary_call_exp.val = -1;
			exp_stack.push(secondary_call_exp);
			akm_node tertiary_call_exp;//三次调用
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
	printf("============ 递归 ===========\n");
	printf("akm(2,1)=%d\n", akm1(2, 1));
	printf("\n");
	printf("============ 非递归方法一 ===========\n");
	printf("akm(2,1)=%d\n", akm2(2, 1));
	printf("\n");
	printf("============ 非递归方法二 ===========\n");
	printf("akm(2,1)=%d\n", akm2(2, 1));
	printf("\n");
	return 0;
}

