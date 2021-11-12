//Test 9-2
#include<cstdio>

using namespace std;
typedef unsigned int uint;

const int MX=1<<10;
uint a[MX][MX],b[MX][MX];

inline uint rand(){static unsigned int sed=0;return (sed=(sed*233+19260421))&(MX-1);}

int main(){
	register int i,j,k;
    for(i=0;i<MX;i++)
    	for(j=0;j<MX;j++)
    		a[i][j]=rand();
    #define A(t) (b[i][k+t]+=a[i][j]*a[j][k+t])
    for(i=0;i<MX;i++)
    	for(j=0;j<MX;j++)
    		for(k=0;k<MX;k+=8)
    			A(0),A(1),A(2),A(3),A(4),A(5),A(6),A(7);
    #undef A
    uint s;
    for(i=0;i<MX;i++)
    	for(j=0;j<MX;j++)
    		s+=a[i][j];
    printf("%u\n",s);
    return 0;
}