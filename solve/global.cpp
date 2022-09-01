#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
typedef struct literal {
  short val;   //本文字的真值(正负)
  short absv;  //本文字的绝对值
  struct literal* nextSame;  //下一个不同子句中同样的文字
  struct literal* pre, * next; //在本子句的前驱后继
  struct clause* belong; //本文字所属子句
  literal(short v = 0, bool isHead = false) {
    val = v;
    absv = abs(v);
    nextSame = nullptr;
    belong = nullptr;
    (isHead ? (pre = next = this) : (pre = next = nullptr));
  }
}L;
typedef struct clause {
  short length;  //子句所含文字长度
  L* delBy;  //该子句因哪个文字被删除（若为nullptr则表示未被删除）
  struct clause* pre, * next; //在本子句集的前驱后继
  L* head;  //本子句文字首节点
  struct clause* singleNext;   //下一个单子句
  clause(bool isHead = false) {
    length = 0;
    delBy = nullptr;
    singleNext = nullptr;
    (isHead ? (pre = next = this) : (pre = next = nullptr));
    head = new L(0, true);  //new方法获得的子句自带文字head
  }
}C;
typedef struct clauseSet {
  C* singleClause; //单子句栈（为nullptr代表无单子句
  C* head;  //本子句集的子句首节点
  clauseSet() {
    singleClause = nullptr;
    head = new C(true); //new方法获得的子句集自带子句head
  }
}S;


static S* s;

//变元数组指针，用以链接相同的子句
static L** l;

//记录变元对应真值，正真负假，0号位记录变元数
static short* res;


struct PTRL //回溯指针
{
  L* ptrL;  //指向文字
  PTRL* next;
  PTRL(L* l = nullptr) {
    ptrL = l;
    next = nullptr;
  }
};
struct PTRC //回溯指针
{
  C* ptrC;  //指向子句
  PTRC* next;
  PTRC(C* c = nullptr) {
    ptrC = c;
    next = nullptr;
  }
};

// string dirname = "cnfs/";
static const string dirname = "";


template<typename T>
inline T abs(T a) {
  return (a > 0 ? a : -a);
}


static const short singleSize = 81;
static const short doubleSize = 162;


static short sudokuArr[200] = {};  //结果数组
static short* initArr[200] = {};  //指向结果数组的对应指针数组

static short hole = 0, count = 0;