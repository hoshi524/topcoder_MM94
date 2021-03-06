#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

constexpr double TIME_LIMIT = 20000000;
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

class ConnectedComponent {
 public:
  vector<int> permute(vector<int> matrix) {
    double START_TIME = get_time();
    int S = (int)sqrt(matrix.size());
    char M[MAX_S][MAX_S];
    char C[MAX_S * MAX_S];
    char T[MAX_S * MAX_S];
    int queue[MAX_S * MAX_S];
    memset(C, 0, sizeof(C));
    memset(T, 0, sizeof(T));
    for (int i = 0; i < S; ++i) {
      for (int j = 0; j < S; ++j) {
        M[i][j] = C[to(i + 1, j + 1)] = matrix[i * S + j];
      }
    }

    vector<int> ret(S);
    vector<int> ans(S);
    double score = -1;
    double best = -1;
    for (int i = 0; i < S; ++i) {
      ret[i] = i;
    }

    int iter = 0, prev = 0;
    int copy_byte = (S + 1) * MAX_S;
    while (true) {
      double time = (START_TIME + TIME_LIMIT - get_time()) / TIME_LIMIT;
      if (time < 0) break;
      int a, b;
      do {
        a = get_random() % S;
        b = get_random() % S;
      } while (a == b);

      memcpy(T, C, copy_byte);
      auto swap = [&](char *x, int a, int b) {
        for (int i = 0; i < S; ++i) {
          x[to(a + 1, i + 1)] = M[ret[b]][ret[i]];
          x[to(b + 1, i + 1)] = M[ret[a]][ret[i]];
          x[to(i + 1, a + 1)] = M[ret[i]][ret[b]];
          x[to(i + 1, b + 1)] = M[ret[i]][ret[a]];
        }
        x[to(a + 1, a + 1)] = M[ret[b]][ret[b]];
        x[to(b + 1, b + 1)] = M[ret[a]][ret[a]];
        x[to(a + 1, b + 1)] = M[ret[b]][ret[a]];
        x[to(b + 1, a + 1)] = M[ret[a]][ret[b]];
      };
      swap(T, a, b);
      {
        auto print = [&](char *t) {
          char m[MAX_S * MAX_S];
          memcpy(m, t, sizeof(m));
          double s = 0;
          for (int p = 0; p < MAX_S * MAX_S; ++p) {
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
              if (s < t) s = t;
            };
            search(p);
          }
          for (int i = 1; i <= S; ++i) {
            for (int j = 1; j <= S; ++j) {
              int p = to(i, j);
              if (t[p]) {
                fprintf(stderr, "%3d", t[p]);
              } else {
                fprintf(stderr, "   ", t[p]);
              }
            }
            fprintf(stderr, "\n");
          }
          fprintf(stderr, "%f %d\n", s, (iter - prev));
        };
        print(C);
        print(T);
        fprintf(stderr, "\n");
        sleep(1);
      }
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
      {
        int c = to((S >> 1) + 1, (S >> 1) + 1);
        search(c + 1);
        search(c - 1);
        search(c + MAX_S);
        search(c - MAX_S);
      }

      if (score < s * (1 + time * (iter - prev) / S / 10)) {
        score = s;
        prev = iter;
        swap(C, a, b);
        int t = ret[a];
        ret[a] = ret[b];
        ret[b] = t;
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
