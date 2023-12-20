#include <bits/stdc++.h>
using namespace std;

vector<string> read_lines(string file_name = "input.txt") {
  vector<string> res;
  string line;
  ifstream file(file_name);
  if (file.is_open()) {
    while (getline(file, line)) {
      res.push_back(line);
    }
    file.close();
  }
  return res;
}

vector<string> split(string s, char c = ' ') {
  vector<string> res;
  string cur = "";
  for (char ch : s) {
    if (ch == c) {
      if (!cur.empty())
        res.push_back(cur);
      cur = "";
    } else {
      cur += ch;
    }
  }
  if (!cur.empty()) {
    res.push_back(cur);
  }
  return res;
}

string rltrim(string s, char c = ' ') {
  int lo = -1, hi = -1;
  for (int i = 0; i < (int)s.size(); i++) {
    if (s[i] != c) {
      lo = i;
      break;
    }
  }
  for (int i = (int)s.size() - 1; i >= 0; i--) {
    if (s[i] != c) {
      hi = i;
      break;
    }
  }
  if (lo != -1 and hi != -1) {
    string res = "";
    for (int i = lo; i <= hi; i++)
      res += s[i];
    return res;
  }
  return "";
}

int64_t silver(vector<string> lines) {
  int64_t ans = 0;
  vector<string> a = lines;
  const int n = (int)a.size(), m = (int)a[0].size();

  pair<int, int> src = {-1, -1};

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (a[i][j] == 'S') {
        src = {i, j};
        break;
      }
    }
    if (src != make_pair(-1, -1))
      break;
  }

  auto ok = [&](int x, int y) -> bool {
    return 0 <= x and x < n and 0 <= y and y < m;
  };

  auto moves = [&](int x, int y) -> vector<pair<int, int>> {
    char c = a[x][y];
    if (c == 'S') {
      return {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    } else if (c == 'F') {
      return {{0, 1}, {1, 0}};
    } else if (c == 'L') {
      return {{0, 1}, {-1, 0}};
    } else if (c == 'J') {
      return {{0, -1}, {-1, 0}};
    } else if (c == '7') {
      return {{0, -1}, {1, 0}};
    } else if (c == '-') {
      return {{0, -1}, {0, 1}};
    } else if (c == '|') {
      return {{1, 0}, {-1, 0}};
    }
    return {};
  };

  auto possible = [&](int x, int y, int dx, int dy) -> bool {
    if (!ok(x + dx, y + dy))
      return false;
    char c = a[x + dx][y + dy];
    string s = "FLJ7-|";
    bool ok = false;
    for (char ch : s) {
      if (ch == c)
        ok = true;
    }
    if (ok) {
      for (auto &[rdx, rdy] : moves(x + dx, y + dy)) {
        if (dx == -rdx and dy == -rdy)
          return true;
      }
    }
    return false;
  };

  auto [sx, sy] = src;
  vector<vector<int64_t>> d(n, vector<int64_t>(m, -1));
  queue<pair<int, int>> q;
  d[sx][sy] = 0;
  q.emplace(sx, sy);
  while (!q.empty()) {
    auto [ux, uy] = q.front();
    ans = max(ans, d[ux][uy]);
    q.pop();
    for (auto &[dx, dy] : moves(ux, uy)) {
      int vx = ux + dx, vy = uy + dy;
      if (possible(ux, uy, dx, dy) and d[vx][vy] == -1) {
        d[vx][vy] = d[ux][uy] + 1;
        q.emplace(vx, vy);
      }
    }
  }

  return ans;
}

