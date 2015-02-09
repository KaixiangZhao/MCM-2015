#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<string>
#include<iostream>
#include<vector>
#include<list>
#include<ctime>
#include<math.h>
//#include<ctime>
using namespace std;
const int lim[]={10,20,25,25,110,150,30};
const int limm[]={8,14,15,15,104,142,24};
const double farrelation=1.0/5.0;
const double baseSkill[7][4]={{0.7,0.7,0.8,0.8},
							  {0.6,0.6,0.7,0.7},
							  {0.6,0.6,0.5,0.5},
							  {0.5,0.5,0.5,0.5},
							  {0.5,0.5,0.4,0.4},
							  {0.4,0.4,0.4,0.4},
							  {0.5,0.5,0.4,0.4}};
const double levelPos[7]={7,6,5,4,3,1,2};
const double levelPd[7]={8,4,2,1.5,1,0.9,0.9};
const double levelRc[7]={1.2,0.7,0.6,0.6,0.3,0.1,0.3};
const double levelTr[7]={0.5,0.6,0.2,0.3,0.1,0.3,0.05};
double PPP=0.65;
double lambda_1=0.00549545,lambda_2=0.000604455;
double sPD[50],sBDR[50],sBDT[50],sBDS[50],sRR[50];
int js=0;
double pd[380];
struct _Block;
struct _People;
struct _Level;
struct _Level{
};
struct _People{
	int id;
	int level;
	double skill[4];
	double time;
	double lpd,npd;
	_Block *block;
	_People(int level):level(level){
		time=rand()*1.0/RAND_MAX;
	}
}*People[400];
struct _Block{
	int id;
	int levelcount[10];
	double skill[4];
	_People *employe[10][15];
	string name;
	_Block(string name,int level[]):name(string(name)){
		for (int i=0;i<7;i++)
			levelcount[i]=level[i];
		memset(employe,0,sizeof(employe));
	}
	//levelcount SM JM S IS E IE A 
}*Block[50];

