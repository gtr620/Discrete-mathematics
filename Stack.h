#include <iostream>
#include "constant.h"
using namespace std;

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

typedef struct Node
{
    char data;
    BiTree Tdata;
} SElemType;

typedef struct
{
    SElemType *base;
    SElemType *top;
    int stacksize;
} SqStack;

Status InitStack(SqStack &s) //初始化
{
    s.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType)); //申请一段连续的栈空间
    if (!s.base)
    {
        exit(OVERFLOW);
    }
    s.top = s.base;
    s.stacksize = STACK_INIT_SIZE;
    return OK;
}

Status Push(SqStack &s, Node e) //压入栈顶元素
{
    if (s.top - s.base > s.stacksize)
    {
        s.base = (SElemType *)malloc((s.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!s.base)
        {
            return OVERFLOW;
        }
    }
    *s.top++ = e;
    return OK;
}

Status GetTop(SqStack s, SElemType &e) //读出栈顶元素
{
    if (s.top == s.base)
    {
        return ERROR;
    }
    e = *(s.top - 1);
    return OK;
}

Status Pop(SqStack &s, SElemType &e) //取出栈顶元素
{
    if (s.top == s.base)
    {
        return ERROR;
    }
    e = *--s.top;
    return OK;
}

void Print(SqStack s)
{
    SElemType e;
    SqStack temp = s;
    while (temp.top != s.base)
    {
        GetTop(temp, e);
        temp.top--;
        cout << e.data << "\t";
    }
    cout << "\n";
}

Status StackEmpty(SqStack s){
    if (s.top == s.base)
    {
        return ERROR;
    }
    return OK;
}