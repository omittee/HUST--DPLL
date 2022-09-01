#pragma once
#include "solver.cpp"

inline bool constraint(string filename) {
  ofstream ofs;
  ofs.open(filename, ios::out);
  if (!ofs.is_open()) {
    cout << "文件打开失败" << endl;
    ofs.close();
    return false;
  }
  ofs << "p cnf 1458 21222      " << endl;
  for (int i = 0; i < 9; i++) {  //可以填的数
    for (int j = 0; j < 9; j++) {
      for (int k = 1; k <= 9; k++) {
        ofs << (i * 81 + j * 9 + k) << " ";
        // ofs << i + 1 << j + 1 << k << " ";
      }
      ofs << "0" << endl;

      for (int k = 1; k <= 9; k++) {
        ofs << 729 + (i * 81 + j * 9 + k) << " ";
        // ofs << i + 1 << j + 1 << k << " ";
      }
      ofs << "0" << endl;
    }
  }
  for (int i = 0; i < 9; i++) {  //一个一个数
    for (int j = 0; j < 9; j++) {
      for (int ka = 1; ka <= 8; ka++) {
        for (int kb = ka + 1; kb <= 9; kb++) {
          ofs << -(i * 81 + j * 9 + ka) << " " << -(i * 81 + j * 9 + kb) << " 0" << endl;
          ofs << -(i * 81 + j * 9 + ka + 729) << " " << -(i * 81 + j * 9 + kb + 729) << " 0" << endl;
          // ofs << -(i + 1) << j + 1 << ka << " "<< -(i + 1) << j + 1 << kb <<endl;
        }
      }
    }
  }
  for (int i = 0; i < 9; i++) { //第i+1行,j+1列含有k
    for (int k = 1; k <= 9; k++) {
      for (int j = 0; j < 9; j++) {
        ofs << (i * 81 + j * 9 + k) << " ";
        // ofs << i + 1 << j + 1 << k << " ";
      }
      ofs << "0" << endl;
      for (int j = 0; j < 9; j++) {
        ofs << (i * 81 + j * 9 + k + 729) << " ";
        // ofs << i + 1 << j + 1 << k << " ";
      }
      ofs << "0" << endl;
    }
  }
  for (int j = 0; j < 9; j++) { //第j+1列,i+1行含有k
    for (int k = 1; k <= 9; k++) {
      for (int i = 0; i < 9; i++) {
        ofs << (i * 81 + j * 9 + k) << " ";
        // ofs << i + 1 << j + 1 << k << " ";
      }
      ofs << "0" << endl;
      for (int i = 0; i < 9; i++) {
        ofs << (i * 81 + j * 9 + k + 729) << " ";
        // ofs << i + 1 << j + 1 << k << " ";
      }
      ofs << "0" << endl;
    }
  }
  for (int k = 1; k <= 9; k++) {
    for (int i = 0; i < 9; i++) { //两格不同数
      for (int ja = 0; ja < 8; ja++) {
        for (int jb = ja + 1; jb < 9; jb++) {
          ofs << -(i * 81 + ja * 9 + k) << " " << -(i * 81 + jb * 9 + k) << " 0" << endl;
          ofs << -(i * 81 + ja * 9 + k + 729) << " " << -(i * 81 + jb * 9 + k + 729) << " 0" << endl;
          // ofs << -(i + 1) << ja + 1 << k << " " << -(i + 1) << jb + 1 << k << " 0" << endl;
        }
      }
    }
  }
  for (int k = 1; k <= 9; k++) {//两格不同数
    for (int j = 0; j < 9; j++) {
      for (int ia = 0; ia < 8; ia++) {
        for (int ib = ia + 1; ib < 9; ib++) {
          ofs << -(ia * 81 + j * 9 + k) << " " << -(ib * 81 + j * 9 + k) << " 0" << endl;
          ofs << -(ia * 81 + j * 9 + k + 729) << " " << -(ib * 81 + j * 9 + k + 729) << " 0" << endl;
          // ofs << -(ia + 1) << j + 1 << k << " " << -(ib + 1) << j + 1 << k << " 0" << endl;
        }
      }
    }
  }
  for (int a = 0; a <= 6; a += 3) { //九宫格
    for (int b = 0; b <= 6; b += 3) {
      for (int k = 1; k <= 9; k++) {
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            ofs << (i + a) * 81 + (j + b) * 9 + k << " ";
            // ofs << i + a + 1 << j + b + 1 << k << " ";
          }
        }
        ofs << "0" << endl;
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            ofs << (i + a) * 81 + (j + b) * 9 + k + 729 << " ";
            // ofs << i + a + 1 << j + b + 1 << k << " ";
          }
        }
        ofs << "0" << endl;
      }
    }
  }
  for (int a = 0; a <= 6; a += 3) { 
    for (int b = 0; b <= 6; b += 3) {
      for (int k = 1; k <= 9; k++) {
        for (int ix = 0; ix < 3; ix++) {
          for (int jx = 0; jx < 2; jx++) {
            for (int iy = 0; iy < 3; iy++) {
              if (ix == iy) continue;
              for (int jy = jx + 1; jy < 3; jy++) {
                ofs << -((ix + a) * 81 + (jx + b) * 9 + k) << " " << -((iy + a) * 81 + (jy + b) * 9 + k) << " 0" << endl;
                ofs << -((ix + a) * 81 + (jx + b) * 9 + k + 729) << " " << -((iy + a) * 81 + (jy + b) * 9 + k + 729) << " 0" << endl;
                // ofs << -(ix + a + 1) << jx + b + 1 << k << " " << -(iy + a + 1) << jy + b + 1 << k << " 0" << endl;
              }
            }
          }
        }
      }
    }
  }
  for (int k = 1; k <= 9; k++) {
    for (int i = 6; i < 9; i++) {
      for (int j = 6; j < 9; j++) {
        ofs << -(i * 81 + j * 9 + k) << " " << (i * 81 + j * 9 + k + 189) << " 0" << endl;
        ofs << (i * 81 + j * 9 + k) << " " << -(i * 81 + j * 9 + k + 189) << " 0" << endl;
      }
    }
  }
  ofs.close();
  return true;
}

