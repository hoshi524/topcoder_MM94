#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 3000;
constexpr int MAX_S = 500;

int S;
int M[MAX_S][MAX_S];
int C[MAX_S + 2][MAX_S + 2];

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

class ConnectedComponent {
 public:
  vector<int> permute(vector<int> matrix) {
    double START_TIME = get_time();
    S = (int)sqrt(matrix.size());
    memset(C, 0, sizeof(C));
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        M[i][j] = matrix[i * S + j];
      }
    }
    vector<int> ret(S);
    double score = 0;
    for (int i = 0; i < S; ++i) {
      ret[i] = i;
    }
    int qx[MAX_S * MAX_S];
    int qy[MAX_S * MAX_S];
    for (int iter = 0, prev = 0; true; ++iter) {
      double time = (START_TIME + TIME_LIMIT - get_time()) / TIME_LIMIT;
      if (time < 0) break;
      int a = get_random() % S;
      int b = get_random() % S;
      if (a == b) continue;
      swap(ret, a, b);

      for (int i = 0; i < S; ++i)
        for (int j = 0; j < S; ++j) C[i + 1][j + 1] = M[ret[i]][ret[j]];
      double s = 0;
      auto search = [&](int x, int y) {
        if (C[x][y] == 0) return;
        int qi = 0, qs = 1, sum = C[x][y];
        C[x][y] = 0;
        qx[qi] = x;
        qy[qi] = y;
        auto push = [&](int x, int y) {
          if (C[x][y]) {
            sum += C[x][y];
            C[x][y] = 0;
            qx[qs] = x;
            qy[qs] = y;
            ++qs;
          }
        };
        while (qi < qs) {
          x = qx[qi];
          y = qy[qi];
          ++qi;
          push(x + 1, y);
          push(x - 1, y);
          push(x, y + 1);
          push(x, y - 1);
        }
        double t = sum * sqrt(qs);
        if (s < t) s = t;
      };
      for (int i = 1; i <= S; ++i) {
        search(i, a + 1);
        search(i, b + 1);
        search(a + 1, i);
        search(b + 1, i);
      }

      if (score <
          s * (1 + get_random_double() * time * (iter - prev) / S / 5)) {
        score = s;
        prev = iter;
      } else {
        swap(ret, a, b);
      }
      // cerr << score << endl;
    }
    return ret;
  }

  void swap(vector<int> &v, int a, int b) {
    int t = v[a];
    v[a] = v[b];
    v[b] = t;
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
