#include "buffer/buffer_pool_manager.hpp"
#include "common/debug.hpp"
#include "common/macros.hpp"
#include "storage/table_heap.hpp"
#include "storage/tuple.hpp"
#include "gtest/gtest.h"
#include <iostream>

TEST(StorageTest, SimpleTablePageTest) {
  const size_t buffer_pool_size = 10;
  auto *dm = new db::DiskManager("test.db");
  auto *bpm = new db::BufferPoolManager(buffer_pool_size, dm, 0);

  auto table_heap = new db::TableHeap(bpm);
  auto c1 = db::Column("user_id", db::TypeId::INTEGER);
  auto c2 = db::Column("user_name", db::TypeId::VARCHAR, 256);
  auto schema = db::Schema({c1, c2});
  auto v1 = db::Value(db::TypeId::INTEGER, 239);
  auto v2 = db::Value(db::TypeId::VARCHAR, "hihihi", 6, true);

  auto tuple = db::Tuple({std::move(v1), std::move(v2)}, schema);
  // std::cout << tuple.ToString(schema) << std::endl;

  auto meta = db::TupleMeta{false};
  auto rid1 = table_heap->InsertTuple(meta, tuple);
  auto rid2 = table_heap->InsertTuple(meta, tuple);
  auto rid3 = table_heap->InsertTuple(meta, tuple);

  ASSERT_EQ(rid1.has_value(), true);
  ASSERT_EQ(rid2.has_value(), true);
  ASSERT_EQ(rid3.has_value(), true);

  auto [meta1, tuple1] = table_heap->GetTuple(*rid1);
  auto [meta2, tuple2] = table_heap->GetTuple(*rid2);

  // assett that the tuple is the same as the inserted tuple
  ASSERT_EQ(tuple.ToString(schema), "(239, hihihi)");
  ASSERT_EQ(tuple.ToString(schema), tuple1.ToString(schema));
  ASSERT_EQ(tuple.ToString(schema), tuple2.ToString(schema));
}
