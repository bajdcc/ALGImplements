// KMP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <IOSTREAM>
using namespace std;

#define BUF_SIZE 100
#define BUF_SIZE_2 200

/************************************************************************/
/* KMP�㷨                                                              */
/************************************************************************/

//************************************
// Method:    Compute next array
// FullName:  ����NEXT����
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char t[] ģʽ��
// Parameter: int next[] next����
// Parameter: int length ģʽ������
//************************************
void Calu_Next_Array(char t[], int next[], int length)
{
	int j = 0, k = -1; next[0] = -1;
	//next[0]ʹ��ģʽ��T������S����ǰǰ��һ����-1���������־
	//next�����СӦΪģʽ��T����
	//jѭ����0��length-2������n-1��ֵ����next[0]=-1��һ����n��ֵ
	while (j < length - 1)
	{
		if (k == -1 || t[j] == t[k])
		{
			//����k=-1�����ǵ�һ��������ҽ������һ��
			//��k=-1ʱ����k=0,j=1,next[1]=0����֪next[1]=0Ҳ�Ǻ㶨�����
			//�ɵ��ƹ�ʽ��֪����ʼ�����next[0]=-1,next[1]=0

			//*******************  ���ƹ�ʽ�Ƶ�  *******************
			//�����t[j] == t[k]�����У�
			//���Ƶ�����next[0]...next[j]�Ѿ������next[j+1]=����
			//��next[j] = k������Ƴ�t[0 .. k-1]=t[j-k .. j-1]
			//����Ϊt[j] == t[k]������t[0 .. k]=t[j-k .. j]���Ƴ���next[j+1]=k+1��
			//����Ϊnext[j] = k���ʡ�j++��k++��
			//�����ۣ���next[0]...next[j]��֪����t[j] == t[k]����ɵ�next[j+1]=next[j]+1��
			next[++j] = ++k;
		}
		else
		{
			//*******************  ���ƹ�ʽ�Ƶ�  *******************
			//����t[j] <> t[k]����next[j] = k������Ƴ�t[0 .. k-1]=t[j-k .. j-1]
			//����t[0 .. k-1]=t[j-k .. j-1]������t[Y .. k-1]=t[j-k+Y .. j-1] (0<=Y<=k-1)
			//��next[k] = K������t[0 .. K-1]=t[k-K .. k-1]����ʱ�Y=k-K��
			//���е�ʽ��t[Y .. k-1]=t[j-k+Y .. j-1]=t[k-K .. k-1]=t[Y .. k-1]=t[0 .. K-1]��
			//��t[j-K .. j-1]=t[0 .. K-1]����next[j]=K=next[k]��
			//����һ��t[j] == t[k'] = t[K]������t[j-K .. j-1]=t[0 .. K-1]��K=next[k]���ɵ�t[j-K .. j]=t[0 .. K]����next[j+1]=next[k]+1
			//�����ۣ���next[0]...next[j]��֪����t[j] <> t[k]����t[j] == t[K] == t[next[k]]������next[j+1]=next[k]+1��
			//����Ҫ�ж�t[j] == t[k']�Ƿ����������k=next[k]
			k = next[k];
		}
	}
}

// ��ӡnext����
void Print_Next_Array(int next[], int length)
{
	for (int i = 0; i < length; i++)
	{
		cout << next[i] << ' ';
	}
	cout << endl;
}


// KMP����
int KMP(char s[], int s_len, char t[], int t_len, int start)
{
	if (start >= s_len) return -1;

	int *next = new int[t_len]; // T�ִ�����Ϊnext���鳤��

	if (s_len == 0) throw "ԭʼ�ִ�����Ϊ�գ�";
	if (t_len == 0) throw "ƥ���ִ�����Ϊ�գ�";
	if (t_len > s_len) throw "ԭʼ�ִ���ƥ���ִ��̣�ƥ�䣨�滻��ʧ�ܣ�";

	Calu_Next_Array(t, next, t_len); // ��next����
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

	if (j == t_len) // T������ĩβ
	{
		return i - j;
	}

	return -1;
}

// �滻
void Replace(char s[], char t[], char r[])
{
	int s_len = strlen(s);
	int t_len = strlen(t);
	int r_len = strlen(r);

	if (s_len + r_len - t_len >= BUF_SIZE_2) throw "���������㣡";

	int index = 0; // ��ʼ����
	int location = 0; // ƥ��λ��
	int count = 0; // �滻����
	int delta = t_len - r_len;
	int repl_min_len = (t_len <= r_len) ? t_len : r_len;

	while (true)
	{
		index = KMP(s, s_len, t, t_len, index);
		if (index == -1) break;

		location = index + delta * count++;
		cout << "�ҵ�ƥ��λ�ã� " << location << endl;

		// ���滻����
		{
			for (int i = 0; i < repl_min_len; i++)
			{
				s[index + i] = r[i];
			}
		}

		if (t_len >= r_len)
		{
			// ���滻��ϣ���ʼ����
			char *src = &s[index + t_len];
			char *dest = &s[index + repl_min_len];
			while (*src != 0 && *dest != 0) *dest++ = *src++;
			*dest = 0;
		}
		else
		{
			// �����ƣ����滻ʣ�µ�
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

	cout << "�ɹ��滻" << count << "��" << endl;
	cout << "�滻����� " << s << endl << endl;
}

int main(int argc, char* argv[])
{
	char buf_string[BUF_SIZE_2];
	char buf_find[BUF_SIZE];
	char buf_replace[BUF_SIZE];

	cout << "�������ַ�����" << endl;
	cin.getline(buf_string, BUF_SIZE_2 - 1);
	cin.sync();

	cout << "������Ҫ���滻���ַ�����" << endl;
	cin.getline(buf_find, BUF_SIZE - 1);
	cin.sync();

	cout << "�������滻����ַ�����" << endl;
	cin.getline(buf_replace, BUF_SIZE - 1);
	cin.sync();

	try
	{
		Replace(buf_string, buf_find, buf_replace);
	}
	catch (const char * pstr)
	{
		cerr << "****** ���� " << pstr << endl << endl;
	}

	cout << endl;

	return 0;
}

