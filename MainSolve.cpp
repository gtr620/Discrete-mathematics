//求命题公式的真值表和主析取范式,主合取范式
#include <math.h>
#include "Stack.h"
//@ author  陈泽曈
string Extract(string Propositiona);                                         //提取命题变元名称，计算数目
bool CalculateList(int x, int y);                                            //根据行列的坐标计算真值
bool OP(char c);                                                             //判断字符是否为运算符 - * + @ $
int Compare(char top, char c);                                               //比较栈顶元素和插入元素大小
int CompareChange(char c);                                                   //将命题符号转换为对应数组数字
bool OP(char c);                                                             //判断字符是否为运算符 - * + @ $ ()
int ColumnChange(char c, string Item);                                       //计算变元的列数
char Operate(SElemType a, SElemType theta, SElemType b, int x, string Item); //计算两项真值
bool Judge(string Item, string Propositional, int x);                        //输出该行真值
string SolveFei(string Propositional);                                       //将命题公式里有非的命题变元转化为小写字母,便于区分识别
void SolveZXQFS(string Item, int x);                                         //求主析取范式子式
void SolveZHQFS(string Item, int x);                                         //求主合取范式子式
void PrintAndTransfrom(string a);                                            //输出∧符号

long int row = 0;    //行数,行表示真值表的真假值
int column = 0; //列数,列表示命题变元
string ZXQFS;   //主析取范式
string ZHQFS;   //主合取范式


int main()
{
    string Propositional, Item; //命题公式
    cout << "\n五种联结词表示方法: 否定:-   合取:*   析取:+   单条件:@   双条件:$\n";
    cout << "\n输入命题公式(命题变元为大写英文字母)：\t";

    cin >> Propositional;//输入命题
    Item = Extract(Propositional); //提取命题变元名称，计算数目
    row = (int)pow(2, column);
    for (int i = 0; i < column; i++)
    {
        cout << Item[i];
        cout << "\t";
    }
    cout << "命题公式\n";
    Propositional = SolveFei(Propositional);
    for (int i = 1; i <= row; i++)
    {
        for (int j = 1; j <= column; j++)
        {
            cout << CalculateList(i, j);
            cout << "\t";
        }
        cout << "    " << Judge(Item, Propositional, i);
        cout << "\n";
    }
    cout << "\n主析取范式:\n";
    PrintAndTransfrom(ZXQFS);
    cout << "\n主合取范式:\n";
    PrintAndTransfrom(ZHQFS);
    system("pause");
}

string Extract(string Propositional) //提取命题变元名称，计算数目
{
    string Item;
    int temp = 0;
    bool Index = false;
    while (Propositional[temp])
    {
        if (Propositional[temp] <= 'Z' && Propositional[temp] >= 'A')
        {
            for (int j = 0; j < column; j++)
            {
                if (Item[j] == Propositional[temp])
                {
                    Index = true;
                    break;
                }
            }
            if (Index)
            {
                temp++;
                Index = false;
                continue;
            }
            Item += Propositional[temp];
            column++;
        }
        if (OP(Propositional[0]) && !((Propositional[0] != '-' && Propositional[0] == '(') || (Propositional[0] == '-' && Propositional[0] != '(')))
        {
            cout << "\n命题公式开头有运算符" << Propositional[0] << ",请去掉后输入\n";
            system("pause");
            exit(0);
        }
        if (Propositional[temp] <= 'z' && Propositional[temp] >= 'a')
        {
            cout << "\n每个命题变元必须为26个大写英文字符之一,请确认后输入\n";
            system("pause");
            exit(0);
        }
        temp++;
    }
    if (column == 1)
    {
        cout << "\n命题变元至少两个\n";
        system("pause");
        exit(0);
    }
    return Item;
}

