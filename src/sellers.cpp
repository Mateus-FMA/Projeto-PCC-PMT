#include <iostream>
#include <bits/stdc++.h>


using namespace std;


vector< map<char,int> > table(const string &pattern,const string &text,const string &alphabet,const int &error){
    int m = static_cast<int>(pattern.size());
    int n = static_cast<int>(alphabet.size());
    vector< vector<int> > table(m*n,vector<int>(m+1));
    vector<int> initial(m+1);
    for(int i=0;i<=m;i++){
        initial[i]=i;
    }
    table[0]=initial;
   // cout<<"table"<<table[0][2];
    vector< map<char,int> > transitions(m*n,map<char,int>());
    cout<<"created vector"<<endl;
    int counter=0,counter2=1;
    while(counter<counter2){
        cout<<counter<<" "<<counter2<<endl;
        initial=table[counter];

        cout<<"flag 0"<<endl;
        for(int i=0;i<n;i++){
            vector<int> novo(m);
            novo[0]=0;
            cout<<"flag 1,5"<<endl;
            for(int j=1;j<=m;j++){
                int u=min(novo[j-1]+1,initial[j]+1);
                if((char)pattern[j-1]==(char)alphabet[i])
                    novo[j]=min(u,initial[j-1]);
                else
                    novo[j]=min(u,initial[j-1]+1);

           }
           cout<<"flag 1"<<endl;
           bool go=true;
           int place=-1;
           for(int k=0;k<counter+1&go==true;k++){
                for(int l=0;l<m;l++){
                    if(table[k][l]!=novo[l]){
                        l=m;

                    }
                    else if(l==m-1){
                        go=false;
                        place=k;
                    }
                }
           }
           //cout<<"flag 2"<<endl;

            //cout<<"flag 3"<<endl;
           if(go==false){
                transitions[counter][(char)alphabet[i]]=place;
           }else{
               cout<<"problem here"<<counter<<(char)alphabet[i]<<counter2<<endl;
                transitions[counter][(char)alphabet[i]]=counter2;

                table[counter2]=novo;

                counter2++;


           }
           cout<<"flag 4"<<endl;
        }
        counter++;
    }
    cout<<"finished worst part"<<endl;
    for(int i=0;i<counter;i++){
        for(int j=0;j<n;j++){
            if(table[transitions[i][(char)alphabet [n]]][m]<=error){
                transitions[i][(char)alphabet [n]]*=-1;

            }
            cout<<transitions[i][(char)alphabet [n]]<<endl;
        }
    }
    return transitions;

}
vector<int> SellersStringMatcher(const string &pattern,const string &text,const string &alphabet, const int &error){
    vector<int>occ(1);
    int m = static_cast<int>(pattern.size());
    int n = static_cast<int>(text.size());
    int walls[m+1][2];
    for(int j=0;j<=m;j++){
        walls[j][0]=j;

    }
    //cout<<"init transitions"<<endl;
    vector<map<char,int> > transitions=table(pattern,text,alphabet,error);
   // cout<<"end transitions"<<endl;
    int init=0;
    for(int i=0;i<n;i++){
        init=transitions[init][(char)text[i]];
        if(init<0){
            occ.push_back(i);
            init*=-1;
        }

    }
    return occ;
}


int main(){
    string a="abcacbabc";
    string pat="abcb";
    string alpha="abc";
    int error=1;
    vector<int> occ=SellersStringMatcher(pat,a,alpha,error);
    for(int i=0;i<occ.size();i++){
        cout<<occ[i]<<endl;

    }
    return 0;
}


//std::vector<int> BoyerMooreMultiStringMatcher(const std::string &pattern,
                                             // const std::vector<std::string> &text_list) {
 // std::string all_chars = pattern;

 // for (size_t i = 0; i < text_list.size(); ++i) {
   // all_chars += text_list[i];
  //}

 // std::string alphabet = RemoveRepeatedLetters(all_chars);
  //std::unordered_map<char, int> bad_character = ComputeBadCharacterTable(pattern, alphabet);
 // std::vector<int> good_suffix = ComputeGoodSuffixTable(pattern);
  //std::vector<int> occurrences;

//  for (size_t i = 0; i < text_list.size(); ++i) {
  //  std::vector<int> tmp = BoyerMooreStringMatcher(pattern, text_list[i], bad_character,
    //                                               good_suffix);
    //occurrences.insert(occurrences.end(), tmp.begin(), tmp.end());
  //}

 // return occurrences;
//}

//}  // namespace pmt

