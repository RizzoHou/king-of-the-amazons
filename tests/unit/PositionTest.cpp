#include <gtest/gtest.h>
#include "core/Position.hpp"

using namespace amazons;

TEST(PositionTest, DefaultConstructor) {
    Position pos;
    EXPECT_EQ(pos.row, 0);
    EXPECT_EQ(pos.col, 0);
}

TEST(PositionTest, ParameterizedConstructor) {
    Position pos(3, 5);
    EXPECT_EQ(pos.row, 3);
    EXPECT_EQ(pos.col, 5);
}

TEST(PositionTest, EqualityOperator) {
    Position pos1(3, 5);
    Position pos2(3, 5);
    Position pos3(4, 5);
    
    EXPECT_TRUE(pos1 == pos2);
    EXPECT_FALSE(pos1 == pos3);
}

TEST(PositionTest, InequalityOperator) {
    Position pos1(3, 5);
    Position pos2(3, 5);
    Position pos3(4, 5);
    
    EXPECT_FALSE(pos1 != pos2);
    EXPECT_TRUE(pos1 != pos3);
}

TEST(PositionTest, IsValid) {
    Position valid1(0, 0);
    Position valid2(9, 9);
    Position valid3(5, 5);
    
    Position invalid1(-1, 0);
    Position invalid2(0, -1);
    Position invalid3(10, 5);
    Position invalid4(5, 10);
    
    EXPECT_TRUE(valid1.isValid());
    EXPECT_TRUE(valid2.isValid());
    EXPECT_TRUE(valid3.isValid());
    
    EXPECT_FALSE(invalid1.isValid());
    EXPECT_FALSE(invalid2.isValid());
    EXPECT_FALSE(invalid3.isValid());
    EXPECT_FALSE(invalid4.isValid());
}

TEST(PositionTest, ToString) {
    Position pos(3, 5);
    std::string str = pos.toString();
    // Expect format like "(3,5)" or similar
    EXPECT_FALSE(str.empty());
    EXPECT_NE(str.find('3'), std::string::npos);
    EXPECT_NE(str.find('5'), std::string::npos);
}

TEST(PositionTest, FromString) {
    Position pos = Position::fromString("(3,5)");
    EXPECT_EQ(pos.row, 3);
    EXPECT_EQ(pos.col, 5);
    
    // Test invalid format
    EXPECT_THROW(Position::fromString("invalid"), std::invalid_argument);
    EXPECT_THROW(Position::fromString("(3,5"), std::invalid_argument);
    EXPECT_THROW(Position::fromString("3,5)"), std::invalid_argument);
}
