#pragma once
#include <string>
#include "operate.cpp"

inline bool readCnf(S* s, string filename) { //读文件
  char c;
  string buf;
  ifstream ifs;
  ifs.open(dirname + filename + ".cnf", ios::in);
  if (!ifs.is_open()) {
    cout << "文件打开失败" << endl;
    return false;
  }
  ifs >> c;
  while (c != 'p') {
    getline(ifs, buf);
    ifs >> c;
  }
  ifs >> buf;
  short vNum = 0, cNum = 0;
  ifs >> vNum >> cNum;
  res = new short[vNum + 10] {};
  res[0] = vNum;
  l = new L * [vNum + 10] {};
  while (cNum--) {
    short v;
    C* tempC = new C;  //创建并插入子句
    insertCL(s, tempC);
    // tempC->head = new L;  //子句文字头节点
    tempC->head->belong = tempC;
    ifs >> v;
    while (v) {
      ++tempC->length; //子句文字增
      L* tempL = new L(v);
      tempL->nextSame = l[tempL->absv];  //连接同文字数组
      l[tempL->absv] = tempL;

      tempL->belong = tempC;
      //连接文字成为子句
      insertCL(tempC, tempL);
      ifs >> v;
    }
    if (tempC->length == 1) {
      addSingle(s, tempC);
    }
  }
  ifs.close();
  return true;
}

inline bool outPutCnf(S* s, int initNum = 0) {  //打印数据
  if (initNum) { //initNum为初盘数字数
    ofstream ofs;
    ofs.open("sudoku.cnf", ios::out);
    if (!ofs.is_open()) {
      cout << "文件打开失败" << endl;
      ofs.close();
      return false;
    }
    ofs << "p cnf 1458 " << (21222 + initNum) << endl;

    C* outC = s->head;
    while (outC->next != s->head) {
      outC = outC->next;
      L* outL = outC->head;
      while (outL->next != outC->head) {
        outL = outL->next;
        ofs << outL->val << " ";
      }
      ofs << "0" << endl;
    }
  }
  else {
    puts("---------------- 读入的数据如下： -----------------\n");
    C* outC = s->head;
    while (outC->next != s->head) {
      outC = outC->next;
      L* outL = outC->head;
      while (outL->next != outC->head) {
        outL = outL->next;
        cout << outL->val << " ";
      }
      cout << endl;
    }
    puts("---------------------- end ----------------------\n");
  }
  return true;
}

inline bool cnfRes(bool flag, clock_t time, string filename, short isSudoku = 0) {
  ofstream ofs;
  ofs.open(filename + ".res", ios::out);
  if (!ofs.is_open()) {
    cout << "文件打开失败" << endl;
    ofs.close();
    return false;
  }
  if (isSudoku) {
    ofs << "s " << test(s) << endl << "v";
    for (int i = 0; i <= res[0]; i++) {
      ofs << " " << res[i];
    }
    ofs << endl << "t " << time;
    ofs.close();
    return true;
  }
  
  if (flag) {
    ofs << "s " << test(s) << endl << "v";
    for (int i = 1; i <= res[0]; i++) {
      ofs << " " << res[i];
    }
    ofs << endl << "t " << time;
  }
  else {
    ofs << "s " << test(s) << endl << "t " << time;
  }
  ofs.close();
  return true;
}