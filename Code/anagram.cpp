map<char, int> storage;           // 738us 
unordered_map<char, int> storage; // 260us 
vector<int> storage(256);         // 43us


template<typename STORAGE>
bool isAnagram(const string& s1, const string& s2, STORAGE& asciiCount)
{
    for(auto& v : s1)
    {
        asciiCount[v]++;    
    }
    for(auto& v : s2)
    {
        if(--asciiCount[static_cast<unsigned char>(v)] == -1)
        {
            return false;
        }
    }

    return true;
}


bool is_anagram(std::string s1, std::string s2)
{
  std::sort(s1.begin(), s1.end());
  std::sort(s2.begin(), s2.end());
  return s1 == s2;
}

bool areAnagram(char *str1, char *str2)
{
    // Create two count arrays and initialize all values as 0
    int count1[NO_OF_CHARS] = {0};
    int count2[NO_OF_CHARS] = {0};
    int i;

    // For each character in input strings, increment count in
    // the corresponding count array
    for (i = 0; str1[i] && str2[i];  i++)
    {
        count1[str1[i]]++;
        count2[str2[i]]++;
    }

    // If both strings are of different length. Removing this condition
    // will make the program fail for strings like "aaca" and "aca"
    if (str1[i] || str2[i])
      return false;

    // Compare count arrays
    for (i = 0; i < NO_OF_CHARS; i++)
        if (count1[i] != count2[i])
            return false;

    return true;
}

