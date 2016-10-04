#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int brd(string pat, int j){
    if(j==0){
        return -1;
    }
    int i=j-1;
    while(i>=0){
        if(pat.substr(0,i)==pat.substr(j-i,i)){
            return i;
        }
        i--;
     }

}
int sbrd(string pat,int j){
    if(j==pat.length()){
        return brd(pat,j);
    }
    int i=j-1;
    while(i>=0){
        if(pat.substr(0,i)==pat.substr(j-i,i)&&pat[i]!=pat[j]){
            return i;
        }
        i--;
     }
     return -1;
}
int* init_brd_bf(string pat){
    int answer[pat.length()];
    for(int i=0;i<pat.length();i++){
        answer[i]=brd(pat,i);

    }
    return answer;

}
int* init_sbrd_bf(string pat){
    int answer[pat.length()];
    for(int i=0;i<pat.length();i++){
        answer[i]=sbrd(pat,i);

    }
    return answer;

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
int* init_sbrd(string pat){
    int m=pat.length();
    int nxt[m+1];

    memset(nxt,-1,sizeof(nxt));
    nxt[1]=0;
    int i=1,j=0;
    int computed_borders=2;
    while(i<m){
        while(i+j<m&&pat[j]==pat[i+j]){
            j++;
            if(i+j==m){
                nxt[i+j]=j;
            }else if(i+j<m){

                if(pat[j]!=pat[i+j]){
                    nxt[i+j]=j;
                }
            }else{
                nxt[i+j]=nxt[j];
                computed_borders++;
            }

        }if(j==0){
            if(pat[0]!=pat[i+1]){
                nxt[i+1]=0;
            }
        }
        i+=j-nxt[j];
        j=max(0,nxt[j]);
    }
    return nxt;
}

int* kmp(string txt,string pat){
    int n,m;
    n=txt.length();
    m=pat.length();
    int occ[10000];
    int occ_counter=0;
    memset(occ,-1,sizeof(occ));

    int nxt[m+1];
    memset(nxt,-1,sizeof(nxt));
    nxt[1]=0;
    int i=1,j=0;
    int computed_borders=2;
    while(i<m){
        while(i+j<m&&pat[j]==pat[i+j]){
            j++;
            if(i+j==m){
                nxt[i+j]=j;
            }else if(i+j<m){

                if(pat[j]!=pat[i+j]){
                    nxt[i+j]=j;
                }
            }else{
                nxt[i+j]=nxt[j];
                computed_borders++;
            }

        }if(j==0){
            if(pat[0]!=pat[i+1]){
                nxt[i+1]=0;
            }
        }
        i+=j-nxt[j];
        j=max(0,nxt[j]);
    }


    i=0;
    j=0;
    while(i<=(n-m)){
        int old_j=j;
        //cout<<j<<"f"<<endl;
        while(j<m && pat[j]==txt[i+j]){
            j++;
        }//cout<<j<<endl;
        if(j==m){
            occ[occ_counter]=i;
            occ_counter++;
        }
        i+=j-nxt[j];
        j=max(0,nxt[j]);
    }
    return occ;
}
int main()
{
    string txt="abracadabra";
    string pat="abra";
    int* bf=kmp(txt,pat);
    for(int i=0;bf[i]!=-1;i++){
        cout<<bf[i]<<" ";

    }
    return 0;
}

