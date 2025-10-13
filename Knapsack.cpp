//maxscore=7534
//g++ -O2 -std=c++11 -fopenmp Knapsack.cpp -o knapsack
#pragma warning(disable:4710)
#pragma warning(disable:4711)
#pragma warning(disable:4820)
#pragma GCC optimize("unroll-loops")
#include <vector>
#include <cfloat>
#include <cstdio>
#include <tuple>
#include <cstring>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <cassert>
#include <random>
#include <queue>
#include <deque>
#include <list>
#include <map>
#include <array>
#include <chrono>
#include <fstream>
#include <functional>
#include <unordered_map>
//#include "hash_map.hpp"
#include <immintrin.h>
#include <omp.h>

#pragma GCC target ("sse4.2")

using namespace std;

typedef unsigned long long ll;

#define ITEM 50
#define BW 50000
#define BW2 10

int val[ITEM] = {360, 83, 59, 130, 431, 67, 230, 52, 93, 125, 670, 892, 600, 38, 48, 147,78, 256, 63, 17, 120, 164, 432, 35, 92, 110, 22, 42, 50, 323, 514, 28,87, 73, 78, 15, 26, 78, 210, 36, 85, 189, 274, 43, 33, 10, 19, 389, 276,312};
      
int wei[ITEM] = {7, 0, 30, 22, 80, 94, 11, 81, 70, 64, 59, 18, 0, 36, 3, 8, 15, 42, 9, 0,42, 47, 52, 32, 26, 48, 55, 6, 29, 84, 2, 4, 18, 56, 7, 29, 93, 44, 71,3, 86, 66, 31, 65, 0, 79, 20, 65, 52, 13};
       
int cap = 850;

ll zoblish_field[ITEM+1][ITEM+1];

struct node {
vector<int>item;
double score;
ll hash;
int t;
}fff[2*BW],ggg[2*BW],ans;

ll xor128() {//xorshift整数乱数
	static unsigned long long rx = 123456789, ry = 362436069, rz = 521288629, rw = 88675123;
	ll rt = (rx ^ (rx << 11));
	rx = ry; ry = rz; rz = rw;
	return (rw = (rw ^ (rw >> 19)) ^ (rt ^ (rt >> 8)));
}

double eval(vector<int>item){
double sumv=0;
double sumw=0;
for(int i=0;i<(int)item.size();i++){  
sumv+=(double)val[item[i]];
sumw+=(double)wei[item[i]];
}     

if((double)cap<sumw){return -1.0;}
return sumv;

}

double get_w(vector<int>item){
double sumv=0;
double sumw=0;
for(int i=0;i<(int)item.size();i++){  
sumv+=(double)val[item[i]];
sumw+=(double)wei[item[i]];
}     

if((double)cap<sumw){return -1.0;}
return sumw;

}

ll calc_hash(vector<int>item){
ll hash=0ll;
for (int i = 0; i < (int)item.size(); i++) {
int num = item[i];
hash ^= zoblish_field[i][num];
}
return hash;
}

double maxscore=0;

double BEAM_SEARCH(node n0) {     
vector<node>dque;
dque.push_back(n0);
    
unordered_map<ll, bool> visited;

int now2=(n0.t)+1;

double score=0;

for (int i = now2; i < ITEM; i++) {
int ks = (int)dque.size();
//#pragma omp parallel for
for (int k = 0; k < ks; k++) {
node temp = dque[k];
for (int j = 0; j < 2; j++) {
node cand = temp;
if (j==0) {
cand.score=eval(cand.item);
//cand.hash=calc_hash(cand.item);
if(cand.score>score){
    score=cand.score;
}
if(cand.score>maxscore){
    maxscore=cand.score;
    ans=cand;
}    
fff[(2 * k) + j] = cand;
}
else{
cand.item.push_back(i);
//cand.hash=calc_hash(cand.item);    
cand.score=eval(cand.item);   
if(cand.score>score){
    score=cand.score;
}
if(cand.score>maxscore){
    maxscore=cand.score;
    ans=cand;
}     
fff[(2 * k) + j] = cand;
}
}
}
dque.clear();
vector<pair<double,int> >vv;
for (int j = 0; j < 2 * ks; j++) {
vv.push_back(make_pair(-fff[j].score,j));    
}
sort(vv.begin(),vv.end());
int push_node=0;
for (int j = 0; push_node < BW ;j++) {  
if(j>=(int)vv.size()){break;}
int p=vv[j].second;
node n1=fff[p];
//if(!visited[n1.hash]){
//visited[n1.hash]=true;
dque.push_back(n1);
push_node++;
//}
}
}

return score;
}

void BEAM_SEARCH2() {     
vector<node>dque;
node n0;
n0.score=0;
n0.hash=0ll;
n0.t=0;    
dque.push_back(n0);

double score=0;
    
unordered_map<ll, bool> visited;

for (int i = 0; i < ITEM; i++) {
int ks = (int)dque.size();
for (int k = 0; k < ks; k++) {
node temp = dque[k];
for (int j = 0; j < 2; j++) {
node cand = temp;
cand.t=i;    
if (j==0) {
cand.score=BEAM_SEARCH(cand);
//cand.hash=calc_hash(cand.item);    
cout<<"scoreA:"<<cand.score<<endl;     
ggg[(2 * k) + j] = cand;
}
else{
cand.item.push_back(i);
//cand.hash=calc_hash(cand.item);    
cand.score=BEAM_SEARCH(cand); 
cout<<"scoreB:"<<cand.score<<endl;   
ggg[(2 * k) + j] = cand;
}
}
}
printf("depth=%d/%d\n",i+1,ITEM);    
dque.clear();
vector<pair<double,int> >vv;    
for (int j = 0; j < 2 * ks; j++) {
vv.push_back(make_pair(-ggg[j].score,j));   
}
sort(vv.begin(),vv.end());
int push_node=0;
for (int j = 0; push_node < BW2 ;j++) {  
if(j>=(int)vv.size()){break;}
int p=vv[j].second;
node n1=ggg[p];
//if(!visited[n1.hash]){
//visited[n1.hash]=true;
dque.push_back(n1);
push_node++;
///}
}
}
}

int main(){

int i1, i2;
for(i1=0;i1<ITEM;++i1){
for(i2=0;i2<ITEM;++i2){
zoblish_field[i1][i2]=xor128();
}
}

BEAM_SEARCH2();

double check=0;

check=eval(ans.item);

cout<<"value:"<<check<<endl;

check=get_w(ans.item);

cout<<"weight:"<<check<<endl;    

for(int i=0;i<(int)ans.item.size();i++){
    cout<<ans.item[i]<<endl;
}
    

return 0;
}