vector< pair<_People**,_Block*> > Level[10];
vector<int> tree[50];
list<int> l;
int disT[50][50];
int bTree[50][50];
int ubTree[50][50];
int reGraph[400][400];
int levelCoV[7],levelCoT[7];
int leave[7],totleave[7],change[7],elevate[7],hire[7];
int totL=0;
//double lastCohesion[50],newCohesion[50];
double leaveP0[400],leaveP[400],aggre[400],eleP[7];
double newLeave[400],lastLeave[400],eleP0[400];
double gRand()
{
    static double V1, V2, S;
    static int phase = 0;
    double X;     
    if ( phase == 0 ) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;
             
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);
         
        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);         
    phase = 1 - phase; 
    return X;
}
int posb(double pos){
	double t;
	t=rand()*1.0/RAND_MAX;
	return t<=pos?1:0;
}
void Floy(){
	for (int i=0;i<35;i++)
		for (int j=0;j<35;j++)
			if (ubTree[i][j])
				disT[i][j]=1;
			else
				disT[i][j]=-1;
	for (int i=0;i<35;i++)
		disT[i][i]=0;
	for (int k=0;k<35;k++)
		for (int i=0;i<35;i++)
			for (int j=0;j<35;j++)
				if (disT[i][k]>=0&&disT[k][j]>=0)
					if (disT[i][k]+disT[k][j]<disT[i][j]||disT[i][j]<0)
						disT[i][j]=disT[i][k]+disT[k][j];
}
double A(int i,int j){
	int n=-(People[i]->level-People[j]->level);
	return n>0?(1.0/(2+n)):((1.0-n)/(2.0-n));
}
double F(int i,int j){
	int G=0,N=0;
	for (int k=0;k<380;k++)
	{
		if (reGraph[i][k]==1&&reGraph[j][k]==1&&k!=i&&k!=j)
			G++;
		if (reGraph[i][k]==1&&k!=i)
			N++;
	}
//	cout<<i<<":"<<j<<" "<<G<<" "<<N<<endl;
	return (G+1)*1.0/N;			
}
double Sc(double a[4],double b[4]){
	double ret=0;
	for (int i=0;i<4;i++)
		ret+=(a[i]-b[i])*(a[i]-b[i]);
	return ret;
}
double sSc(double a[4],double b[4]){
	double ret=0;
	for (int i=0;i<4;i++)
		if (a[i]-b[i]<0)
			ret+=(a[i]-b[i])*(a[i]-b[i]);
	return ret;
}
void _aggre(){
	memset(aggre,0,sizeof(aggre));
	for (int i=0;i<380;i++)
		if (People[i]!=NULL)
		{
			double tot=0,sel=0;
			for (int j=0;j<380;j++)
				if (reGraph[i][j]==1)
					tot+=1.0;
			if (tot<2) continue;
			tot=tot*(tot-1)/2;			
			for (int j=0;j<380;j++)
				for (int k=j+1;k<380;k++)
					if (People[j]!=NULL&&People[k]!=NULL)
						if (reGraph[i][j]==1&&reGraph[i][k]==1&&reGraph[j][k]==1)
							sel+=1;
			aggre[i]=sel/tot;
		}

}
int totV=0;
void _statis(){
	memset(levelCoV,0,sizeof(levelCoV));
	memset(levelCoT,0,sizeof(levelCoT));
	memset(eleP,0,sizeof(eleP));
	totV=0;
	for (int i=0;i<7;i++)
		for (int j=0;j<Level[i].size();j++)
			if (*(Level[i][j].first)==NULL)
			{
				levelCoV[i]++;
				totV++;
			}
			else
				if ((*(Level[i][j].first))->time>=1.0)
					levelCoT[i]++;
				
	for (int i=1;i<6;i++)
		eleP[i]=levelCoV[i-1]*1.0/levelCoT[i];
	for (int i=1;i<6;i++)
		if (eleP[i]>=1)
			eleP[i]=1.0;
}
void _prod(){
	for (int i=0;i<380;i++)
		if (People[i]!=NULL)
			People[i]->lpd=levelPd[People[i]->level]/12*(1+aggre[i])*(1-leaveP[i])*(min(People[i]->time,1.0)+1)/2;
}
void _leave(){
	for (int i=0;i<380;i++)
		if (People[i]!=NULL)
			leaveP[i]=0.869*leaveP0[i]+(1-eleP[People[i]->level])*lambda_1+lastLeave[i]*lambda_2;
}
void init(){
	srand((unsigned)time(NULL));
	lambda_1=370*PPP/12*0.107/247;
	lambda_2=370*PPP/12*0.022/202;
	for (int i=0;i<400;i++) delete People[i];
	for (int i=0;i<50;i++) delete Block[i];
	for (int i=0;i<10;i++) Level[i].clear();
	for (int i=0;i<50;i++) tree[i].clear();
	memset(People,0,sizeof(People));
	memset(Block,0,sizeof(Block));
	memset(totleave,0,sizeof(totleave));
	int t1[]={4,0,0,0,0,0,0};
	Block[0]=new _Block("CEO",t1);
	int t2[]={1,3,0,0,0,0,0};
	Block[1]=new _Block("Research",t2);
	int t3[]={1,2,0,1,4,5,1};
	Block[2]=new _Block("CIO",t3);
	int t4[]={1,2,0,1,4,5,1};
	Block[3]=new _Block("CFO",t4);
	int t5[]={0,1,0,1,2,0,0};
	Block[4]=new _Block("HR",t5);
	int t6[]={1,3,0,0,0,0,0};
	Block[5]=new _Block("VP",t6);
	int t7[]={1,3,0,0,0,0,0};
	Block[6]=new _Block("Facilities",t7);
	int t8[]={1,3,0,0,0,0,0};
	Block[7]=new _Block("Sales_Marketing",t8);
	int t9[]={0,0,0,1,5,6,2};
	Block[8]=new _Block("Networks",t9);
	Block[9]=new _Block("Information",t9);
	Block[10]=new _Block("Programmer_Manger",t9);
	Block[12]=new _Block("Plant_Blue",t9);
	Block[15]=new _Block("World_Wide",t9);
	Block[16]=new _Block("Internet",t9);
	int t10[]={0,3,1,2,0,6,2};
	Block[11]=new _Block("Production_Manager",t10);
	int t11[]={0,0,0,1,5,7,1};
	Block[13]=new _Block("Plant_Green",t11);
	Block[14]=new _Block("Regional",t11);
	int t12[]={0,0,4,0,0,0,0};
	Block[17]=new _Block("Director1",t12);
	Block[18]=new _Block("Director2",t12);
	Block[19]=new _Block("Director3",t12);
	Block[20]=new _Block("Director4",t12);
	Block[21]=new _Block("Director5",t12);
	Block[22]=new _Block("Director6",t12);
	int t13[]={0,0,0,1,5,7,1};
	Block[23]=new _Block("Branch_A",t13);
	Block[24]=new _Block("Branch_B",t13);
	Block[25]=new _Block("Branch_C",t13);
	Block[26]=new _Block("Branch_D",t13);
	Block[27]=new _Block("Branch_E",t13);
	Block[28]=new _Block("Branch_F",t13);
	Block[29]=new _Block("Branch_G",t13);
	Block[30]=new _Block("Branch_H",t13);
	Block[31]=new _Block("Branch_I",t13);
	Block[32]=new _Block("Branch_J",t13);
	Block[33]=new _Block("Branch_K",t13);
	Block[34]=new _Block("Branch_L",t13);	
	//---------------------------------------
	tree[0].push_back(1);
	tree[0].push_back(2);
	tree[0].push_back(3);
	tree[0].push_back(4);
	tree[0].push_back(5);
	tree[0].push_back(6);
	tree[0].push_back(7);
	tree[1].push_back(8);
	tree[1].push_back(9);
	tree[5].push_back(10);
	tree[5].push_back(11);
	tree[6].push_back(12);
	tree[6].push_back(13);
	tree[7].push_back(14);
	tree[7].push_back(15);
	tree[7].push_back(16);
	tree[11].push_back(17);
	tree[11].push_back(18);
	tree[11].push_back(19);
	tree[11].push_back(20);
	tree[11].push_back(21);
	tree[11].push_back(22);
	tree[17].push_back(23);
	tree[17].push_back(24);
	tree[18].push_back(25);
	tree[18].push_back(26);
	tree[19].push_back(27);
	tree[19].push_back(28);
	tree[19].push_back(29);
	tree[20].push_back(30);
	tree[21].push_back(31);
	tree[21].push_back(32);
	tree[22].push_back(33);
	tree[22].push_back(34);

	//---------------------------------------
	for (int i=0;i<35;i++)
	{
		Block[i]->id=i;
		for (int j=0;j<7;j++)
			for (int k=0;k<Block[i]->levelcount[j];k++)
				Level[j].push_back(make_pair(&(Block[i]->employe[j][k]),Block[i]));
		for (int j=0;j<4;j++)
			Block[i]->skill[j]=gRand()*0.02;
	}
	js=0;
	for (int i=0;i<7;i++)
	{
		for (int j=0;j<limm[i];j++)
		{
			People[js++]=new _People(i);
			People[js-1]->id=js-1;
			int k;
			while (*(Level[i][k=(rand()%Level[i].size())].first)!=NULL);
			*(Level[i][k].first)=People[js-1];		
			People[js-1]->block=Level[i][k].second;					
			for (int k=0;k<4;k++)
				People[js-1]->skill[k]=baseSkill[i][k]+0.02*gRand();
		/*	cout<<i<<" ";
			for (int k=0;k<4;k++)
				cout<<People[js-1]->skill[k]<<" ";
			system("pause");*/
			People[js-1]->lpd=levelPd[i]/12;
			People[js-1]->time=rand()*2.0/RAND_MAX;
		}			
	}
	memset(bTree,0,sizeof(bTree));
	memset(ubTree,0,sizeof(ubTree));
	for (int i=0;i<35;i++)	
		for (int j=0;j<tree[i].size();j++)
		{
			bTree[i][tree[i][j]]=1;
			ubTree[i][tree[i][j]]=ubTree[tree[i][j]][i]=1;
		}		
	Floy();
	memset(reGraph,0,sizeof(reGraph));

	for (int i=0;i<380;i++)
		for (int j=0;j<380;j++)
			if (People[i]!=NULL&&People[j]!=NULL&&i!=j)
			{
				int bidi=People[i]->block->id;
				int bidj=People[j]->block->id;
				if (bidi==bidj)
					reGraph[i][j]=reGraph[j][i]=1;
				int k=posb(farrelation*1.0/disT[bidi][bidj]);
				if (k==1)
					reGraph[i][j]=reGraph[j][i]=1;	
			}
			
	
	_statis();
	for (int i=0;i<380;i++)
		if (People[i]!=NULL)
		{
			if (People[i]->level==6)
				continue;
			leaveP0[i]=0.01131+lambda_1*(1-eleP[People[i]->level])+lambda_2*0.627;
			lastLeave[i]=0.627;
			eleP0[i]=eleP[People[i]->level];
		}			
//	system("pause");

	cout<<js<<endl;
}
void simulate(){
	int simulateM=24;
	memset(sPD,0,sizeof(sPD));
	memset(sBDT,0,sizeof(sBDT));
	memset(sRR,0,sizeof(sRR));
	memset(sBDR,0,sizeof(sBDR));
	memset(sBDS,0,sizeof(sBDS));
	for (int jm=0;jm<simulateM;jm++){		
		memset(leave,0,sizeof(leave));
		memset(change,0,sizeof(change));
		memset(elevate,0,sizeof(elevate));
		memset(hire,0,sizeof(hire));	
		memset(newLeave,0,sizeof(newLeave));
		_aggre();
		_statis();
		_leave();		
		_prod();	
		for (int i=0;i<380;i++)
			if (People[i]!=NULL)
			{
				sPD[jm]+=People[i]->lpd;
				int le=People[i]->level;
				sBDT[jm]+=levelTr[le]/12;
				sBDS[jm]+=levelPd[le]/12;
			}
	/*	for (int i=0;i<380;i++)
			if (People[i]!=NULL)
				cout<<lastLeave[i]<<endl;
		system("pause");*/		
		for (int le=0;le<6;le++){
			for (int i=0;i<Level[le].size();i++)
				if (*(Level[le][i].first)!=NULL)
				{
					if (jm%12==0)
					{
						double minv,tt[4];
						int minj;
						minj=i;
						for (int k=0;k<4;k++)
							tt[k]=baseSkill[le][k]+Level[le][i].second->skill[k];
						minv=Sc((*(Level[le][i].first))->skill,tt);
						for (int j=0;j<Level[le].size();j++)
							if (*(Level[le][j].first)==NULL)
							{
								double tmp[4],tSc;
								for (int k=0;k<4;k++)
									tmp[k]=baseSkill[le][k]+Level[le][j].second->skill[k];
								tSc=Sc((*(Level[le][i].first))->skill,tmp);
								if (tSc<minv)
								{
									minv=tSc;
									minj=j;
								}
							}
						if (minj!=i)
						{
							int pid=(*(Level[le][i].first))->id;
							*(Level[le][minj].first)=*(Level[le][i].first);
							*(Level[le][i].first)=NULL;
							People[pid]->block=Level[le][minj].second;
							int bidi=People[pid]->block->id;		
							for (int j=0;j<380;j++)
								if (People[j]!=NULL&&pid!=j)
								{
									int bidj=People[j]->block->id;
									if (bidi==bidj)
										reGraph[pid][j]=reGraph[j][pid]=1;
								}	
							change[le]++;
							continue;
						}
					}
					/*zouren*/
					int pid=(*(Level[le][i].first))->id;
					leaveP[pid]=PPP/12;
					int k;
					if (le>=1&&le<=3)
						k=posb(leaveP[pid]*3);
					else
						k=posb(leaveP[pid]);
					if (k!=1) continue;
				
					*(Level[le][i].first)=NULL;				
					for (int j=0;j<380;j++)
						if (reGraph[j][pid]==1)
						{
							newLeave[j]+=(A(j,pid)+F(j,pid))*0.5;
							reGraph[j][pid]=reGraph[pid][j]=0;
						}				
					delete People[pid];
					People[pid]=NULL;		
					leave[le]++;
					
				}
		//cout<<jm<<" 1"<<endl;
		//elevate
			if (le<=4)
				for (int i=0;i<Level[le].size();i++)
					if (*(Level[le][i].first)==NULL)
					{
						int suitj=-1;
						double suitv=-1;
						for (int j=0;j<Level[le+1].size();j++)
							if (*(Level[le+1][j].first)!=NULL&&(*(Level[le+1][j].first))->time>=1)
							{
								double tmp[4],tSc;
								for (int k=0;k<4;k++)
									tmp[k]=baseSkill[le][k]+Level[le][i].second->skill[k];
								tSc=sSc((*(Level[le+1][j].first))->skill,tmp);
								if (tSc>0.001)
									continue;
							/*	for (int op=0;op<4;op++)
								 	cout<<tmp[op]<<" ";
								 	cout<<endl;
								cout<<(*(Level[le+1][j].first))->id<<" "<<(*(Level[le+1][j].first))->level<<endl;
								for (int op=0;op<4;op++)
								 	cout<<(*(Level[le+1][j].first))->skill[op]<<" ";
								 	cout<<endl;
								system("pause");*/
								tSc=0.3805*(*(Level[le+1][j].first))->skill[0]+0.0759*(*(Level[le+1][j].first))->skill[1]+
									0.2371*(*(Level[le+1][j].first))->skill[2]+0.3030*(*(Level[le+1][j].first))->skill[3];
								if (tSc>suitv)
								{
									suitv=tSc;
									suitj=j;
								}
							}
						if (suitj<0) continue;
						*(Level[le][i].first)=*(Level[le+1][suitj].first);
						(*(Level[le][i].first))->block=Level[le][i].second;
						(*(Level[le][i].first))->level=le;
						(*(Level[le][i].first))->time=0.0;
						*(Level[le+1][suitj].first)=NULL;
						int pid=(*(Level[le][i].first))->id;
						for (int j=0;j<380;j++)
							if (People[j]!=NULL&&j!=pid)
							{
								if (People[j]->block->id==People[pid]->block->id)
									reGraph[pid][j]=reGraph[j][pid]=1;						
							}	
						elevate[le]++;							
					}
	//	cout<<jm<<" 2"<<endl;
	//  hire
			for (int i=0;i<Level[le].size();i++)
				if (*(Level[le][i].first)==NULL)
				{
					int K=posb(370*0.10*1.0/totV);
					if (K!=1) continue;
					K=posb(1.0/levelPos[le]);
					if (K==1)
					{
						int j;
						for (j=0;j<370;j++)
							if (People[j]==NULL)
							{
								People[j]=new _People(le);
								for (int k=0;k<4;k++)
									People[j]->skill[k]=baseSkill[le][k]+0.02*gRand();
								People[j]->id=j;
								break;
							}
						*(Level[le][i].first)=People[j];
						People[j]->block=Level[le][i].second;
						People[j]->time=0.0;
						People[j]->lpd=levelPd[le]/12;
						for (int k=0;k<380;k++)
							if (People[k]!=NULL&&k!=j)
							{
								int bidi=People[j]->block->id;
								int bidj=People[k]->block->id;
								if (bidi==bidj)
								reGraph[k][j]=reGraph[j][k]=1;			
							}
						hire[le]++;
						sBDR[jm]+=levelRc[le];
					}
				}
	//	cout<<jm<<" 3"<<endl;
		}
		for (int i=0;i<380;i++)
			if (People[i]!=NULL)
			{
				People[i]->time=People[i]->time+0.02;
				if (jm==12)
					for (int j=0;j<4;j++)
						People[i]->skill[j]+=0.02*gRand();
			}
		for (int i=0;i<380;i++)
			lastLeave[i]=newLeave[i];
		for (int le=0;le<7;le++)
			totleave[le]+=leave[le];
		int count=0;
		for (int i=0;i<380;i++)
			if (People[i]!=NULL)
			 count++;
		sRR[jm]=count;
		if (jm!=23) continue;
		cout<<"NO change leave elevate hire"<<endl;
		for (int le=0;le<7;le++)
		{
			cout<<le<<" "<<change[le]<<" "<<leave[le]<<" "<<elevate[le]<<" "<<hire[le]<<endl;
		}		
		_statis();
		for (int le=0;le<7;le++)
			cout<<lim[le]-levelCoV[le]<<" ";
		cout<<endl;
		for (int le=0;le<7;le++)
			cout<<totleave[le]<<" ";
		cout<<endl;
		cout<<count<<endl;
		count=0;
		for (int i=0;i<380;i++)
			for (int j=0;j<380;j++)
				if (reGraph[i][j]==1)
				 count++;
		cout<<count<<endl;
	}
	
}
int main(){
	PPP=0.18;
	double ansT=0,ansR=0,ansS=0,ansTot=0;
	int tot=1;
	for (int op=0;op<tot;op++)
	{
		init();
		simulate();
		double ret=0,retT=0,retR=0,retS=0;
		for (int i=0;i<25;i++)
		{
			ret+=sBDT[i]+sBDR[i]+sBDS[i];
			retT+=sBDT[i];
			retR+=sBDR[i];
			retS+=sBDS[i];
		}			
		cout<<ret<<endl;
		cout<<"T : "<<(retT)<<endl;
		cout<<"S : "<<(retS)<<endl;
		cout<<"R : "<<(retR)<<endl;
		ansTot+=ret;
		ansT+=retT;
		ansS+=retS;
		ansR+=retR;
	}
	cout<<"Tot : "<<(ansTot/=tot)<<endl;
	cout<<"T : "<<(ansT/=tot)<<endl;
	cout<<"S : "<<(ansS/=tot)<<endl;
	cout<<"R : "<<(ansR/=tot)<<endl;
	
/*	freopen("task5.out","a+",stdout);
	for (int i=0;i<24;i++)
		cout<<sRR[i]<<" ";
	cout<<endl;*/
	fclose(stdout);
	freopen("pd.out","a+",stdout);
	for (int i=0;i<25;i++)
	cout<<sPD[i]<<" ";
	cout<<endl;
	fclose(stdout);
/*	freopen("bd_25.out","a+",stdout);
	for (int i=0;i<25;i++)
	cout<<sBDR[i]<<" ";
	cout<<endl;
	for (int i=0;i<25;i++)
	cout<<sBDT[i]<<" ";
	cout<<endl;
	fclose(stdout);*/

}
/*	init();
	freopen("2Pythonnode","w",stdout);
	int v[400],bjs[400];
	memset(v,0,sizeof(v));
	memset(bjs,0,sizeof(bjs));
	for (int i=0;i<35;i++)
		if(i%5==0)
		for (int j=0;j<7;j++)
			for (int k=0;k<Block[i]->levelcount[j];k++)
				if (Block[i]->employe[j][k]!=NULL)
				{
					v[Block[i]->employe[j][k]->id]=1;
					printf("%d %d %d\n",Block[i]->employe[j][k]->id,i,Block[i]->employe[j][k]->level);
				}	
	fclose(stdout);
	freopen("2Pythonlink","w",stdout);
	for (int i=0;i<380;i++)
		for (int j=i+1;j<380;j++)
			if (v[i]==1&&v[j]==1&&reGraph[i][j])
			{
				printf("%d %d %d\n",i,j,People[i]->block->id==People[j]->block->id);
				bjs[i]++;
				bjs[j]++;
			}
	fclose(stdout);
	freopen("2Pythonbjs","w",stdout);
	for (int i=0;i<380;i++)
		if (v[i]==1)
			printf("%d %d\n",i,bjs[i]);
	fclose(stdout);*/
