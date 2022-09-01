#include "sudoku.cpp"
bool solve(string path) {
  clock_t start, end;
  destroyS(s);
  s = new S;
  if (!readCnf(s, path)) {
    return false;
  }
  // outPutCnf(s);
  PTRL* pl = new PTRL;
  PTRC* pc = new PTRC;
  start = clock();
  bool flag = DPLL(s, pl, pc);
  end = clock();
  if (cnfRes(flag, end - start, path)) {
    cout << "结果已输出到文件: " + path + ".res" << endl;
  }
  delete pl;
  pl = nullptr;
  delete pc;
  pc = nullptr;

  destroyS(s);
  return true;
}
bool sudoku() {
  clock_t start, end;
  constraint("c.cnf");
  destroyS(s);
  s = new S;

  if (!readCnf(s, "c")) {
    return false;
  }
  srand((unsigned)time(NULL));

  //生成终盘
  if (getSudokuFinal(s)) {
    int cnt = 0;
    for (int i = 1; i <= res[0]; i++) {
      if (res[i] > 0) {
        sudokuArr[cnt++] = res[i];
      }
    }
  }
  else {
    cout << "生成终盘失败! ";
    return false;
  }

  //打印终盘
  for (int i = 0; i < doubleSize; i++) {
    if(i&&!(i%9)) cout<<"|";
    cout << ((sudokuArr[i] % 9) ? sudokuArr[i] % 9 : 9) << " ";
  }
  cout<<endl;

  //删除之前添加的单子句,回到初始约束条件(单子句是最后添加的才能这么写，否则需要遍历整个s)
  for (C* c = s->head->next; c->length == 1; c = s->head->next) {
    //添加的单子句的文字一定在最上层
    l[c->head->next->absv] = l[c->head->next->absv]->nextSame;
    destroyC(c);
  }

  //挂载指针数组
  for (int i = 0; i < singleSize; i++) {
    initArr[i] = &sudokuArr[i];
  }
  for (int i = singleSize; i < doubleSize; i++) {
    //第二个数独的第一个九宫格与上一个挂钩
    if ((i - singleSize) / 9 < 3 && (i - singleSize) % 9 < 3) initArr[i] = &sudokuArr[i - 21];
    else initArr[i] = &sudokuArr[i];
  }

  short pos;
  for (int i = 0; i < singleSize; i++) {
    if (i % 9 == 0) { //每一层开头随机
      pos = i + rand() % 9;
      (*initArr[pos]) = 0;
    }
    if (i != pos) {  //每一层先随机挖一个洞
      C* tempSC = createSingle(sudokuArr[i]);   //创建单子句
      insertCL(s, tempSC);
      addSingle(s, tempSC);
    }
  }
  for (int i = singleSize; i < doubleSize; i++) {
    if ((i - singleSize) / 9 < 3 && (i - singleSize) % 9 < 3) continue;   //不创建

    if (i % 9 == 0) {
      //由于上面跳过第一个九宫格，所以前三层不会挖洞
      pos = i + rand() % 9;
      (*initArr[pos]) = 0;
    }
    if (i != pos) {
      C* tempSC = createSingle(sudokuArr[i]);   //创建单子句
      insertCL(s, tempSC);
      addSingle(s, tempSC);
    }
  }
  hole = 15;
  count = 0;
  // cout << endl << "生成数独初盘中...";
  // start = clock();
  getSudokuInit(s);
  // end = clock();
  // cout << endl << end - start;

  int initNum = 0;
  cout<<endl;
  for (int i = 0; i < singleSize; i++) {
    if(i&&!(i%9)) cout<<"|";
    if (*initArr[i]) {
      ++initNum;
      cout << ((sudokuArr[i] % 9) ? sudokuArr[i] % 9 : 9) << " ";
    }
    else cout<<0<<" ";
  }
  for (int i = singleSize; i < doubleSize; i++) {
    if(i&&!(i%9)) cout<<"|";
    if (*initArr[i]) {
      cout << ((sudokuArr[i] % 9) ? sudokuArr[i] % 9 : 9) << " ";
      if ((i - singleSize) / 9 < 3 && (i - singleSize) % 9 < 3) continue;
      ++initNum;
    }
    else cout<<0<<" ";
  }
  cout<<endl;
  if (outPutCnf(s, initNum)) {
    solve("sudoku");
  }
  destroyS(s);
  return true;
}
int main(int argc, char* argv[]) {
  // int op = 1;
  // while (op) {
  //   system("cls");
  //   puts("\n\n");
  //   puts("--------------------- Menu ----------------------\n");
  //   puts("-------------------------------------------------\n");
  //   puts("    	1. SAT     2. Sudoku  	  0. Exit  \n");
  //   puts("-------------------------------------------------\n");
  //   puts("    请选择你的操作[0~2]: ");
  //   scanf("%d", &op);
  //   switch (op) {
  //     case 1: {
  //       puts("请输入算例文件路径名: ");
  //       string path;
  //       cin >> path;
  //       solve(path);
  //       getchar();
  //       getchar();
  //       getchar();
  //       break;
  //     }
  //     case 2: {
  //       sudoku();
  //       getchar();
  //       getchar();
  //       break;
  //     }
  //     case 0: {
  //       cout << "退出系统 " << endl;
  //       getchar();
  //       getchar();
  //       return 0;
  //     }
  //   }
  // }
  if(argc == 1) {
    sudoku();
  }
  else {
    string path = argv[1];
    solve(path);
  }
  return 0;
}