int64_t gold(vector<string> lines) {
  int64_t ans = 0;
  vector<string> a(2 * lines.size(), string(2 * lines[0].size(), '.'));
  for (int i = 0; i < (int)lines.size(); i++) {
    for (int j = 0; j < (int)lines[0].size(); j++) {
      a[2 * i][2 * j] = lines[i][j];
    }
  }

  const int n = (int)a.size(), m = (int)a[0].size();

  auto ok = [&](int x, int y) -> bool {
    return 0 <= x and x < n and 0 <= y and y < m;
  };

  auto moves = [&](int x, int y) -> vector<pair<int, int>> {
    char c = a[x][y];
    if (c == 'S') {
      return {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    } else if (c == 'F') {
      return {{0, 1}, {1, 0}};
    } else if (c == 'L') {
      return {{0, 1}, {-1, 0}};
    } else if (c == 'J') {
      return {{0, -1}, {-1, 0}};
    } else if (c == '7') {
      return {{0, -1}, {1, 0}};
    } else if (c == '-') {
      return {{0, -1}, {0, 1}};
    } else if (c == '|') {
      return {{1, 0}, {-1, 0}};
    }
    return {};
  };

  auto possible = [&](int x, int y, int dx, int dy) -> bool {
    if (!ok(x + dx, y + dy))
      return false;
    char c = a[x + dx][y + dy];
    string s = "SFLJ7-|";
    bool ok = false;
    for (char ch : s) {
      if (ch == c)
        ok = true;
    }
    if (ok) {
      for (auto &[rdx, rdy] : moves(x + dx, y + dy)) {
        if (dx == -rdx and dy == -rdy)
          return true;
      }
    }
    return false;
  };

  for (int i = 0; i < n; i++) {
    for (int j = 1; j < m; j += 2) {
      if (possible(i, j, 0, -1) and possible(i, j, 0, 1))
        a[i][j] = '-';

      if (possible(i, j, -1, 0) and possible(i, j, 1, 0))
        a[i][j] = '|';
    }
  }
  for (int j = 0; j < m; j++) {
    for (int i = 1; i < n; i += 2) {
      if (possible(i, j, 0, -1) and possible(i, j, 0, 1))
        a[i][j] = '-';

      if (possible(i, j, -1, 0) and possible(i, j, 1, 0))
        a[i][j] = '|';
    }
  }

  pair<int, int> src = {-1, -1};

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (a[i][j] == 'S') {
        src = {i, j};
        break;
      }
    }
    if (src != make_pair(-1, -1))
      break;
  }

  auto [sx, sy] = src;
  vector<vector<int64_t>> d(n, vector<int64_t>(m, -1));
  queue<pair<int, int>> q;
  d[sx][sy] = 0;
  q.emplace(sx, sy);
  while (!q.empty()) {
    auto [ux, uy] = q.front();
    q.pop();
    for (auto &[dx, dy] : moves(ux, uy)) {
      int vx = ux + dx, vy = uy + dy;
      if (possible(ux, uy, dx, dy) and d[vx][vy] == -1) {
        d[vx][vy] = d[ux][uy] + 1;
        q.emplace(vx, vy);
      }
    }
  }

  function<void(int, int)> dfs = [&](int x, int y) -> void {
    d[x][y] = 0;
    for (auto &[dx, dy] : moves(sx, sy)) {
      int vx = x + dx, vy = y + dy;
      if (ok(vx, vy) and d[vx][vy] == -1) {
        dfs(vx, vy);
      }
    }
  };

  for (int i = 0; i < n; i++) {
    int x = i, y = 0;
    if (d[x][y] == -1)
      dfs(x, y);
    y = m - 1;
    if (d[x][y] == -1)
      dfs(x, y);
  }

  for (int j = 0; j < m; j++) {
    int x = 0, y = j;
    if (d[x][y] == -1)
      dfs(x, y);
    x = n - 1;
    if (d[x][y] == -1)
      dfs(x, y);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      ans += (d[i][j] == -1);
    }
  }

  int res = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      vector<pair<int, int>> nxt = {
          {i, j}, {i + 1, j + 1}, {i + 1, j}, {i, j + 1}};
      int count = 0;
      for (auto &[x, y] : nxt) {
        if (ok(x, y)) {
          count += (d[x][y] == -1);
        }
      }
      if (count == 4) {
        res++;
        for (auto &[x, y] : nxt) {
          d[x][y] = -2;
        }
      }
    }
  }
  ans = res;

  return ans;
}

int main() {
  vector<string> lines = read_lines();
  cout << silver(lines) << '\n';
  cout << gold(lines) << '\n';
  return 0;
}