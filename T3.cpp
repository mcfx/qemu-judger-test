//Test 3
#include<cstdio>

using namespace std;
const int MX=1000;
int G[MX][MX];
int sed=0;
inline int rand(){return sed=(sed*sed*73+sed*233+19260817)&0x0000ffff;}
int main(){
	for(int i=0;i<MX;i++)
		for(int j=0;j<MX;j++)
			G[i][j]=rand();
	for(int i=0;i<MX;i++)
		for(int j=0;j<MX;j++)
			for(int k=0;k<MX;k++)
				if(G[j][k]>G[j][i]+G[i][k])G[j][k]=G[j][i]+G[i][k];
	int ans=0;
	for(int i=0;i<MX;i++)
		for(int j=0;j<MX;j++)
			ans^=G[i][j];
	printf("%d\n",ans);
	return 0;
}