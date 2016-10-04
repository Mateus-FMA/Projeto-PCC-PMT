#include <iostream>
#include <bits/stdc++.h>
using namespace std;

void ahocorasick(string txt,string* pat){
    int cur=0;
    int asize=1000;
    int occ[256][asize];
    for(int i=0;i<256;i++){
        for(int j=0;j<asize;j++){
            occ[i][j]=-1;
        }
    }
    int n=sizeof(txt);
    char alphabet[256];
    for (int i=0;i<256;i++){
        alphabet[i]=i;
    }
    //cout<<"build_goto"<<endl;
    //build_goto(pat,alphabet)
    int got[256][256];
     for(int i=0;i<256;i++){
        for(int j=0;j<256;j++){
            got[i][j]=-1;
        }
    }
    int nxt=0;
    for(int k=0;k<sizeof(pat);k++){
        cur=0;
        int j=0;
        int m=pat[k].length();
        while(j<m&&got[cur][pat[k][j]]!=-1){
            //cout<<"h"<<cur<<" "<<k<<" "<<j<<endl;
            cur=got[cur][pat[k][j]];
            j++;

        }
        while(j<m){
            nxt++;
            got[cur][pat[k][j]]=nxt;
            //cout<<"i"<<nxt<<" "<<cur<<endl;
            cur=nxt;
            j++;

        }
        for(int i=0;i<256;i++){
            if(occ[cur][i]==-1){
                occ[cur][i]=k;
                //cout<<cur<<" j"<<i<<" "<<k<<endl;
                i=256;
            }
        }


    }
    for(int i=0;i<256;i++){
        if(got[0][i]==-1){
            got[0][i]=0;
        }
    }
    //build_fail(pat,goto,occ,alphabet)
    //cout<<"build_fail"<<endl;
    int fail[256];
    int quill[256];
    memset(quill,0,sizeof(quill));
    int qcounter=0;
    for(int i=0;i<256;i++){
        if(got[0][i]!=0){
            fail[got[0][i]]=0;
            quill[qcounter]=got[0][i];
            qcounter++;
        }

    }
    int popper=0;
    while(popper<qcounter){
        cur=quill[popper];
        popper++;
        for(int i=0;i<256;i++){
            if(got[cur][i]!=-1){
                nxt=got[cur][i];
                quill[qcounter]=nxt;
                qcounter++;
                int brd=fail[cur];
                while(got[brd][i]==-1){
                    brd = fail[brd];
                }
                fail[nxt]=got[brd][i];
                for(int j=0;j<asize;j++){
                    if(occ[nxt][j]<0){
                        for(int k=0;k<asize;k++){
                            if(occ[fail[nxt]][k]!=-1){
                                occ[nxt][j+k]=occ[fail[nxt]][k];
                            }else{
                                k=asize;
                            }
                        }
                        j=asize;
                    }
                }

            }
        }

    }
    //cout<<"FSM finished"<<endl;
    //build_fsm finished
    int e[1000][100];

    for(int i=0;i<1000;i++){
        for(int j=0;j<100;j++){
            e[i][j]=-1;
        }
    }
    //cout<<"memset"<<endl;
    cur=0;
    for(int i=0;i<txt.length();i++){
        while(got[cur][txt[i]]==-1){
            cur=fail[cur];
        }
        cur=got[cur][txt[i]];
        //cout<<cur<<" k"<<txt[i]<<endl;
        if(occ[cur][0]!=-1){
            //cout<<"found"<<endl;
            for(int k=0;occ[cur][k]!=-1;k++){
                for(int j=0;j<100;j++){
                    if(e[occ[cur][k]][j]==-1){
                        //cout<<k<<" "<<j<<endl;
                        e[occ[cur][k]][j]=i-pat[occ[cur][k]].length()+1;
                        //cout<<i<<" "<<occ[cur][k]<<" "<<pat[occ[cur][k]].length()<<endl;
                        j=100;

                    }
                }
            }
        }
    }
    for(int i=0;i<sizeof(pat);i++){
        int j=0;
        cout<<"[";
        for(j=0;e[i][j]!=-1;j++){
            cout<<e[i][j]<<" ";
        }
        cout<<"]"<<endl;
    }


}
int main()
{
    string txt="my god she sells sea shells in hers the sea shore";
    string pat[4];
    pat[0]="he";
    pat[1]= "she";
    pat[2]= "his";
    pat[3]= "hers";
    //string alphabet = " abcdefghijklmnopqrstuvwxyz";
    //cout<<"aho started"<<endl;
    ahocorasick(txt,pat);
    //cout<<"aho finished"<<endl;



    return 0;
}
