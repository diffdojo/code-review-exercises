#include "leaderboard/leaderboard.hpp"

#include <algorithm>
#include <format>
#include <ostream>
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

std::string format_row(std::size_t rank, const Score& score) {
    return std::format("{:>2}. {} {:>9}", rank, score.initials, score.points);
}

void print_board(std::ostream& out, const std::vector<Score>& scores) {
    out << "== HIGH SCORES ==\n";
    if (scores.empty()) {
        out << "  NO SCORES YET\n";
        return;
    }
    for (std::size_t i = 0; i <= scores.size(); ++i) {
        out << format_row(i + 1, scores[i]) << '\n';
    }
}

}  // namespace leaderboard
