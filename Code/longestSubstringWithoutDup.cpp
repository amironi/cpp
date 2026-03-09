
string longestSubstringWithoutDup(const string s) {
    string res;
    string max_res;
    unsigned int max = 0;
    
    unordered_map<char,unsigned int> letters;
    
    for (unsigned int i = 0 ; i < s.size(); i++) {

        char c = s[i];
        
        if (letters.find(c) == letters.end() ) {
            res +=c;
        
            if (res.size() > max ) {
                max_res = res;
                max = res.size();
            }

        } else {
            res = s.substr(letters[c]+1,i-1);
        }
        
        letters[c] = i;
    }

    return max_res;
}