#include "check.hpp"
#include "leaderboard/leaderboard.hpp"

#include <cstdint>
#include <string>

using leaderboard::Leaderboard;
using leaderboard::Score;

TEST(keeps_scores_highest_first) {
    Leaderboard board;
    board.submit({"AAA", 300});
    board.submit({"BBB", 900});
    board.submit({"CCC", 500});
    const auto top = board.top();
    CHECK_EQ(top.size(), std::size_t{3});
    CHECK_EQ(top[0].initials, std::string("BBB"));
    CHECK_EQ(top[1].initials, std::string("CCC"));
    CHECK_EQ(top[2].initials, std::string("AAA"));
}

TEST(equal_points_keep_earlier_entry_first) {
    Leaderboard board;
    board.submit({"OLD", 500});
    board.submit({"NEW", 500});
    const auto top = board.top();
    CHECK_EQ(top[0].initials, std::string("OLD"));
    CHECK_EQ(top[1].initials, std::string("NEW"));
}

TEST(keeps_only_top_ten) {
    Leaderboard board;
    for (std::uint32_t i = 1; i <= 10; ++i) {
        CHECK(board.submit({"AAA", i * 100}));
    }
    CHECK(!board.submit({"LOW", 100}));
    CHECK(board.submit({"TOP", 5000}));
    const auto top = board.top();
    CHECK_EQ(top.size(), std::size_t{10});
    CHECK_EQ(top.front().initials, std::string("TOP"));
    CHECK_EQ(top.back().points, std::uint32_t{200});
}