inline bool getSudokuFinal(S* s) {
  short arr[9] = { 1,2,3,4,5,6,7,8,9 };
  for (int a = 0; a <= 6; a += 3) {
    for (int b = 0; b < 9; b++) {
      swap(arr[b], arr[rand() % 9]);
    }
    int m = 0;
    for (int i = 0; i < 3; i++) {  //填充对角线九宫格
      for (int j = 0; j < 3; j++) {
        C* tempSC = createSingle((i + a) * 81 + (j + a) * 9 + arr[m++]);   //创建单子句
        insertCL(s, tempSC);
        addSingle(s, tempSC);
      }
    }
  }
  for (int a = 3; a <= 6; a += 3) {
    for (int b = 0; b < 9; b++) {
      swap(arr[b], arr[rand() % 9]);
    }
    int m = 0;
    for (int i = 0; i < 3; i++) {  //填充对角线九宫格
      for (int j = 0; j < 3; j++) {
        C* tempSC = createSingle((i + a) * 81 + (j + a) * 9 + arr[m++] + 729);   //创建单子句
        insertCL(s, tempSC);
        addSingle(s, tempSC);
      }
    }
  }
  PTRL* pl = new PTRL;
  PTRC* pc = new PTRC;
  bool flag = DPLL(s, pl, pc);  //生成终盘
  delete pl;
  pl = nullptr;
  delete pc;
  pc = nullptr;
  return flag;
}
inline bool getSudokuInit(S* ss) {
  PTRL* pl = new PTRL;
  PTRC* pc = new PTRC;
  short pos = rand() % doubleSize;  //随机挖洞
  while ((*initArr[pos]) == 0) //已经被挖的重新挖
  {
    pos = rand() % doubleSize;
  }
  short v = (*initArr[pos]);
  C* getC = nullptr;
  for (C* c = s->head->next; c->length == 1; c = c->next) {  //找到对应单子句进行删除
    if (c->head->next->val == v) {
      getC = c;
      l[c->head->next->absv] = l[c->head->next->absv]->nextSame;
      removeCL(c);  //先移除
      break;
    }
  }
  short k = ((v % 9) ? v % 9 : 9);  //获取值
  v -= k;  //直接从0开始
  for (int i = 1; i <= 9; i++) {
    if (i == k) continue;  //跳过正确答案
    L* lp = l[v + i];
    C* tempSC = createSingle(v + i);   //创建单子句
    if (DPLL(ss, pl, pc, tempSC)) {  //多个解
      l[getC->head->next->absv] = getC->head->next;
      rebuildCL(getC);
      l[v + i] = lp;
      if(hole < 100 && count < 5) { //count防止一直多个解而死循环
        count++;
        return getSudokuInit(ss);
      } 
      return true;//不唯一且洞足够，不挖并退出
    }
    l[v + i] = lp;
  }
  (*initArr[pos]) = 0;  //单解则挖洞
  hole++;
  destroyC(getC);
  return getSudokuInit(ss);
}
