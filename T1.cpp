//Test 1
#include<cstdio>

using namespace std;

int main(){
	int a=1000000000,b=1;
	while(a)b<<=1,a--;
	printf("%d\n",b);
	return 0;
}