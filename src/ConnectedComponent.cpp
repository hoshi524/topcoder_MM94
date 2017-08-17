#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 2000;
constexpr int MAX_S = 500;

double START_TIME;
int S;
int M[MAX_S][MAX_S];
int C[MAX_S][MAX_S];

double get_time() {
  unsigned long long a, d;
  __asm__ volatile("rdtsc" : "=a"(a), "=d"(d));
  return (d << 32 | a) / 2500000.0;
}

unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

class ConnectedComponent {
 public:
  vector<int> permute(vector<int> matrix) {
    START_TIME = get_time();
    S = (int)sqrt(matrix.size());
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        M[i][j] = matrix[i * S + j];
      }
    }
    vector<int> ret(S);
    vector<int> per(S);
    double score = 0;
    int remain[MAX_S];
    for (int i = 0; i < S; ++i) {
      remain[i] = i;
    }
    while ((START_TIME + TIME_LIMIT - get_time()) / TIME_LIMIT > 0) {
      for (int i = 0; i < S; ++i) {
        int t = get_random() % (S - i);
        per[i] = remain[t];
        int v = remain[t];
        remain[t] = remain[S - i - 1];
        remain[S - i - 1] = v;
      }
      for (int i = 0; i < S; ++i)
        for (int j = 0; j < S; ++j) C[i][j] = M[per[i]][per[j]];
      double s = calc();
      if (score < s) {
        score = s;
        ret = per;
      }
    }
    return ret;
  }

  double calc() {
    double score = 0;
    bool ok[MAX_S][MAX_S];
    memset(ok, false, sizeof(ok));
    int qx[MAX_S * MAX_S];
    int qy[MAX_S * MAX_S];
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        if (C[i][j] == 0 || ok[i][j]) continue;
        ok[i][j] = true;
        int qi = 0, qs = 1, sum = 0;
        qx[qi] = i;
        qy[qi] = j;
        while (qi < qs) {
          int x = qx[qi];
          int y = qy[qi];
          ++qi;
          sum += C[x][y];
          if (x + 1 < S && C[x + 1][y] != 0 && !ok[x + 1][y]) {
            ok[x + 1][y] = true;
            qx[qs] = x + 1;
            qy[qs] = y;
            ++qs;
          }
          if (x - 1 >= 0 && C[x - 1][y] != 0 && !ok[x - 1][y]) {
            ok[x - 1][y] = true;
            qx[qs] = x - 1;
            qy[qs] = y;
            ++qs;
          }
          if (y + 1 < S && C[x][y + 1] != 0 && !ok[x][y + 1]) {
            ok[x][y + 1] = true;
            qx[qs] = x;
            qy[qs] = y + 1;
            ++qs;
          }
          if (y - 1 >= 0 && C[x][y - 1] != 0 && !ok[x][y - 1]) {
            ok[x][y - 1] = true;
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
