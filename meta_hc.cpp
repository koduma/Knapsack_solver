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

typedef unsigned long long ull;

#define ITEM 50
#define BW 50000
#define BW2 10

struct node {
vector<int>item;
double score;
};

int val[ITEM] = {360, 83, 59, 130, 431, 67, 230, 52, 93, 125, 670, 892, 600, 38, 48, 147,78, 256, 63, 17, 120, 164, 432, 35, 92, 110, 22, 42, 50, 323, 514, 28,87, 73, 78, 15, 26, 78, 210, 36, 85, 189, 274, 43, 33, 10, 19, 389, 276,312};
      
int wei[ITEM] = {7, 0, 30, 22, 80, 94, 11, 81, 70, 64, 59, 18, 0, 36, 3, 8, 15, 42, 9, 0,42, 47, 52, 32, 26, 48, 55, 6, 29, 84, 2, 4, 18, 56, 7, 29, 93, 44, 71,3, 86, 66, 31, 65, 0, 79, 20, 65, 52, 13};
       
int cap = 850;

ull xor128() {//xorshift整数乱数
	static unsigned long long rx = 123456789, ry = 362436069, rz = 521288629, rw = 88675123;
	ull rt = (rx ^ (rx << 11));
	rx = ry; ry = rz; rz = rw;
	return (rw = (rw ^ (rw >> 19)) ^ (rt ^ (rt >> 8)));
}
int rnd(int mini, int maxi) {
	static mt19937 mt((int)time(0));
	uniform_int_distribution<int> dice(mini, maxi);
	return dice(mt);
}
double d_rnd() {
	static mt19937 mt((int)time(0));
	uniform_real_distribution<double> dice(0, 1.0);
	return dice(mt);
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
node solve(node travel) {
    
    double MLEN=-1.0;
    
    node no1;
       
    int discover=0;
    
    int counter=0;
    
    vector<node>elite;
    
    int iter=0;

    travel.score=eval(travel.item);
    
    while(1){
        
    if(travel.score>7000){
        elite.push_back(travel);
    }
	    
    int choice=-1;
	    
    if((int)elite.size()>0){        
    int r=rnd(0,(int)elite.size()-1);    
    travel=elite[r];
    choice=r;
    }
        
    node old=travel;
    node news=travel;
     
    for(int u=0;u<2;u++){

    int rd=rnd(0,1);
    if(rd==1&&u==0){
    int looked[ITEM]={0};
    for(int i=0;i<(int)news.item.size();i++){
    looked[news.item[i]]=1;     
    }
    while(1){    
    int c=rnd(0,ITEM-1);
    if(looked[c]==0){    
    looked[c]=1;
    news.item.push_back(c);
    break;    
    }    
    int br=1;    
    for(int i=0;i<ITEM;i++){
    if(looked[i]==0){br=0;break;}    
    }
    if(br==1){break;}    
    }    
    }
    else if(rd==1&&u==1){
    int looked[ITEM]={0};
    for(int i=0;i<(int)news.item.size();i++){
    looked[news.item[i]]=1;     
    }
    while(1){    
    int c=rnd(0,ITEM-1);
    if(looked[c]==1){    
    looked[c]=0;
    int d=0;    
    for(int i=0;i<(int)news.item.size();i++){
    if(c==news.item[i]){d=i;break;}    
    }
    news.item.erase(news.item.begin() + d);    
    break;    
    }    
    int br=1;    
    for(int i=0;i<ITEM;i++){
    if(looked[i]==1){br=0;break;}    
    }
    if(br==1){break;}    
    }
    }        
    }

    news.score=eval(news.item);
    old.score=travel.score;
        
    if(news.score>old.score){     
    discover++;
    travel=news;
    if(choice>=0){
    elite[choice]=news;
    }
    }   
    if(MLEN<travel.score){
    MLEN=travel.score;
    no1=travel;   
    }
    iter++;
    if(iter%10000000==0){break;}
    }
return no1;
}

node solve2(node travel) {
    
        
    double MLEN=-1.0;
    
    node no1;
       
    int discover=0;
    
    int counter=0;
    
    vector<node>elite;
    
    int iter=0;

    travel.score=eval(travel.item);
    
    while(1){
        
    if(travel.score>7000){
        elite.push_back(travel);
    }
	    
    int choice=-1;
	    
    if((int)elite.size()>0){        
    int r=rnd(0,(int)elite.size()-1);    
    travel=elite[r];
    choice=r;
    }
        
    node old=travel;
    node news=travel;
     
    for(int u=0;u<2;u++){

    int rd=rnd(0,1);
    if(rd==1&&u==0){
    int looked[ITEM]={0};
    for(int i=0;i<(int)news.item.size();i++){
    looked[news.item[i]]=1;     
    }
    while(1){    
    int c=rnd(0,ITEM-1);
    if(looked[c]==0){    
    looked[c]=1;
    news.item.push_back(c);
    break;    
    }    
    int br=1;    
    for(int i=0;i<ITEM;i++){
    if(looked[i]==0){br=0;break;}    
    }
    if(br==1){break;}    
    }    
    }
    else if(rd==1&&u==1){
    int looked[ITEM]={0};
    for(int i=0;i<(int)news.item.size();i++){
    looked[news.item[i]]=1;     
    }
    while(1){    
    int c=rnd(0,ITEM-1);
    if(looked[c]==1){    
    looked[c]=0;
    int d=0;    
    for(int i=0;i<(int)news.item.size();i++){
    if(c==news.item[i]){d=i;break;}    
    }
    news.item.erase(news.item.begin() + d);    
    break;    
    }    
    int br=1;    
    for(int i=0;i<ITEM;i++){
    if(looked[i]==1){br=0;break;}    
    }
    if(br==1){break;}    
    }
    }        
    }

    news.score=eval(news.item);
    old.score=travel.score;

    node nn=solve(news);

    news=nn;    
        
    if(news.score>7000 && news.score>MLEN){     
    discover++;
    travel=news;
    if(choice>=0){
    elite[choice]=news;
    }
    }   
    if(MLEN<travel.score){
    MLEN=travel.score;
    no1=travel;
    }
    iter++;
    if(iter%30==0){break;}
	    
printf("iter=%d,ok=%d,news.score=%lf,MLEN=%lf\n",iter,discover,nn.score,MLEN);
    }
return no1;
}

int main(){     

node ans;
ans.score=0;

ans=solve2(ans);

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
