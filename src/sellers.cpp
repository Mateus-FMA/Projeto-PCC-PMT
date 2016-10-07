#include <iostream>
#include <bits/stdc++.h>




namespace pmt{
namespace{
std::string RemoveRepeatedLetters(const std::string &str) {
  std::unordered_map<char, int> container;
  std::ostringstream oss;
  std::copy_if(str.begin(), str.end(), std::ostream_iterator<char>(oss),
               [&container] (char c) -> bool { return container[c]++ == 0; });

  return oss.str();
}
std::vector< map<char,int> > table( std::string pattern, std::string text, std::string alphabet, int error){
    int m = static_cast<int>(pattern.size());
    int n = static_cast<int>(alphabet.size());
    int o = static_cast<int>(text.size());
    std::vector< std::vector<int> > table(min(100000,m*n*o),std::vector<int>(m+1));
    std::vector<int> initial(m+1);
    for(int i=0;i<=m;i++){
        initial[i]=i;
    }
    table[0]=initial;
    std::vector< std::map<char,int> > transitions(min(100000,m*n*o),map<char,int>());
    int counter=0,counter2=1;
    while(counter<counter2){
        initial=table[counter];

        std::vector<int> novo(m+1);
        for(int i=0;i<n;i++){
            for(int j=1;j<=m;j++){
                int u=min(novo[j-1]+1,initial[j]+1);

                if((char)pattern[j-1]==(char)alphabet[i]){
                    novo[j]=initial[j-1];



                }else{
                    novo[j]=min(u,initial[j-1]+1);

                }
                if(novo[j]>error)novo[j]=error+1;


           }
           bool go=true;
           int place=-1;
           for(int k=0;k<counter&&go==true;k++){
                for(int l=0;l<=m;l++){

                    if(table[k][l]!=novo[l]){
                        l=m+10;

                    }
                    else if(l==m){
                        go=false;
                        place=k;
                    }

                }
           }
           if(go==false){
                transitions[counter][(char)alphabet[i]]=place;
                go=true;
           }else{
               transitions[counter][(char)alphabet[i]]=counter2;
               table[counter2]=novo;
               counter2++;


           }
        }
        counter++;
    }

    for(int i=0;i<counter;i++){
        for(int j=0;j<n;j++){
            int next=transitions[i][(char)alphabet [j]];
            if(table[next][m]<=error){
                transitions[i][(char)alphabet [j]]=next*-1;
            }

        }
    }
    return transitions;

}
std::vector<int> SellersStringMatcher( std::string pattern, std::string text, std::string alphabet,int error){

    int n = static_cast<int>(text.size());
    std::vector<int>occ(n);
    std::vector<map<char,int> > transitions=table(pattern,text,alphabet,error);
    int init=0;
    int counter=0;
    for(int i=0;i<n;i++){
        init=transitions[init][(char)text[i]];
        if(init<0){
            occ[counter]=i;
            init*=-1;
            counter++;

        }


    }
    std::vector<int> occ2(counter);
    for(int i=0;i<counter;i++){
        occ2[i]=occ[i];
    }
    return occ2;
}


vector<int> SellersMultiStringMatcher(const std::string &pattern,
                                             const std::vector<std::string> &text_list) {
  std::string all_chars = pattern;

 for (size_t i = 0; i < text_list.size(); ++i) {
   all_chars += text_list[i];
  }

 std::string alphabet = RemoveRepeatedLetters(all_chars);
  std::unordered_map<char, int> bad_character = ComputeBadCharacterTable(pattern, alphabet);
  std::vector<int> good_suffix = ComputeGoodSuffixTable(pattern);
  std::vector<int> occurrences;

  for (size_t i = 0; i < text_list.size(); ++i) {
    std::vector<int> tmp = SellersStringMatcher(pattern, text_list[i], bad_character,
                                                   good_suffix);
    occurrences.insert(occurrences.end(), tmp.begin(), tmp.end());
  }

  return occurrences;
}

}  // namespace pmt
} // namespace
