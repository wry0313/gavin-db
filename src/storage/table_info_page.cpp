#include "storage/table_info_page.hpp"
#include "catalog/schema.hpp"
#include "common/config.hpp"
#include "common/exception.hpp"
#include "common/macros.hpp"
#include <cstdint>
#include <cstring>
namespace db {
void TableInfoPage::Init(const std::string name, const Schema &schema,
                         const table_oid_t table_oid) {
  // initial persistentce
  auto table_info = TableInfo{schema, name, table_oid};
  StoreTableInfo(table_info);
}
auto TableInfoPage::GetTableInfo() -> TableInfo {
  ASSERT(table_info_offset_ >= TABLE_INFO_PAGE_HEADER_SIZE,
         "table info offset not initialized correctly");

  auto schema = Schema();
  uint32_t offset = table_info_offset_;
  schema.DeserializeFrom(page_start_ + offset);
  offset += schema.GetSerializationSize();
  uint32_t table_name_size =
      *reinterpret_cast<const uint32_t *>(page_start_ + offset);
  offset += sizeof(uint32_t);
  std::string table_name(page_start_ + offset, table_name_size);
  offset += table_name_size;
  uint32_t table_oid =
      *reinterpret_cast<const uint32_t *>(page_start_ + offset);

  return {schema, table_name, table_oid};
}
void TableInfoPage::UpdateTableSchema(const Schema &schema) {
  auto old_table_info = GetTableInfo();
  auto new_table_info =
      TableInfo{schema, old_table_info.name_, old_table_info.table_oid_};

  StoreTableInfo(new_table_info);
}

void TableInfoPage::StoreTableInfo(const TableInfo &table_info) {

  uint32_t table_info_size = table_info.schema_.GetSerializationSize() +
                             sizeof(uint32_t) + table_info.name_.size() +
                             sizeof(table_oid_t);
  uint32_t offset = PAGE_SIZE - table_info_size;
  if (offset < TABLE_INFO_PAGE_HEADER_SIZE) {
    throw Exception(
        "Schema + table name is too large to be stored on one page");
  }

  table_info_offset_ = offset;
  table_info.SerializeTo(page_start_ + offset);
}

} // namespace db
