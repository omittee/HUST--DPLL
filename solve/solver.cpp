#pragma once
#include "cnfParser.cpp"

inline bool DPLL(S* s, PTRL* headpl, PTRC* headpc, C* tempC = nullptr) {  //temC为上一步传入的创建的单子句
  if (tempC) {
    addSingle(s, tempC);
  }
  C* sgC;
  PTRL* tpl = headpl;
  PTRC* tpc = headpc;
  while (sgC = s->singleClause) { 
    L* sameL = sgC->head->next;
    res[sameL->absv] = sameL->val;  //记录该文字真值
    sameL = l[sameL->absv];
    while (sameL) {
      if (!sameL->belong->delBy) {//若子句已经删除就跳过当前文字
        //相同则子句满足去掉，不同则去掉该文字
        if (sameL->val == res[sameL->absv]) {
          removeCL(sameL->belong);
          if (sameL->belong != tempC) {
            PTRC* nc = new PTRC(sameL->belong);  //建立移除的子句链，便于回溯
            nc->next = headpc;
            headpc = nc;
          }
          sameL->belong->delBy = sameL;  //标记删除
        }
        else {
          removeCL(sameL);
          PTRL* nl = new PTRL(sameL);
          nl->next = headpl;
          headpl = nl;

          sameL->belong->length--;  //文字减
          if (sameL->belong->length == 1) {
            sameL->belong->singleNext = sgC->singleNext;  //在当前单子句后接上新单子句
            sgC->singleNext = sameL->belong;
          }
          else if (!sameL->belong->length) //该子句为空
          {
            destroyC(tempC);
            reEstab(s, headpc, tpc, headpl, tpl);
            return false;
          }
        }
      }

      if (s->head->next == s->head) {
        destroyC(tempC);
        reEstab(s, headpc, tpc, headpl, tpl);
        return true;
      }
      sameL = sameL->nextSame;
    }
    while (sgC->singleNext && sgC->singleNext->delBy) { //剔除删除的子句
      s->singleClause = sgC->singleNext;
      sgC->singleNext = NULL;
      sgC = s->singleClause;
    }
    s->singleClause = sgC->singleNext;
    sgC->singleNext = NULL;
  }
  destroyC(tempC);

  //选取变元

  short v;
  // v = s->head->next->head->next->val;//选择第一个未删除子句的第一个子文字（无优化）
  C* selectC = getMinC(s);
  if (selectC) {
    v = selectC->head->next->val;
  }
  else {
    v = s->head->next->head->next->val;
  }
  short absv = abs(v);
  L *lp = l[absv];
  C* tempSC = createSingle(v); //创建单子句
  // cout<<v<<" ";
  if (DPLL(s, headpl, headpc, tempSC)) {
    l[absv] = lp;  //移除单子句栈的记录
    reEstab(s, headpc, tpc, headpl, tpl);
    return true;
  }
  else {
    // cout<<-v<<" ";
    l[absv] = lp;
    tempSC = createSingle(-v);
    bool k = DPLL(s, headpl, headpc, tempSC);
    l[absv] = lp;
    reEstab(s, headpc, tpc, headpl, tpl);
    return k;
  }
}

