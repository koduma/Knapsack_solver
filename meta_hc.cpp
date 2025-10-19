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
    ull item[((ITEM-1)/64)+1];
    double score;
    double sumw;
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

vector<int> get_item(ull item[((ITEM-1)/64)+1]) {
    vector<int> result;
    for (int i = 0; i < ITEM; i++) {
        if (((item[i/64] >> (i % 64)) & 1ULL) == 1ULL) {
            result.push_back(i);
        }
    }
    return result;
}

// Evaluate node from its bitset; also fill node.sumw
double eval_node(node &n) {
    double sumv = 0.0;
    double sumw = 0.0;
    for (int i = 0; i < ITEM; ++i) {
        if ( ((n.item[i/64] >> (i%64)) & 1ULL) ) {
            sumv += (double)val[i];
            sumw += (double)wei[i];
        }
    }
    n.sumw = sumw;
    if (sumw > (double)cap) return -1.0;
    return sumv;
}

node solve(node travel) {
    
    double MLEN=-1.0;
    node no1;
    int discover=0;
    int counter=0;
    vector<node>elite;
    int iter=0;

    // Ensure travel has correct score and sumw
    travel.score = eval_node(travel);

    vector<int>items;
    
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

        items=get_item(travel.item);    
        
        node old=travel;
        node news=travel;
     
        for(int u=0;u<2;u++){

            int rd=rnd(0,1);
            if(rd==1&&u==0){
                int looked[ITEM]={0};
                for(int i=0;i<(int)items.size();i++){
                    looked[items[i]]=1;     
                }
                while(1){    
                    int c=rnd(0,ITEM-1);
                    if(looked[c]==0){    
                        looked[c]=1;
                        items.push_back(c);
                        int idx = c/64;
                        int off = c%64;
                        news.item[idx] |= ( (ull)1 << off );
                        double prev_score = news.score;
                        news.sumw = news.sumw + wei[c];
                        if(news.sumw > (double)cap){
                            news.score = -1.0;    
                        }
                        else{    
                            if(prev_score < 0.0){
                                news.score = eval_node(news);
                            } else {
                                news.score = prev_score + val[c];
                            }
                        }    
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
                for(int i=0;i<(int)items.size();i++){
                    looked[items[i]]=1;     
                }
                while(1){    
                    int c=rnd(0,ITEM-1);
                    if(looked[c]==1){    
                        looked[c]=0;
                        int d=0;    
                        for(int i=0;i<(int)items.size();i++){
                            if(c==items[i]){d=i;break;}    
                        }
                        int idx = c/64;
                        int off = c%64;
                        news.item[idx] &= ~( (ull)1 << off );   
                        double prev_score = news.score;
                        news.sumw = news.sumw - wei[c];
                        if(news.sumw > (double)cap){
                            news.score = -1.0;    
                        }
                        else{    
                            if(prev_score < 0.0){
                                news.score = eval_node(news);
                            } else {
                                news.score = prev_score - val[c];
                            }
                        }    
                        if(d >= 0 && d < (int)items.size()){
                            items.erase(items.begin() + d);
                        }
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

    // Ensure travel has correct score and sumw
    travel.score = eval_node(travel);

    vector<int>items;
    
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

        items=get_item(travel.item);    
        
        node old=travel;
        node news=travel;
     
        for(int u=0;u<2;u++){

            int rd=rnd(0,1);
            if(rd==1&&u==0){
                int looked[ITEM]={0};
                for(int i=0;i<(int)items.size();i++){
                    looked[items[i]]=1;     
                }
                while(1){    
                    int c=rnd(0,ITEM-1);
                    if(looked[c]==0){    
                        looked[c]=1;
                        items.push_back(c);
                        int idx = c/64;
                        int off = c%64;
                        news.item[idx] |= ( (ull)1 << off );
                        double prev_score = news.score;
                        news.sumw = news.sumw + wei[c];
                        if(news.sumw > (double)cap){
                            news.score = -1.0;    
                        }
                        else{    
                            if(prev_score < 0.0){
                                news.score = eval_node(news);
                            } else {
                                news.score = prev_score + val[c];
                            }
                        }    
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
                for(int i=0;i<(int)items.size();i++){
                    looked[items[i]]=1;     
                }
                while(1){    
                    int c=rnd(0,ITEM-1);
                    if(looked[c]==1){    
                        looked[c]=0;
                        int d=0;    
                        for(int i=0;i<(int)items.size();i++){
                            if(c==items[i]){d=i;break;}    
                        }
                        int idx = c/64;
                        int off = c%64;
                        news.item[idx] &= ~( (ull)1 << off );   
                        double prev_score = news.score;
                        news.sumw = news.sumw - wei[c];
                        if(news.sumw > (double)cap){
                            news.score = -1.0;    
                        }
                        else{    
                            if(prev_score < 0.0){
                                news.score = eval_node(news);
                            } else {
                                news.score = prev_score - val[c];
                            }
                        }    
                        if(d >= 0 && d < (int)items.size()){
                            items.erase(items.begin() + d);
                        }
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

double n_eval(vector<int>item){
    double sumv=0;
    double sumw=0;
    for(int i=0;i<(int)item.size();i++){  
        sumv+=(double)val[item[i]];
        sumw+=(double)wei[item[i]];
    }     

    if((double)cap<sumw){return -1.0;}
    return sumv;
}
double n_get_w(vector<int>item){
    double sumv=0;
    double sumw=0;
    for(int i=0;i<(int)item.size();i++){  
        sumv+=(double)val[item[i]];
        sumw+=(double)wei[item[i]];
    }     

    if((double)cap<sumw){return -1.0;}
    return sumw;

}

int main(){     

    node ans;
    ans.score=0;
    ans.sumw=0;
    for(int i=0;i<((ITEM-1)/64)+1;i++){
        ans.item[i]=0ll;    
    }    

    ans=solve2(ans);

    double check=0;
    vector<int>items;

    items=get_item(ans.item);

    check=n_eval(items);

    cout<<"value:"<<check<<endl;

    check=n_get_w(items);

    cout<<"weight:"<<check<<endl;

    for(int i=0;i<(int)items.size();i++){
        cout<<items[i]<<endl;   
    }

    return 0;
}
