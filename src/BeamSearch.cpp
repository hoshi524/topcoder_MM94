#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 2000;
constexpr int MAX_S = 1 << 9;

double get_time() {
  unsigned long long a, d;
  __asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
  return (d << 32 | a) / 2500000.0;
}

unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

int to(int x, int y) { return (x << 9) | y; }

int S;
char M[MAX_S][MAX_S];

struct State {
  int permute[MAX_S];
  double score;

  void copy(State& s) { memcpy(permute, s.permute, sizeof(permute)); }

  void transition(int index, int value) {
    permute[index] = value;
    static char m[MAX_S * MAX_S];
    memset(m, 0, sizeof(m));
    for (int i = 0; i <= index; ++i) {
      for (int j = 0; j <= index; ++j) {
        m[to(i + 1, j + 1)] = M[permute[i]][permute[j]];
      }
    }
    score = 0;
    static int queue[MAX_S * MAX_S];
    auto search = [&](int p) {
      if (m[p] == 0) return;
      int qi = 0, qs = 1, sum = m[p];
      m[p] = 0;
      queue[0] = p;
      auto push = [&](int p) {
        if (m[p]) {
          sum += m[p];
          m[p] = 0;
          queue[qs++] = p;
        }
      };
      while (qi < qs) {
        p = queue[qi++];
        push(p + 1);
        push(p - 1);
        push(p + MAX_S);
        push(p - MAX_S);
      }
      double t = sum * sqrt(qs);
      if (score < t) score = t;
    };
    for (int i = 0; i <= index; ++i) {
      for (int j = 0; j <= index; ++j) {
        search(to(i, j));
      }
    }
  }
};

State& get() {
  static int index = 0;
  static State buf[0x80000];
  return buf[index++];
}

class ConnectedComponent {
 public:
  vector<int> permute(vector<int> matrix) {
    double START_TIME = get_time();
    S = (int)sqrt(matrix.size());
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        M[i][j] = matrix[i * S + j];
      }
    }

    State& best = get();

    vector<State*> queue[MAX_S];
    queue[0].emplace_back(&best);

    while (START_TIME + TIME_LIMIT > get_time()) {
      for (int i = 0; i < S; ++i) {
        if (queue[i].empty()) continue;
        int t = 0;
        double score = 0;
        for (int j = 0; j < queue[i].size(); ++j) {
          if (score < queue[i][j]->score) {
            score = queue[i][j]->score;
            t = j;
          }
        }
        State s = *queue[i][t];
        queue[i].erase(queue[i].begin() + t);

        static bool used[MAX_S];
        memset(used, false, sizeof(used));
        for (int j = 0; j < i; ++j) {
          used[s.permute[j]] = true;
        }
        for (int j = 0; j < S; ++j) {
          if (used[j]) continue;
          State& c = get();
          c.copy(s);
          c.transition(i, j);
          if (i + 1 < S) {
            queue[i + 1].emplace_back(&c);
          } else if (best.score < c.score) {
            best = c;
          }
        }
      }
    }

    vector<int> ret(S);
    for (int i = 0; i < S; ++i) {
      ret[i] = best.permute[i];
    }
    return ret;
  }
};

// -------8<------- end of solution submitted to the website -------8<-------
int main() {
  ConnectedComponent cc;
  int M;
  cin >> M;
  vector<int> matrix(M);
  for (int i = 0; i < matrix.size(); ++i) cin >> matrix[i];

  vector<int> ret = cc.permute(matrix);
  cout << ret.size() << endl;
  for (int i = 0; i < (int)ret.size(); ++i) cout << ret[i] << endl;
  cout.flush();
}
