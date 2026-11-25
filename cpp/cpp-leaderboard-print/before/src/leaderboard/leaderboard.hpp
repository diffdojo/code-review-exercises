#pragma once

#include <cstddef>
#include <cstdint>
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

}  // namespace leaderboard
