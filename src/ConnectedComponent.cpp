#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 2000;
constexpr int MAX_S = 500;

int S;
int M[MAX_S][MAX_S];

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
    while (true) {
      double time = (START_TIME + TIME_LIMIT - get_time()) / TIME_LIMIT;
      if (time < 0) break;
      int a = get_random() % S;
      int b = get_random() % S;
      if (a == b) continue;
      swap(ret, a, b);
      double s = calc(ret);
      if (score < s) {
        score = s;
      } else {
        swap(ret, a, b);
      }
    }
    return ret;
  }

  void swap(vector<int> &v, int a, int b) {
    int t = v[a];
    v[a] = v[b];
    v[b] = t;
  }

  double calc(vector<int> &v) {
    int C[MAX_S][MAX_S];
    for (int i = 0; i < S; ++i)
      for (int j = 0; j < S; ++j) C[i][j] = M[v[i]][v[j]];

    double score = 0;
    int qx[MAX_S * MAX_S];
    int qy[MAX_S * MAX_S];
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        if (C[i][j] == 0) continue;
        int qi = 0, qs = 1, sum = C[i][j];
        qx[qi] = i;
        qy[qi] = j;
        while (qi < qs) {
          int x = qx[qi];
          int y = qy[qi];
          ++qi;
          if (x + 1 < S && C[x + 1][y]) {
            sum += C[x + 1][y];
            C[x + 1][y] = 0;
            qx[qs] = x + 1;
            qy[qs] = y;
            ++qs;
          }
          if (x - 1 >= 0 && C[x - 1][y]) {
            sum += C[x - 1][y];
            C[x - 1][y] = 0;
            qx[qs] = x - 1;
            qy[qs] = y;
            ++qs;
          }
          if (y + 1 < S && C[x][y + 1]) {
            sum += C[x][y + 1];
            C[x][y + 1] = 0;
            qx[qs] = x;
            qy[qs] = y + 1;
            ++qs;
          }
          if (y - 1 >= 0 && C[x][y - 1]) {
            sum += C[x][y - 1];
            C[x][y - 1] = 0;
            qx[qs] = x;
            qy[qs] = y - 1;
            ++qs;
          }
        }
        double t = sum * sqrt(qs);
        if (score < t) score = t;
      }
    }
    return score;
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
