//Test 7-2
#include<cstdio>

using namespace std;
typedef unsigned long long ull;

int P=1000000007;
const int MX=100000000;

int main(){
    ull ans=1;
    for(int i=1;i<MX;i++)ans=ans*i%P;
    printf("%llu\n",ans);
    return 0;
}