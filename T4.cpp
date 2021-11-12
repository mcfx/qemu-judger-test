//Test 4
#include<cstdio>
#include<algorithm>
#include<set>

using namespace std;

const int MX=1000000;
int sed=0;
inline int rand(){return sed=(sed*sed*73+sed*233+19260817);}
int main(){
	set<int>S;
	for(int i=0;i<MX;i++)S.insert(rand());
	int ans=0;
	for(set<int>::iterator it=S.begin();it!=S.end();it++)ans^=*it;
	printf("%d\n",ans);
	return 0;
}