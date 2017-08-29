#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 9500;
constexpr int MAX_S = 500;
constexpr int MAX_SS = MAX_S * MAX_S;

double get_time() {
  unsigned long long a, d;
  __asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
  return (d << 32 | a) / 2500000.0;
}

unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

double get_random_double() { return (double)get_random() / UINT_MAX; }

int S;
char M[MAX_S][MAX_S];

int to(int x, int y) { return x * S + y; }

struct State {
  int permute[MAX_S], data[MAX_SS], sum[MAX_SS];
  double score;

  State() {
    score = -1;
    for (int i = 0; i < MAX_S; ++i) {
      permute[i] = i;
    }
    for (int i = 0; i < MAX_SS; ++i) {
      data[i] = -1;
    }
  }

  void copy(const State& s, int x) {
    memcpy(permute, s.permute, sizeof(permute));
    for (int i = 0; i <= x; ++i) {
      for (int j = 0; j <= x; ++j) {
        int p = to(i, j);
        data[p] = s.data[p];
        sum[p] = s.sum[p];
      }
    }
  }

  void unite(int x, int y) {
    x = root(x);
    y = root(y);
    if (x != y) {
      if (data[y] < data[x]) swap(x, y);
      data[x] += data[y];
      data[y] = x;
      sum[x] += sum[y];
    }
  }
  int root(int x) { return data[x] < 0 ? x : data[x] = root(data[x]); }
  double getScore(int x) {
    x = root(x);
    return sum[x] * sqrt(-data[x]);
  }

  void transition(int l, int r) {
    int t = permute[l];
    permute[l] = permute[r];
    permute[r] = t;

    for (int i = 0; i <= l; ++i) {
      sum[to(i, l)] = M[permute[i]][permute[l]];
      sum[to(l, i)] = M[permute[l]][permute[i]];
    }
    score = 0;
    for (int i = 0; i < l; ++i) {
      auto unite_ = [&](int x, int y, int a, int b) {
        if (M[permute[x]][permute[y]] && M[permute[a]][permute[b]]) {
          unite(to(x, y), to(a, b));
        }
      };
      unite_(i, l, i, l - 1);
      unite_(i, l, i + 1, l);
      unite_(l, i, l - 1, i);
      unite_(l, i, l, i + 1);
      score = max({score, getScore(to(i, l)), getScore(to(l, i))});
    }
  }
};

constexpr int MAX_WIDTH = 500;
State states[2][MAX_WIDTH];

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

    states[0][0].score = 0;
    int tn = 1;
    for (int i = 0; i < S; ++i) {
      State* current = states[i & 1];
      State* next = states[(i + 1) & 1];
      double now = get_time();
      double passage = now - START_TIME;
      double remain = START_TIME + TIME_LIMIT - now;
      double onewidth = (1 + S - i) * (S - i);
      double onetrans = passage / tn;
      int width = remain / onetrans / onewidth;
      if (width < 3) width = 3;
      if (width > MAX_WIDTH) width = MAX_WIDTH;
      for (int j = 0; j < width; ++j) {
        next[j].score = -1;
      }
      for (int j = 0; j < width - 1; ++j) {
        State* c = &current[j];
        if (c->score < 0) continue;
        tn += S - i;
        for (int k = i; k < S; ++k) {
          State* n = &next[0];
          for (int m = 1; m < width; ++m) {
            if (n->score > next[m].score) {
              n = &next[m];
            }
          }
          n->copy(*c, i);
          n->transition(i, k);
        }
      }
    }
    vector<int> permute(S);
    for (int i = 0; i < S; ++i) {
      permute[i] = states[S & 1][0].permute[i];
    }
    return permute;
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
