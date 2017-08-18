#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 3000;
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

double get_random_double() { return (double)get_random() / UINT_MAX; }

void swap(vector<int> &v, int a, int b) {
  int t = v[a];
  v[a] = v[b];
  v[b] = t;
}

int to(int x, int y) { return (x << 9) | y; }

class ConnectedComponent {
 public:
  vector<int> permute(vector<int> matrix) {
    double START_TIME = get_time();
    int S = (int)sqrt(matrix.size());
    int M[MAX_S][MAX_S];
    int C[MAX_S * MAX_S];
    int T[MAX_S * MAX_S];
    int queue[MAX_S * MAX_S];
    memset(C, 0, sizeof(C));
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        M[i][j] = C[to(i + 1, j + 1)] = matrix[i * S + j];
      }
    }

    vector<int> ret(S);
    vector<int> ans(S);
    double score = 0;
    double best = 0;
    for (int i = 0; i < S; ++i) {
      ret[i] = i;
    }

    int iter = 0, prev = 0;
    int copy_byte = (S + 1) * MAX_S * 4;
    while (true) {
      double time = (START_TIME + TIME_LIMIT - get_time()) / TIME_LIMIT;
      if (time < 0) break;
      int a = get_random() % S;
      int b = get_random() % S;
      if (a == b) continue;

      memcpy(T, C, copy_byte);
      for (int i = 0; i < S; ++i) {
        T[to(a + 1, i + 1)] = M[ret[b]][ret[i]];
        T[to(b + 1, i + 1)] = M[ret[a]][ret[i]];
        T[to(i + 1, a + 1)] = M[ret[i]][ret[b]];
        T[to(i + 1, b + 1)] = M[ret[i]][ret[a]];
      }
      T[to(a + 1, a + 1)] = M[ret[b]][ret[b]];
      T[to(b + 1, b + 1)] = M[ret[a]][ret[a]];
      T[to(a + 1, b + 1)] = M[ret[b]][ret[a]];
      T[to(b + 1, a + 1)] = M[ret[a]][ret[b]];
      double s = 0;
      auto search = [&](int p) {
        if (T[p] == 0) return;
        int qi = 0, qs = 1, sum = T[p];
        T[p] = 0;
        queue[0] = p;
        auto push = [&](int p) {
          if (T[p]) {
            sum += T[p];
            T[p] = 0;
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
        if (s < t) s = t;
      };
      for (int i = 1; i <= S; ++i) {
        search(to(i, a + 1));
        search(to(i, b + 1));
        search(to(a + 1, i));
        search(to(b + 1, i));
      }

      if (score <
          s * (1 + get_random_double() * time * (iter - prev) / S / 5)) {
        score = s;
        prev = iter;
        for (int i = 0; i < S; ++i) {
          C[to(a + 1, i + 1)] = M[ret[b]][ret[i]];
          C[to(b + 1, i + 1)] = M[ret[a]][ret[i]];
          C[to(i + 1, a + 1)] = M[ret[i]][ret[b]];
          C[to(i + 1, b + 1)] = M[ret[i]][ret[a]];
        }
        C[to(a + 1, a + 1)] = M[ret[b]][ret[b]];
        C[to(b + 1, b + 1)] = M[ret[a]][ret[a]];
        C[to(a + 1, b + 1)] = M[ret[b]][ret[a]];
        C[to(b + 1, a + 1)] = M[ret[a]][ret[b]];
        swap(ret, a, b);
        if (best < s) {
          best = s;
          ans = ret;
        }
      }
      ++iter;
    }
    return ans;
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
