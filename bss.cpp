
//maxscore=7534
//g++ -O2 -std=c++11 -fopenmp bss.cpp -o bss
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
#define BW 10000
#define BW2 5

int val[ITEM] = {360, 83, 59, 130, 431, 67, 230, 52, 93, 125, 670, 892, 600, 38, 48, 147,78, 256, 63, 17, 120, 164, 432, 35, 92, 110, 22, 42, 50, 323, 514, 28,87, 73, 78, 15, 26, 78, 210, 36, 85, 189, 274, 43, 33, 10, 19, 389, 276,312};
      
int wei[ITEM] = {7, 0, 30, 22, 80, 94, 11, 81, 70, 64, 59, 18, 0, 36, 3, 8, 15, 42, 9, 0,42, 47, 52, 32, 26, 48, 55, 6, 29, 84, 2, 4, 18, 56, 7, 29, 93, 44, 71,3, 86, 66, 31, 65, 0, 79, 20, 65, 52, 13};
       
int cap = 850;

struct node {
ll item[((ITEM-1)/64)+1];
double score;
double sumw;
double score2;
int t;
}fff[2*BW],ggg[2*BW],ans;

ll xor128() {//xorshift整数乱数
	static unsigned long long rx = 123456789, ry = 362436069, rz = 521288629, rw = 88675123;
	ll rt = (rx ^ (rx << 11));
	rx = ry; ry = rz; rz = rw;
	return (rw = (rw ^ (rw >> 19)) ^ (rt ^ (rt >> 8)));
}

double eval(double sumv,double sumw,int k){

if(0<=k&&k<ITEM){
sumv+=(double)val[k];
sumw+=(double)wei[k];
}    

if((double)cap<sumw){return -1.0;}
return sumv;

}

double get_w(double sumw,int k){
if(0<=k&&k<ITEM){    
sumw+=(double)wei[k];
}
if((double)cap<sumw){return (double)cap+1.0;}
return sumw;

}

double maxscore=0;

double BEAM_SEARCH(node n0) {     
vector<node>dque;
dque.push_back(n0);

int now2=(n0.t)+1;

double score=0;

for (int i = now2; i < ITEM; i++) {
int ks = (int)dque.size();
#pragma omp parallel for
for (int k = 0; k < ks; k++) {
node temp = dque[k];
for (int j = 0; j < 2; j++) {
node cand = temp;
if (j==0) {
cand.score=eval(cand.score,cand.sumw,-1);
cand.sumw=get_w(cand.sumw,-1);       
fff[(2 * k) + j] = cand;
}
else{
cand.item[i/64] |= (((ll)(1))<<((i)%64));
cand.score=eval(cand.score,cand.sumw,i);
cand.sumw=get_w(cand.sumw,i);    
fff[(2 * k) + j] = cand;
}
}
}
dque.clear();
vector<pair<double,int> >vv;
for (int j = 0; j < 2 * ks; j++) {
vv.push_back(make_pair(-fff[j].score,j));
if(fff[j].score>score){
    score=fff[j].score;
}
if(fff[j].score>maxscore){
    maxscore=fff[j].score;
    ans=fff[j];
}     
}
sort(vv.begin(),vv.end());
int push_node=0;
for (int j = 0; push_node < BW ;j++) {  
if(j>=(int)vv.size()){break;}
int p=vv[j].second;
node n1=fff[p];
dque.push_back(n1);
push_node++;
}
}

return score;
}

void BEAM_SEARCH2() {     
vector<node>dque;
node n0;
n0.score=0;
n0.sumw=0;
n0.t=0;
for(int i=0;i<((ITEM-1)/64)+1;i++){
n0.item[i]=0ll;    
}    
dque.push_back(n0);

double score=0;

for (int i = 0; i < ITEM-1; i++) {
int ks = (int)dque.size();
for (int k = 0; k < ks; k++) {
node temp = dque[k];
for (int j = 0; j < 2; j++) {
node cand = temp;
cand.t=i;    
if (j==0) {
cand.score=eval(cand.score,cand.sumw,-1);
cand.sumw=get_w(cand.sumw,-1);    
cand.score2=BEAM_SEARCH(cand);    
cout<<"scoreA:"<<cand.score2<<endl;     
ggg[(2 * k) + j] = cand;
}
else{
cand.item[i/64] |= (((ll)(1))<<((i)%64)); 
cand.score=eval(cand.score,cand.sumw,i);    
cand.sumw=get_w(cand.sumw,i);
cand.score2=BEAM_SEARCH(cand); 
cout<<"scoreB:"<<cand.score2<<endl;   
ggg[(2 * k) + j] = cand;
}
}
}
printf("depth=%d/%d\n",i+1,ITEM-1);    
dque.clear();
vector<pair<double,int> >vv;    
for (int j = 0; j < 2 * ks; j++) {
vv.push_back(make_pair(-ggg[j].score2,j));   
}
sort(vv.begin(),vv.end());
int push_node=0;
for (int j = 0; push_node < BW2 ;j++) {  
if(j>=(int)vv.size()){break;}
int p=vv[j].second;
node n1=ggg[p];
dque.push_back(n1);
push_node++;
}
}
}

vector<int> get_item(ll item[((ITEM-1)/64)+1]) {
    vector<int> result;
    for (int i = 0; i < ITEM; i++) {
        if (((item[i/64] >> (i % 64)) & 1) == 1) {
            result.push_back(i);
        }
    }
    return result;
}

int main(){

vector<tuple<int,double,int> >v;

for(int i=0;i<ITEM;i++){
if(wei[i]==0){    
v.push_back(make_tuple(val[i],0,i));
}
else{
v.push_back(make_tuple(0,(double)val[i]/(double)wei[i],i));    
}    
}

sort(v.begin(),v.end());
reverse(v.begin(), v.end());

int val2[ITEM];
int wei2[ITEM];

memcpy(val2,val,sizeof(val2));
memcpy(wei2,wei,sizeof(wei));

for(int i=0;i<ITEM;i++){
val[i]=val2[get<2>(v[i])];
wei[i]=wei2[get<2>(v[i])];    
}        

BEAM_SEARCH2();

double valuex=0;
double weightx=0;  

int select[ITEM]={0};

vector<int>items;

items=get_item(ans.item);

for(int i=0;i<(int)items.size();i++){
    for(int j=0;j<ITEM;j++){
        if(val2[j]==val[items[i]]&&wei2[j]==wei[items[i]]&&select[j]==0){
            select[j]=1;
            cout<<j<<endl;
            valuex+=(double)val2[j];
            weightx+=(double)wei2[j];
            break;
        }
    }    
}

   
cout<<"value:"<<valuex<<endl;

cout<<"weight:"<<weightx<<endl;   
    

return 0;
}
