#include <bits/stdc++.h>

using namespace std;

const long long INF = 999999999;

bool corrected;
vector<string> candidates;
vector<string> dictionary;
vector<string> answers;
map<string, int> word_ocurrences;
vector<pair<int, string> > cool_candidates;
map<pair<string, string>, int> contiguous_words_ocurrences;

int current_best;
int memo[41][41];
string typed, target;

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

void add_candidates_transposing(string word) {
  for (int j = 1; j < word.size(); j++) {
    string new_word = word;
    string swapped;
    swapped.push_back(word[j]);
    swapped.push_back(word[j - 1]);
    new_word.replace(j - 1, 2, swapped);
    if (is_a_word(new_word)) {
      corrected = true;
      answers.push_back(new_word);
    }
    candidates.push_back(new_word);
  }
}

void add_candidates_deleting(string word) {
  for (int j = 0; j < word.size(); j++) {
      string new_word = word;
      new_word.erase(new_word.begin() + j);
      if (is_a_word(new_word)) {
      corrected = true;
      answers.push_back(new_word);
    }
    }
}

void add_candidates_inserting(string word) {
  for (int j = 0; j < word.size(); j++) {
      for (int letter = 'a'; letter <= 'z'; letter++) {
        string new_word = word;
        string inserted;
        inserted.push_back(letter);
        new_word.insert(j, inserted);
        if (is_a_word(new_word)) {
      corrected = true;
      answers.push_back(new_word);
    }
      }
    }
}

void add_candidates_replacing(string word) {
  for (int j = 0; j < word.size(); j++) {
      string new_word = word;
      for (int letter = 'a'; letter <= 'z'; letter++) {
        string inserted;
        inserted.push_back(letter);
        new_word.replace(j, 1, inserted);
        if (is_a_word(new_word)) {
      corrected = true;
      answers.push_back(new_word);
    }
      }
    }
}

string correct_word(string s, string last_word) { 
  string word;
  corrected = false;
  int ans = 0;
  typed = s;
  current_best = 4;
  answers.clear();
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
  add_candidates_transposing(s);
 // add_candidates_deleting(s);
  //add_candidates_inserting(s);
  //add_candidates_replacing(s);
  if (corrected) {
    for (int i = 0; i < answers.size(); i++) {
      target = answers[i];
    int score = calculate_proba(last_word);
    if (score > ans) {
      ans = score;
      word = target;
    }
    }
    return word;
  }
  if (cool_candidates[0].first == 1) {
    return word;
  }
  for (int i = 0; i < candidates.size(); i++) {
    typed = candidates[i];
    cool_candidates.clear();
    for (int i = 0; i < dictionary.size(); i++) {
      target = dictionary[i];
      int score = edit_distance() + 1;
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
  }
  return (word.empty() ? s : word);
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
  big.open("golden/randy.txt");
  freopen("target/randal2.txt", "w", stdout);
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

