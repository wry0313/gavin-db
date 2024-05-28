#include "catalog/schema.hpp"
#include "storage/tuple.hpp"
#include "gtest/gtest.h"
using namespace std::literals::string_view_literals;
TEST(StorageTest, TupleTest) {
  auto c1 = db::Column("user_id", db::TypeId::INTEGER);
  auto c2 = db::Column("user_name", db::TypeId::VARCHAR, 40);
  auto c3 = db::Column("user_middle_name", db::TypeId::VARCHAR, 40);
  auto schema = db::Schema({c1, c2, c3});
  auto v1 = db::Value(db::TypeId::INTEGER, 2);
  auto v2 = db::Value(db::TypeId::VARCHAR, std::string("gavin"));
  auto v3 = db::Value(db::TypeId::VARCHAR, "wang"sv);
  auto tuple = db::Tuple({v1, v2, v3}, schema);
  ASSERT_EQ(tuple.ToString(schema), "(2, gavin, wang)");
}

TEST(StorageTest, TupleTest2) {
  auto c1 = db::Column("user_id", db::TypeId::INTEGER);
  auto c2 = db::Column("user_name", db::TypeId::VARCHAR, 3);
  auto schema = db::Schema({c1, c2});
  uint32_t num = 2;
  auto v1 = db::Value(db::TypeId::INTEGER, num);
  // std::string_view sv_var = "hii4";
  // auto v2 = db::Value(db::TypeId::VARCHAR, "hii4");

  // EXPECT_THROW(db::Tuple({v1, v2}, schema), std::runtime_error);
}
