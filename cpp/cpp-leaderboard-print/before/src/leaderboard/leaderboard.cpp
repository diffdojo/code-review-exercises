#include "leaderboard/leaderboard.hpp"

#include <algorithm>
#include <utility>

namespace leaderboard {

bool Leaderboard::submit(Score score) {
    const auto pos = std::upper_bound(
        entries_.begin(), entries_.end(), score.points,
        [](std::uint32_t points, const Score& entry) { return points > entry.points; });
    if (pos == entries_.end() && entries_.size() >= kMaxEntries) {
        return false;
    }
    entries_.insert(pos, std::move(score));
    if (entries_.size() > kMaxEntries) {
        entries_.pop_back();
    }
    return true;
}

std::vector<Score> Leaderboard::top() const {
    return entries_;
}

}  // namespace leaderboard
