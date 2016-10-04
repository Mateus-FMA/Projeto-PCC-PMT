#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int* bruteforce(string pat,string txt){
    int occ[10000];
    int counter=0;
    memset(occ,-1,sizeof(occ));
    int m=pat.length(),n=txt.length();
    for(int i=0;i<n-m;i++){
        int j=0;
        while(j<m&&pat[j]==txt[i+j]){
            j++;
        }
        if(j==m){
            occ[counter]=i;
            counter++;
        }
    }
    return occ;
}
int main()
{
    string txt="babcbabcabcaabcabcabcacabc";
    string pat="abcabcacab";
    int* bf=bruteforce(pat,txt);
    for(int i=0;bf[i]!=-1;i++){
        cout<<bf[i]<<endl;

    }
    return 0;
}
