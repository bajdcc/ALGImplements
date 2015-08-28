// KMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <IOSTREAM>
using namespace std;

#define BUF_SIZE 100
#define BUF_SIZE_2 200

/************************************************************************/
/* KMP算法                                                              */
/************************************************************************/

//************************************
// Method:    Compute next array
// FullName:  计算NEXT数组
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char t[] 模式串
// Parameter: int next[] next数组
// Parameter: int length 模式串长度
//************************************
void Calu_Next_Array(char t[], int next[], int length)
{
	int j = 0, k = -1; next[0] = -1;
	//next[0]使得模式串T与主串S各向前前进一步，-1用作特殊标志
	//next数组大小应为模式串T长度
	//j循环从0到length-2，共置n-1次值加上next[0]=-1，一共置n次值
	while (j < length - 1)
	{
		if (k == -1 || t[j] == t[k])
		{
			//假如k=-1，这是第一次情况，且仅会出现一次
			//则当k=-1时，有k=0,j=1,next[1]=0，可知next[1]=0也是恒定不变的
			//由递推公式可知，初始情况：next[0]=-1,next[1]=0

			//*******************  递推公式推导  *******************
			//如果是t[j] == t[k]，则有：
			//【推导：设next[0]...next[j]已经求出，next[j+1]=？】
			//记next[j] = k，则可推出t[0 .. k-1]=t[j-k .. j-1]
			//又因为t[j] == t[k]，则有t[0 .. k]=t[j-k .. j]，推出【next[j+1]=k+1】
			//又因为next[j] = k，故【j++，k++】
			//【结论：若next[0]...next[j]已知，且t[j] == t[k]，则可得next[j+1]=next[j]+1】
			next[++j] = ++k;
		}
		else
		{
			//*******************  递推公式推导  *******************
			//由于t[j] <> t[k]，记next[j] = k，则可推出t[0 .. k-1]=t[j-k .. j-1]
			//由于t[0 .. k-1]=t[j-k .. j-1]，则有t[Y .. k-1]=t[j-k+Y .. j-1] (0<=Y<=k-1)
			//令next[k] = K，则有t[0 .. K-1]=t[k-K .. k-1]，此时令【Y=k-K】
			//即有等式【t[Y .. k-1]=t[j-k+Y .. j-1]=t[k-K .. k-1]=t[Y .. k-1]=t[0 .. K-1]】
			//即t[j-K .. j-1]=t[0 .. K-1]，【next[j]=K=next[k]】
			//若下一次t[j] == t[k'] = t[K]，由于t[j-K .. j-1]=t[0 .. K-1]且K=next[k]，可得t[j-K .. j]=t[0 .. K]，即next[j+1]=next[k]+1
			//【结论：若next[0]...next[j]已知，且t[j] <> t[k]，若t[j] == t[K] == t[next[k]]，则有next[j+1]=next[k]+1】
			//这里要判定t[j] == t[k']是否成立，故令k=next[k]
			k = next[k];
		}
	}
}

// 打印next数组
void Print_Next_Array(int next[], int length)
{
	for (int i = 0; i < length; i++)
	{
		cout << next[i] << ' ';
	}
	cout << endl;
}


// KMP查找
int KMP(char s[], int s_len, char t[], int t_len, int start)
{
	if (start >= s_len) return -1;

	int *next = new int[t_len]; // T字串长度为next数组长度

	if (s_len == 0) throw "原始字串不能为空！";
	if (t_len == 0) throw "匹配字串不能为空！";
	if (t_len > s_len) throw "原始字串比匹配字串短，匹配（替换）失败！";

	Calu_Next_Array(t, next, t_len); // 求next数组
	Print_Next_Array(next, t_len);

	int i = start, j = 0;

	while (i < s_len && j < t_len)
	{
		if (j == -1 || s[i] == t[j])
		{
			i++; j++;
		}
		else
		{
			j = next[j];
		}
	}

	delete[] next;

	if (j == t_len) // T遍历到末尾
	{
		return i - j;
	}

	return -1;
}

// 替换
void Replace(char s[], char t[], char r[])
{
	int s_len = strlen(s);
	int t_len = strlen(t);
	int r_len = strlen(r);

	if (s_len + r_len - t_len >= BUF_SIZE_2) throw "缓冲区不足！";

	int index = 0; // 起始索引
	int location = 0; // 匹配位置
	int count = 0; // 替换次数
	int delta = t_len - r_len;
	int repl_min_len = (t_len <= r_len) ? t_len : r_len;

	while (true)
	{
		index = KMP(s, s_len, t, t_len, index);
		if (index == -1) break;

		location = index + delta * count++;
		cout << "找到匹配位置： " << location << endl;

		// 先替换部分
		{
			for (int i = 0; i < repl_min_len; i++)
			{
				s[index + i] = r[i];
			}
		}

		if (t_len >= r_len)
		{
			// 已替换完毕，开始左移
			char *src = &s[index + t_len];
			char *dest = &s[index + repl_min_len];
			while (*src != 0 && *dest != 0) *dest++ = *src++;
			*dest = 0;
		}
		else
		{
			// 先右移，再替换剩下的
			char *null_src = &s[index + repl_min_len - 1];
			char *src = &s[s_len];
			char *dest = &s[s_len + r_len - t_len];
			while (src != null_src) *dest-- = *src--;

			dest = src + 1;
			src = &r[t_len];
			while (*src != 0) *dest++ = *src++;
		}

		s_len -= delta;
		index -= delta;
		index++;
	}

	cout << "成功替换" << count << "处" << endl;
	cout << "替换结果： " << s << endl << endl;
}

int main(int argc, char* argv[])
{
	char buf_string[BUF_SIZE_2];
	char buf_find[BUF_SIZE];
	char buf_replace[BUF_SIZE];

	cout << "请输入字符串：" << endl;
	cin.getline(buf_string, BUF_SIZE_2 - 1);
	cin.sync();

	cout << "请输入要被替换的字符串：" << endl;
	cin.getline(buf_find, BUF_SIZE - 1);
	cin.sync();

	cout << "请输入替换后的字符串：" << endl;
	cin.getline(buf_replace, BUF_SIZE - 1);
	cin.sync();

	try
	{
		Replace(buf_string, buf_find, buf_replace);
	}
	catch (const char * pstr)
	{
		cerr << "****** 错误： " << pstr << endl << endl;
	}

	cout << endl;

	return 0;
}

