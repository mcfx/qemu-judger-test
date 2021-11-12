//Test 6-1
#include<cstdio>

using namespace std;
const int MX=1<<25;
int a[MX];

inline unsigned int rand(){static unsigned int sed=0;return (sed=(sed*233+19260421))&(MX-1);}

int main(){
    for(int i=0;i<MX;i++)a[rand()]=i;
    return 0;
}