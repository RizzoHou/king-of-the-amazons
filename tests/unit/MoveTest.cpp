#include <gtest/gtest.h>
#include "core/Move.hpp"
#include "core/Position.hpp"

using namespace amazons;

TEST(MoveTest, DefaultConstructor) {
    Move move;
    EXPECT_TRUE(move.from == Position(0, 0));
    EXPECT_TRUE(move.to == Position(0, 0));
    EXPECT_TRUE(move.arrow == Position(0, 0));
}

TEST(MoveTest, ParameterizedConstructor) {
    Position from(1, 2);
    Position to(3, 4);
    Position arrow(5, 6);
    
    Move move(from, to, arrow);
    EXPECT_TRUE(move.from == from);
    EXPECT_TRUE(move.to == to);
    EXPECT_TRUE(move.arrow == arrow);
}

TEST(MoveTest, EqualityOperator) {
    Move move1(Position(1, 2), Position(3, 4), Position(5, 6));
    Move move2(Position(1, 2), Position(3, 4), Position(5, 6));
    Move move3(Position(1, 2), Position(3, 4), Position(5, 7));
    
    EXPECT_TRUE(move1 == move2);
    EXPECT_FALSE(move1 == move3);
}

TEST(MoveTest, InequalityOperator) {
    Move move1(Position(1, 2), Position(3, 4), Position(5, 6));
    Move move2(Position(1, 2), Position(3, 4), Position(5, 6));
    Move move3(Position(1, 2), Position(3, 4), Position(5, 7));
    
    EXPECT_FALSE(move1 != move2);
    EXPECT_TRUE(move1 != move3);
}

TEST(MoveTest, IsValid) {
    Move valid(Position(1, 2), Position(3, 4), Position(5, 6));
    Move invalid1(Position(-1, 2), Position(3, 4), Position(5, 6));
    Move invalid2(Position(1, 2), Position(8, 4), Position(5, 6));
    Move invalid3(Position(1, 2), Position(3, 4), Position(5, 8));
    
    EXPECT_TRUE(valid.isValid());
    EXPECT_FALSE(invalid1.isValid());
    EXPECT_FALSE(invalid2.isValid());
    EXPECT_FALSE(invalid3.isValid());
}

TEST(MoveTest, ToString) {
    Move move(Position(1, 2), Position(3, 4), Position(5, 6));
    std::string str = move.toString();
    
    // New format: "1 2 3 4 5 6"
    EXPECT_EQ(str, "1 2 3 4 5 6");
}

TEST(MoveTest, FromString) {
    std::string str = "1 2 3 4 5 6";
    Move move = Move::fromString(str);
    
    EXPECT_TRUE(move.from == Position(1, 2));
    EXPECT_TRUE(move.to == Position(3, 4));
    EXPECT_TRUE(move.arrow == Position(5, 6));
    
    // Test invalid strings
    EXPECT_THROW(Move::fromString("invalid"), std::invalid_argument);
    EXPECT_THROW(Move::fromString("1 2 3 4 5"), std::invalid_argument); // Missing value
    EXPECT_THROW(Move::fromString("1 2 3 4 5 6 7"), std::invalid_argument); // Extra value
    EXPECT_THROW(Move::fromString("1 2 3 a 5 6"), std::invalid_argument); // Non-numeric
}