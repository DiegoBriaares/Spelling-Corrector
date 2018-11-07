#include <bits/stdc++.h>

using namespace std;

const long long INF = 999999999;

bool corrected;
vector<string> candidates;
vector<string> new_candidates;
map<string, bool> in_set;
map<string, int> word_ocurrences;
map<pair<string, string>, int> contiguous_words_ocurrences;


string generalize(string s) {
  string new_s = "";
  for (int i = 0; i < s.size(); i++) {
    if (s[i] >= 'A' && s[i] <= 'Z' || s[i] >= 'a' && s[i] <= 'z') {
    char non_capital_letter;
      if (s[i] >= 'A' && s[i] <= 'Z') {
        non_capital_letter = s[i] + ('a' - 'A');
        new_s.push_back(non_capital_letter);
      }
      else {
        non_capital_letter = s[i];
        new_s.push_back(non_capital_letter);
      }
    }
  }
  return new_s;
}

bool is_a_word(string s) {
  return (word_ocurrences.find(s) != word_ocurrences.end());
}

void add_candidates_deleting() {
  vector<string> my_candidates;
  for (int i = 0; i < candidates.size(); i++) {
    string word = candidates[i], new_word;
    for (int j = 0; j < word.size(); j++) {
      new_word = word;
      new_word.erase(new_word.begin() + j);
      if (is_a_word(new_word)) corrected = true;
      if (!in_set[new_word]) {
          my_candidates.push_back(new_word);
          in_set[new_word] = true;
        }
    }
  }
  for (int i = 0; i < my_candidates.size(); i++) {
    new_candidates.push_back(my_candidates[i]);
  }
}

void add_candidates_transposing() {
  vector<string> my_candidates;
  for (int i = 0; i < candidates.size(); i++) {
    string word = candidates[i], new_word;
    for (int j = 1; j < word.size(); j++) {
      new_word = word;
      string swapped;
      swapped.push_back(word[j]);
      swapped.push_back(word[j - 1]);
      new_word.replace(j - 1, 2, swapped);
      if (is_a_word(new_word)) corrected = true;
      if (!in_set[new_word]) {
          my_candidates.push_back(new_word);
          in_set[new_word] = true;
        }
    }
  }
  for (int i = 0; i < my_candidates.size(); i++) {
    new_candidates.push_back(my_candidates[i]);
  }
}

void add_candidates_inserting() {
  vector<string> my_candidates;
  for (int i = 0; i < candidates.size(); i++) {
    string word = candidates[i], new_word;
    for (int j = 0; j < word.size(); j++) {
      for (int letter = 'a'; letter <= 'z'; letter++) {
        new_word = word;
        string inserted;
        inserted.push_back(letter);
        new_word.insert(j, inserted);
        if (is_a_word(new_word)) corrected = true;
        if (!in_set[new_word]) {
          my_candidates.push_back(new_word);
          in_set[new_word] = true;
        }
      }
    }
  }
  for (int i = 0; i < my_candidates.size(); i++) {
    new_candidates.push_back(my_candidates[i]);
  }
}

void add_candidates_replacing() {
  vector<string> my_candidates;
  for (int i = 0; i < candidates.size(); i++) {
    string word = candidates[i], new_word;
    for (int j = 0; j < word.size(); j++) {
      new_word = word;
      for (int letter = 'a'; letter <= 'z'; letter++) {
        string inserted;
        inserted.push_back(letter);
        new_word.replace(j, 1, inserted);
        if (is_a_word(new_word)) corrected = true;
        if (!in_set[new_word]) {
          my_candidates.push_back(new_word);
          in_set[new_word] = true;
        }
      }
    }
  }
  for (int i = 0; i < my_candidates.size(); i++) {
    new_candidates.push_back(my_candidates[i]);
  }
}

string most_probable_word(string last_word) {
  string word;
  int ans = 0;
  for (int i = 1; i < candidates.size(); i++) {
    if (!is_a_word(candidates[i])) continue;
    int ocurrences = 1;
    if (contiguous_words_ocurrences.find(make_pair(last_word, candidates[i])) != contiguous_words_ocurrences.end()) { 
      ocurrences = contiguous_words_ocurrences[make_pair(last_word, candidates[i])];
    }
    ocurrences *= ocurrences;
    ocurrences += word_ocurrences[candidates[i]];
    if (ocurrences > ans) {
      ans = ocurrences;
      word = candidates[i];
    }
  }
  return word;
}

