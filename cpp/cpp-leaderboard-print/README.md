# High score table for the arcade cabinet

C++ · beginner

You are the reviewer. Read the ticket, then the diff, and decide: approve or request changes?

## Ticket

- Add `leaderboard::print_board(std::ostream& out, const std::vector<Score>& scores)`, which writes the high score table for the cabinet's attract-mode screen. `scores` is what `Leaderboard::top()` returns: 0 to 10 entries, already in table order (highest first).
- The table is the header line `== HIGH SCORES ==` followed by exactly one row per entry of `scores`, in the order given, and nothing else. The entry at index 0 gets rank 1, the next rank 2, and so on.
- Each row is built by the new `format_row(rank, score)`: the rank right-aligned in 2 columns, `. `, the initials, a space, and the points right-aligned in 9 columns, for example ` 1. AAA    123450`. Every line ends with `\n`.
- When `scores` is empty, the table is the header followed by the single line `  NO SCORES YET` (two leading spaces).

## Pull request description

Adds the high score table that the attract-mode screen will show between demo loops. `leaderboard.hpp` declares two new functions: `format_row`, which lays out one row (rank, initials, points) with `std::format`, and `print_board`, which writes the header and then one row per entry of the list `Leaderboard::top()` returns, or a `NO SCORES YET` line when nobody has played yet. `leaderboard.cpp` implements them next to the existing `submit` and `top`, which are unchanged. `tests/leaderboard_test.cpp` gains tests for the new table code.

## Context

C++20, built with CMake (existing, unchanged; it compiles every `src/**/*.cpp` into the `cabinet` library and every `tests/*_test.cpp` into one test binary) by Apple clang 17 locally and GCC 13 in CI, with `-Wall -Wextra -Wconversion` treated as errors in CI; standard library only. The cabinet runs a single-threaded game loop, so nothing here is shared between threads. `Score` and `Leaderboard` are existing and unchanged: `Score::initials` is always exactly 3 characters `A` to `Z` (the entry screen enforces it) and `Score::points` is 0 to 999,999,999; `Leaderboard::top()` returns a copy of the table, at most `kMaxEntries` (10) entries, highest first. The tests use the repo's existing helper `tests/check.hpp` (`TEST(name)` registers a test, `CHECK(expr)` and `CHECK_EQ(a, b)` record a failure, and `tests/check_main.cpp` runs every registered test and exits non-zero on any failure). The attract-mode screen and the operator's service menu call `print_board(display_stream, board.top())` in a follow-up PR; until then nothing calls it outside the tests.

## Diff

`src/leaderboard/leaderboard.hpp` (modified)

```diff
--- a/src/leaderboard/leaderboard.hpp
+++ b/src/leaderboard/leaderboard.hpp
@@ -2,6 +2,7 @@
 
 #include <cstddef>
 #include <cstdint>
+#include <iosfwd>
 #include <string>
 #include <vector>
 
@@ -26,4 +27,11 @@
     std::vector<Score> entries_;
 };
 
+// One table row: rank right-aligned in 2 columns, ". ", initials, " ",
+// points right-aligned in 9 columns, e.g. " 1. AAA    123450".
+std::string format_row(std::size_t rank, const Score& score);
+
+// Writes the high score table for `scores`, already in table order.
+void print_board(std::ostream& out, const std::vector<Score>& scores);
+
 }  // namespace leaderboard
```

`src/leaderboard/leaderboard.cpp` (modified)

```diff
--- a/src/leaderboard/leaderboard.cpp
+++ b/src/leaderboard/leaderboard.cpp
@@ -1,6 +1,8 @@
 #include "leaderboard/leaderboard.hpp"
 
 #include <algorithm>
+#include <format>
+#include <ostream>
 #include <utility>
 
 namespace leaderboard {
@@ -23,4 +25,19 @@
     return entries_;
 }
 
+std::string format_row(std::size_t rank, const Score& score) {
+    return std::format("{:>2}. {} {:>9}", rank, score.initials, score.points);
+}
+
+void print_board(std::ostream& out, const std::vector<Score>& scores) {
+    out << "== HIGH SCORES ==\n";
+    if (scores.empty()) {
+        out << "  NO SCORES YET\n";
+        return;
+    }
+    for (std::size_t i = 0; i <= scores.size(); ++i) {
+        out << format_row(i + 1, scores[i]) << '\n';
+    }
+}
+
 }  // namespace leaderboard
```

`tests/leaderboard_test.cpp` (modified)

```diff
--- a/tests/leaderboard_test.cpp
+++ b/tests/leaderboard_test.cpp
@@ -2,10 +2,13 @@
 #include "leaderboard/leaderboard.hpp"
 
 #include <cstdint>
+#include <sstream>
 #include <string>
 
 using leaderboard::Leaderboard;
 using leaderboard::Score;
+using leaderboard::format_row;
+using leaderboard::print_board;
 
 TEST(keeps_scores_highest_first) {
     Leaderboard board;
@@ -40,3 +43,15 @@
     CHECK_EQ(top.front().initials, std::string("TOP"));
     CHECK_EQ(top.back().points, std::uint32_t{200});
 }
+
+TEST(row_pads_rank_and_points) {
+    CHECK_EQ(format_row(1, {"AAA", 123450}), std::string(" 1. AAA    123450"));
+    CHECK_EQ(format_row(10, {"ZED", 999999999}), std::string("10. ZED 999999999"));
+    CHECK_EQ(format_row(3, {"BOB", 0}), std::string(" 3. BOB         0"));
+}
+
+TEST(empty_board_says_so) {
+    std::ostringstream out;
+    print_board(out, {});
+    CHECK_EQ(out.str(), std::string("== HIGH SCORES ==\n  NO SCORES YET\n"));
+}
```

The full files are in [`before/`](before) and [`after/`](after).

## Your review

Before you look anywhere else, write down:

1. Your verdict: approve or request changes.
2. The line you would comment on, and what you would say.
3. The input or sequence of events that breaks it, if you think something does.

## Check your review

Open the same PR in DiffDojo, leave your comments on the lines and submit: [High score table for the arcade cabinet](https://app.diffdojo.com/challenges/cpp-leaderboard-print?utm_source=github&utm_campaign=exercises). No signup needed for this one. You get the expected review and see what you caught and what you missed.