/*	l.push_back(0);
	while (!l.empty()){
		cout<<l.front()<<endl;
		for (int i=0;i<tree[l.front()].size();i++)
			l.push_back(tree[l.front()][i]);
		l.pop_front();
	}
	system("pause");*/
/*	for (int i=0;i<100;i++)
	cout<<gaussrand()<<endl;*/
/*	int test[10];
	memset(test,0,sizeof(test));
	for (int i=0;i<35;i++)
		for (int j=0;j<7;j++)
			test[j]+=Block[i]->levelcount[j];
	for (int i=0;i<7;i++)
		cout<<test[i]<<endl;*/
	
/*	for (int i=0;i<js;i++)
		cout<<People[i]->level<<endl;*/
/*	freopen("yo","w",stdout);
	for (int i=0;i<7;i++)
		for (int j=0;j<Level[i].size();j++)
			if (*(Level[i][j].first)==NULL)
				printf("Block %d: NULL\n",Level[i][j].second->id);
			else
				printf("Block %d: %d, ID %d %d\n",Level[i][j].second->id,i,(*(Level[i][j].first))->id,(*(Level[i][j].first))->level);

	fclose(stdout);*/
/*	freopen("CON","w",stdout);
	while (1)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		cout<<People[x]->block->id<<" "<<People[y]->block->id<<endl;
		cout<<relation[x][y]<<endl;
	}*/
	/*	while(1){
		cout<<posb(farrelation)<<endl;
		system("pause");
	}*/
	
	/*	while(1)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		cout<<disT[x][y]<<" "<<disT[y][x]<<endl;
	}*/
	/*	while(1)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		cout<<reGraph[x][y]<<" "<<reGraph[y][x]<<endl;
	}*/
	/*	while(1)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		cout<<A(x,y)<<" "<<A(y,x)<<endl;
	}*/
	/*	while(1)
	{
		int x,y;
		scanf("%d%d",&x,&y);
		cout<<F(x,y)<<" "<<F(y,x)<<endl;
	}*/
		/*while(1)
	{
		int x;
		cin>>x;
		for (int i=0;i<4;i++)
			cout<<People[x]->skill[i]<<" ";
		cout<<endl;
		system("pause");
	}*/
		/*for (int i=0;i<35;i++)
	{
		int n=0,m=0,bj=0;
		double pd=0,S=0,lead=0;
		lastCohesion[i]=0;
		for (int j=0;j<7;j++)
			for (int k=0;k<Block[i]->levelcount[j];k++)
				if (Block[i]->employe[j][k]!=NULL)
				{
					pd+=Block[i]->employe[j][k]->lpd;
					n++;
				}
		lastCohesion[i]=pd*1.0/n;
		for (int j=0;j<7;j++)
			for (int k=0;k<Block[i]->levelcount[j];k++)
				if (Block[i]->employe[j][k]!=NULL)
					for (int o=0;o<7;o++)
						for (int p=0;p<Block[i]->levelcount[o];p++)
							if (Block[i]->employe[o][p]!=NULL)
							{
								S+=(A(Block[i]->employe[j][k]->id,Block[i]->employe[o][p]->id)+
									F(Block[i]->employe[j][k]->id,Block[i]->employe[o][p]->id))*0.50;
								m++;
							}
		lastCohesion[i]+=S*1.0/m;				
		for (int j=0;j<7&&bj!=1;j++)
			for (int k=0;k<Block[i]->levelcount[j];k++)
				if (Block[i]->employe[j][k]!=NULL){
					for (int o=0;o<4;o++)
						lead=Block[i]->employe[j][k]->skill[o];	
					bj=1;	
					break;			
				}		
		lastCohesion[i]+=lead*1.0/4;			
	}*/
		/*	for (int i=0;i<380;i++)
			if (People[i]!=NULL)
				{
					printf("%d %d %.5lf %.5lf\n",i,People[i]->level,leaveP[i],People[i]->lpd);
					system("pause");
				}			*/
