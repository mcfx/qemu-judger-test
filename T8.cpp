//Test 8
#include<cstdio>

using namespace std;

const int MX=20000000;

int main(){
    double ans=0.61234567898765,t=1,s=0;
    for(int i=1;i<MX;i++)s+=(t*=ans);
    printf("%f\n",s);
    return 0;
}