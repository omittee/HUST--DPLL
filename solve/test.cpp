#include <iostream>
using namespace std;
//argv[0] = exeName argv[1]开始为参数
int main(int argc, char* argv[]) {
  cout<<argc<<endl;
  for(int i = 0; i < argc; i++) {
    cout<<argv[i]<<"bbb"<<endl;
  }
  cout<<"hhh";
  return 0;
}




