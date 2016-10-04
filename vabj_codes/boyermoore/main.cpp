#include <iostream>
#include <bits/stdc++.h>

using namespace std;
int* bad_char(string pat){
    int c[256];
    memset(c,-1,sizeof(c));
    for(int j=0;j<pat.length();j++){
        int f=pat[j];
        c[f]=j;

    }
    return c;
}
int* init_brd(string pat){
    int m=pat.length();
    int nxt[m+1];
    memset(nxt,0,sizeof(nxt));
    nxt[0]=-1;
    int i=1,j=0;
    int computed_borders=2;
    while(i<m){
        while(i+j<m&&pat[j]==pat[i+j]){
            j++;
            nxt[i+j]=j;
            computed_borders++;
        }
        i+=j-nxt[j];
        j=max(0,nxt[j]);
    }
    return nxt;
}
int* good_suffix_bf(string pat){
    int m=pat.length();
    int s[m];
    memset(s,m,sizeof(s));
    for (int j=0;j<m;j++){
        int k=m-1;
        while(k>0 && !(((m-j)<=k && pat.substr(j,m-j).compare(pat.substr(k-(m-j),m-j))==0||((k<(m-j))&&(pat.substr(0,k).compare(pat.substr(m-k,k))==0))))){
            k--;
        }
        s[j]=m-k;

    }
    return s;
}
int* good_suffix(string pat){

    //init_brd(pat)
    int m=pat.length();
    int Pi[m+1];
    memset(Pi,0,sizeof(Pi));
    Pi[0]=-1;
    int i=1,j=0;
    int computed_borders=2;
    while(i<m){
        while(i+j<m&&pat[j]==pat[i+j]){
            j++;
            Pi[i+j]=j;
            computed_borders++;
        }
        i+=j-Pi[j];
        j=max(0,Pi[j]);
    }
    //init_brd(patR)
    string patR;
    //cout<<"before for in gs"<<endl;
    for(int i=0;i<pat.length();i++){
        patR[pat.length()-i-1]=pat[i];
    }
    int PiR[m+1];
    memset(PiR,0,sizeof(PiR));
    PiR[0]=-1;
    i=1,j=0;
    while(i<m){
        while(i+j<m&&patR[j]==patR[i+j]){
            j++;
            PiR[i+j]=j;
            computed_borders++;
        }
        i+=j-PiR[j];
        j=max(0,PiR[j]);
    }


    int s[m+1];
    memset(s,m-Pi[m],sizeof(s));
    //cout<<"memset in gs"<<m-Pi[m]<<endl;
    for(int l=1;l<m;l++){
        int j=m-PiR[l];
        if(s[j]>l-PiR[l]){
            s[j]=l-PiR[l];
        }
    }

    return s;
}
int* boyer_moore(string txt,string pat){
    int n=txt.length(),m=pat.length();
    //cout<<"m="<<m<<endl;
    int occ[10000];
    memset(occ,-1,sizeof(occ));
    int counter=0;
    //cout<<"badchar"<<endl;
    int c[256];
    memset(c,-1,sizeof(c));
    for(int j=0;j<pat.length();j++){
        int f=pat[j];
        c[f]=j;

    }
    //cout<<"badchar finished"<<endl;
    //init_brd(pat)
    int Pi[m+1];
    memset(Pi,0,sizeof(Pi));
    Pi[0]=-1;
    int i=1,j=0;
    int computed_borders=2;
    while(i<m){
        while(i+j<m&&pat[j]==pat[i+j]){
            j++;
            Pi[i+j]=j;
            computed_borders++;
        }
        i+=j-Pi[j];
        j=max(0,Pi[j]);
    }
    //cout<<"pi[m]"<<Pi[m]<<endl;
    //init_brd(patR)
    string patR;
    for(int i=0;i<pat.length();i++){
        patR[pat.length()-i-1]=pat[i];
    }
    int PiR[m+1];
    memset(PiR,0,sizeof(PiR));
    PiR[0]=-1;
    i=1,j=0;
    while(i<m){
        while(i+j<m&&patR[j]==patR[i+j]){
            j++;
            PiR[i+j]=j;
            computed_borders++;
        }
        i+=j-PiR[j];
        j=max(0,PiR[j]);
    }


    int s[m+1];
    int pie=m-Pi[m];
    //cout<<"pie="<<pie<<endl;
    for(int i=0;i<m+1;i++){
        s[i]=pie;
    }
    //cout<<s[0]<<"s[0]"<<endl;
    for(int l=1;l<m;l++){
        int j=m-PiR[l];
        if(s[j]>l-PiR[l]){
            s[j]=l-PiR[l];
        }
    }


    //cout<<"good suffix ended"<<endl;
    i=0;
    while(i<=n-m){
        int j=m-1;
        //cout<<"j="<<j<<endl;
        while (j>=0){
            if(pat[j]==txt[i+j]){
                j--;
            }else goto wbreak;

        }
        wbreak:
        if(j<0){
           occ[counter]=i;
           //cout<<"found one"<<endl;
           counter++;
           i+=s[0];
           //cout<<s[0];
        }else{
            int f=txt[i+j];
            //cout<<f;
            i+=max(s[j+1],j-c[f]);
        }
        //cout<<i<<endl;

    }
    return occ;
}
int main()
{
    string txt="abracadabraf";
    string pat="abra";
    int* answer=boyer_moore(txt,pat);
    //cout<<answer[0]<<endl;
    for(int i=0;answer[i]!=-1;i++){
        cout<<answer[i]<<" ";
    }
    return 0;
}