bool CalculateList(int x, int y) //根据行列的坐标计算真值
{
    int index;
    x--;
    index = x / pow(2, column - y);
    if (index % 2)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool CalculateListFei(int x, int y) //根据行列的坐标计算真值(处理非专用)
{
    int index;
    x--;
    index = x / pow(2, column - y);
    if (index % 2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Judge(string Item, string Propositional, int x) //输出该行真值
{
    int i = 0, index = 0;
    char tempC = '(';
    Propositional += '#'; //在表达式右边添加#表示结束
    SqStack OPTR;         //运算符栈
    SqStack OPND;         //运算数栈
    InitStack(OPTR);
    InitStack(OPND);
    SElemType e, temp, theta, a, b, temp1;
    e.data = '#';
    Push(OPTR, e);
    e.data = Propositional[i];
    temp1.data = '0';
    while (e.data != '#' || temp1.data != '#')
    {
        if (CompareChange(e.data) == 0) //非做特殊处理
        {
            index = 1;
            Propositional.replace(i, 2 * sizeof(char), &tempC);
            Propositional.erase(i + 1, 3 * sizeof(char));
            e.data = '(';
        }
        if (!OP(e.data))
        {
            Push(OPND, e);
            e.data = Propositional[++i];
        }
        else
        {
            GetTop(OPTR, temp);
            switch (Compare(temp.data, e.data))
            {
            case -1: //栈顶元素优先级低
                Push(OPTR, e);
                e.data = Propositional[++i];
                break;
            case 0: //脱括号并接受下一字符
                Pop(OPTR, e);
                if (index == 1) //解决非后面跟括号的问题
                {
                    Pop(OPND, e);
                    if (e.data == 0)
                    {
                        e.data = 1;
                    }
                    else
                    {
                        e.data = 0;
                    }
                    Push(OPND, e);
                    index = 0;
                }
                e.data = Propositional[++i];
                break;
            case 1: //退栈并将运算结果入栈
                Pop(OPTR, theta);
                Pop(OPND, b);
                Pop(OPND, a);
                temp1.data = Operate(a, theta, b, x, Item);
                Push(OPND, temp1);
                break;
            }
        }
        GetTop(OPTR, temp1);
    }

    GetTop(OPND, temp1);
    if (temp1.data == '0')
    {
        SolveZHQFS(Item, x);
        return false;
    }
    else
    {
        SolveZXQFS(Item, x);
        return true;
    }
}

bool OP(char c) //判断字符是否为运算符 - * + @ $ ()
{
    if (c == '-' || c == '*' || c == '+' || c == '@' || c == '$' || c == '(' || c == ')' || c == '#')
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Compare(char top, char c) //比较栈顶元素和插入元素大小
{
    int x, y;
    int A[8][8] = {{1, 1, 1, 1, 1, -1, 1, 1}, {-1, 1, -2, 1, 1, -1, 1, 1}, {-1, -2, 1, 1, 1, -1, 1, 1}, {-1, -1, -1, 1, 1, -1, 1, 1}, {-1, -1, -1, -1, 1, -1, 1, 1}, {-1, -1, -1, -1, -1, -1, 0, -2}, {1, 1, 1, 1, 1, -2, 1, 1}, {-1, -1, -1, -1, -1, -1, -2, 0}};
    x = CompareChange(top);
    y = CompareChange(c);
    if (A[x][y] == -2)
    {
        cout << "/n命题公式格式不正确,请检查\n";
        system("pause");
        exit(0);
    }
    return A[x][y];
}

int CompareChange(char c) //将命题符号转换为对应数组数字
{
    if (c == '-')
        return 0;
    if (c == '*')
        return 1;
    if (c == '+')
        return 2;
    if (c == '@')
        return 3;
    if (c == '$')
        return 4;
    if (c == '(')
        return 5;
    if (c == ')')
        return 6;
    if (c == '#')
        return 7;
    return 8; //不会被return 只是消除警告
}

int ColumnChange(char c, string Item) //计算变元的列数
{
    if (c >= 'a' && c <= 'z')
    {
        c -= 32; //如果读到小写字母,转化为大写字母,因为Item中存的是大写字母
    }
    int n = 0;
    while (Item[n] != c)
    {
        n++;
    }
    return n;
}
char Operate(SElemType a, SElemType theta, SElemType b, int x, string Item) //计算两项真值
{
    int A, B;
    if (a.data == '1')
    {
        A = 1;
    }
    else if (a.data == '0')
    {
        A = 0;
    }
    else if (a.data >= 'A' && a.data <= 'Z')
    {
        A = CalculateList(x, ColumnChange(a.data, Item) + 1);
    }
    else
    {
        A = CalculateListFei(x, ColumnChange(a.data, Item) + 1);
    }

    if (b.data == '1')
    {
        B = 1;
    }
    else if (b.data == '0')
    {
        B = 0;
    }
    else if (b.data >= 'A' && b.data <= 'Z')
    {
        B = CalculateList(x, ColumnChange(b.data, Item) + 1);
    }
    else
    {
        B = CalculateListFei(x, ColumnChange(b.data, Item) + 1);
    }
    switch (theta.data)
    {
    case '+':
        if (A == 0 && B == 0) //析取
        {
            return '0';
        }
        else
        {
            return '1';
        }
        break;
    case '*':
        if (A == 0 || B == 0) //合取
        {
            return '0';
        }
        else
        {
            return '1';
        }
        break;
    case '@':
        if (A == 1 && B == 0) //单条件
        {
            return '0';
        }
        else
        {
            return '1';
        }
        break;
    case '$':
        if ((A == 1 && B == 0) || (A == 0 && B == 1)) //双条件
        {
            return '0';
        }
        else
        {
            return '1';
        }
        break;
    default:
        cout << "\n表达式中有未知运算符,请按照要求修改\n";
        system("pause");
        exit(0);
    }
}

string SolveFei(string Propositional) //将命题公式里有非的命题变元转化为小写字母,便于区分识别
{
    int i = 0;
    char temp;
    while (Propositional[i])
    {
        if (Propositional[i] == '-' && Propositional[i + 1] != '(')
        {
            temp = Propositional[i + 1];
            temp += 32; //转化为小写字母
            Propositional.replace(i, 2 * sizeof(char), &temp);
            Propositional.erase(i + 1, sizeof(char));
        }
        i++;
    }
    return Propositional;
}

void SolveZXQFS(string Item, int x) //求主析取范式子式
{
    string temp;
    int index, i = 0;
    for (int j = 1; j <= column; j++)
    {
        index = CalculateList(x, j);
        if (index == 1)
        {
            temp += Item[i++];
        }
        else
        {
            temp += '-';
            temp += Item[i++];
        }
        if (j != column)
        {
            temp += '&';
        }
    }
    if (x == 1)
    {
        ZXQFS = ZXQFS + temp;
    }
    else
    {
        ZXQFS = ZXQFS + ':' + '(' + temp;
    }
    ZXQFS += ')'; //末尾括号
}
void SolveZHQFS(string Item, int x) //求主合取范式子式
{
    string temp;
    int index, i = 0;
    static bool a = true; //指示被调用几次
    for (int j = 1; j <= column; j++)
    {
        index = CalculateList(x, j);
        if (index == 1)
        {
            temp += '-';
            temp += Item[i++];
        }
        else
        {
            temp += Item[i++];
        }
        if (j != column)
        {
            temp += ':';
        }
    }
    if (a)
    {
        ZHQFS = ZHQFS + temp;
        a = false;
    }
    else
    {
        ZHQFS = ZHQFS + '&' + '(' + temp; //书上例题(P*Q)+(-P*R)
    }
    ZHQFS += ')';
}

void PrintAndTransfrom(string a) //合取析取符号不是ASC2码,只能单独输出
{
    int i = 0;
    cout << '('; //打前括号
    while (a[i])
    {
        if (a[i] == '&')
        {
            cout << "∧";
        }
        else if (a[i] == ':')
        {
            cout << "∨";
        }
        else
        {
            cout << a[i];
        }
        i++;
    }
    cout << "\n";
}