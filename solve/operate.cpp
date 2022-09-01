#pragma once
#include "global.cpp"
#include <type_traits>
template<typename TSC, typename TCL>
inline void insertCL(TSC* SC, TCL* CL) {  //插入子句
  CL->next = SC->head->next;
  CL->pre = SC->head;
  SC->head->next = CL;
  CL->next->pre = CL;
}


template<typename T>
inline void removeCL(T* cl) {  //移除子句/子文字
  if (is_same<T, C>::value || is_same<T, L>::value) {
    if (cl->pre && cl->next) {
      cl->pre->next = cl->next;
      cl->next->pre = cl->pre;
    }
  }
}
template<typename T>
inline void rebuildCL(T* cl) {  //重建子句/子文字
  if (is_same<T, C>::value || is_same<T, L>::value) {
    if (cl->pre && cl->next) {
      cl->pre->next = cl; cl->next->pre = cl;
    }
  }
}

inline void addSingle(S* s, C* c) {
  c->singleNext = s->singleClause;
  s->singleClause = c;
}

inline void reEstab(S* s, PTRC* headpc, PTRC* nc, PTRL* headpl, PTRL* nl) { //根据回溯指针执行重建
  PTRC* tc = nullptr;
  PTRL* tl = nullptr;
  while (headpl != nl) {
    rebuildCL(headpl->ptrL);
    headpl->ptrL->belong->length++; //恢复子句文字长度
    tl = headpl->next;
    delete headpl;
    headpl = tl;
  }
  
  while (s->singleClause&&s->singleClause->length > 1)
  {
    s->singleClause = s->singleClause->singleNext;
  }
  
  while (headpc != nc) {
    rebuildCL(headpc->ptrC);
    headpc->ptrC->delBy = nullptr;//撤销删除标识
    if (headpc->ptrC->length == 1) {
      addSingle(s, headpc->ptrC);
    }
    tc = headpc->next;
    delete headpc;
    headpc = tc;
  }
}

inline C* createSingle(short v) {
  C* c = new C;
  c->length = 1;
  L* tempL = new L(v);
  c->head->next = c->head->pre = tempL;
  tempL->next = tempL->pre = c->head;

  tempL->nextSame = l[tempL->absv];  //连接同文字数组
  l[tempL->absv] = tempL;

  tempL->belong = c;
  return c;
}

inline C* getMinC(S* s) {
  C* c = s->head->next;
  C* minC = nullptr; //c最短非单子句
  while (c != s->head) {
    if (c->length > 1) {
      minC = ((!minC || c->length < minC->length) && (!c->delBy)) ? c : minC;
      if (minC->length == 2) return minC;
    }
    c = c->next;
  }
  return minC;
}

inline void getSingle(S* s, C* tempC = nullptr) {
  s->singleClause = nullptr;
  C* c = s->head->next;
  while (c != s->head) {
    if (c->length == 1) {
      addSingle(s, c);
    }
    c = c->next;
  }
  if (tempC) {
    addSingle(s, tempC);
  }
}

inline void destroyC(C*& c) { //销毁子句
  if (c) {
    removeCL(c);
    L* rmL = c->head, * rmLNext = rmL->next;
    while (rmLNext != c->head) {
      delete rmL;
      rmL = rmLNext;
      rmLNext = rmLNext->next;
    }
    delete rmL;
    rmL = nullptr;
    delete c;
    c = nullptr;
  }
}

inline void destroyS(S*& s) {
  delete l;
  l = nullptr;
  delete res;
  res = nullptr;

  if (!s) {
    return;
  }
  s->singleClause = nullptr;
  C* c = s->head->next;
  while (c != s->head) {
    destroyC(c);
    c = s->head->next;
  }
  destroyC(s->head);
  delete s;
  s = nullptr;
}

inline bool test(S* s) {
  C* tC = s->head;
  cout << endl;
  while (tC->next != s->head) {
    tC = tC->next;
    L* tL = tC->head;
    bool flagl = false;
    while (tL->next != tC->head) {
      tL = tL->next;
      if (res[tL->absv] && res[tL->absv] == tL->val) {
        flagl = true;
        break;
      }
    }
    if (!flagl) return false;
  }
  return true;
}