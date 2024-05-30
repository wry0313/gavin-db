#include "catalog/column.hpp"

#include "common/type.hpp"

#include <cstdint>
#include <sstream>
namespace db {

auto Column::ToString(bool simplified) const -> std::string {
	if (simplified) {
		std::ostringstream os;
		os << column_name_ << ":" << Type::TypeIdToString(column_type_);
		if (column_type_ == TypeId::VARCHAR) {
			os << "(" << length_ << ")";
		}
		return (os.str());
	}

	std::ostringstream os;

	os << "Column[" << column_name_ << ", " << Type::TypeIdToString(column_type_) << ", " << "Offset:" << column_offset_
	   << ", ";
	os << "Length:" << length_;
	os << "]";
	return (os.str());
}

void Column::SerializeTo(char *storage) const {
	// serialize the type id
	uint32_t offset = 0;
	memcpy(storage, &column_type_, sizeof(column_type_));
	offset += sizeof(column_type_);
	// serialize the length
	memcpy(storage + offset, &length_, sizeof(length_));
	offset += sizeof(length_);
	// serialize the col name size
	uint32_t col_name_size = column_name_.size();
	memcpy(storage + offset, &col_name_size, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	// serialize the actual column name
	memcpy(storage + offset, column_name_.data(), column_name_.size());
}
void Column::DeserializeFrom(const char *storage) {
	uint32_t offset = 0;
	column_type_ = *reinterpret_cast<const TypeId *>(storage);
	offset += sizeof(column_type_);
	length_ = *reinterpret_cast<const uint32_t *>(storage + offset);
	offset += sizeof(uint32_t);
	uint32_t col_name_size = *reinterpret_cast<const uint32_t *>(storage + offset);
	offset += sizeof(uint32_t);
	column_name_.assign(storage + offset, col_name_size);
	ASSERT(column_name_.size() == col_name_size, "Column name size mismatch");
	ASSERT(length_ != 0, "Column length is not set");
	ASSERT(column_type_ != TypeId::INVALID, "Column type is not set");
}

auto Column::GetSerializationSize() const -> uint32_t {
	return sizeof(TypeId) + sizeof(uint32_t) + column_name_.size() + sizeof(length_);
}

} // namespace db
