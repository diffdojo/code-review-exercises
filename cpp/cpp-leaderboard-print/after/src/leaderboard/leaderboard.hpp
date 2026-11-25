#pragma once

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace leaderboard {

struct Score {
    std::string initials;  // exactly 3 characters, 'A'-'Z'
    std::uint32_t points;  // 0 to 999'999'999
};

class Leaderboard {
public:
    static constexpr std::size_t kMaxEntries = 10;

    // Returns true when the score made the table.
    bool submit(Score score);

    // Highest first; equal points keep the earlier submission first.
    std::vector<Score> top() const;

private:
    std::vector<Score> entries_;
};

// One table row: rank right-aligned in 2 columns, ". ", initials, " ",
// points right-aligned in 9 columns, e.g. " 1. AAA    123450".
std::string format_row(std::size_t rank, const Score& score);

// Writes the high score table for `scores`, already in table order.
void print_board(std::ostream& out, const std::vector<Score>& scores);

}  // namespace leaderboard