vector<string> dictionary;
vector<pair<int, string> > cool_candidates;

int current_best;
int memo[41][41];
string typed, target;

int dp(int i, int j) {
  if (j == typed.size()) {
    return 0;
  }
  if(i == target.size()) return (-INF);
  if (memo[i][j] != -1) return memo[i][j];
  memo[i][j] = dp(i, j + 1);
  for (int it = i; it < target.size(); it++) {
    if (target[it] == typed[j]) {
      return memo[i][j] = max(memo[i][j], dp(it + 1, j + 1) + 1);
    }
  }
  return memo[i][j];
}

int edit_distance() {
  memset(memo, -1, sizeof(memo));
  int distance = abs(dp(0, 0) - (int)typed.size());
  current_best = min(current_best, distance);
  return distance;
}

int calculate_proba (string last_word) {
  return word_ocurrences[target] + (contiguous_words_ocurrences[make_pair(last_word, target)] * contiguous_words_ocurrences[make_pair(last_word, target)]);
}

string other_word_correction(string s, string last_word) { 
  string word;
  int ans = 0;
  typed = s;
  current_best = 4;
  candidates.clear();
  cool_candidates.clear();
  for (int i = 0; i < dictionary.size(); i++) {
    target = dictionary[i];
    int score = edit_distance();
    if (abs((int)target.size() - (int)typed.size()) <= current_best && score <= current_best) cool_candidates.push_back(make_pair(score, target));
  }
  sort(cool_candidates.begin(), cool_candidates.end());
  for (int i = 0; i < cool_candidates.size(); i++) {
    if (i && cool_candidates[i].first > cool_candidates[i - 1].first) break;
    target = cool_candidates[i].second;
    int score = calculate_proba(last_word);
    if (score > ans) {
      ans = score;
      word = target;
    }
  }
  return (word.empty() ? s : word);
}

string correct_word(string s, string last_word) {
  string word;
  corrected = false;
  in_set.clear();
  candidates.clear();
  new_candidates.clear();
  candidates.push_back(s);
  for (int edit_distance = 1; edit_distance <= 2; edit_distance++) {
    add_candidates_deleting();
    add_candidates_transposing();
    add_candidates_inserting();
    add_candidates_replacing();
    for (int i = 0; i < new_candidates.size(); i++) candidates.push_back(new_candidates[i]);
    new_candidates.clear();
    if (corrected) {
      word = most_probable_word(last_word);
      return word;
    }
  }
  return other_word_correction(s, last_word);
  return s;
}

int main () {
  string word, last_word, new_word;
  ifstream big;
  big.open("big.txt");
  while (big >> word) {
    word = generalize(word);
    if (!word.empty()) {
      if (!word_ocurrences[word]) dictionary.push_back(word);
      word_ocurrences[word]++;
      if(word_ocurrences[word] == 1)contiguous_words_ocurrences[make_pair("", word)]++;
      if (!last_word.empty()) {
        contiguous_words_ocurrences[make_pair(last_word, word)]++;
      }
      last_word = word;
    }
  }
  big.close();
  big.open("words.txt");
  while (big >> word) {
    int ocurrences;
    char coma;
    big >> ocurrences;
    big >> coma;
    word = generalize(word);
    if (!word_ocurrences[word])dictionary.push_back(word);
    word_ocurrences[word] += ocurrences;
  }
  big.close();
  ifstream text;
  big.open("target/chuchum.txt");
  freopen("target/choxom.txt", "w", stdout);
  last_word = "";
  while (big >> word) {
    new_word = generalize(word);
    if (!is_a_word(new_word)) {
      word = correct_word(new_word, last_word);
      cout << word << " ";
    }
    else {
      cout << word << " ";
    }
    word = generalize(word);
    if (!word.empty()) last_word = word;
  }
}

