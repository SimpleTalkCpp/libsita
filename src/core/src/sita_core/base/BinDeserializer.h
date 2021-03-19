#pragma once

#include "sita_base.h"
#include "Error.h"
#include "ByteOrder.h"

namespace sita {

class BinDeserializer;

class BinDeserializer : public NonCopyable {
public:
	BinDeserializer(const u8* data, size_t dataSize) 
		: _data(data)
		, _end(data + dataSize)
	{}

	BinDeserializer(const Vector<u8>& v)
		: BinDeserializer(v.data(), v.size())
	{}

	SITA_INLINE void io_fixed(i8 & value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(i16& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(i32& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(i64& value)	{ _io_fixed(value); }

	SITA_INLINE void io_fixed(u8 & value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(u16& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(u32& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(u64& value)	{ _io_fixed(value); }

	template<class T>
	void io(T& value);

	size_t remain() const { return _end - _cur; }

private:
	template<class T> SITA_INLINE void _io_fixed(T& value);

	const u8* _data = nullptr;
	const u8* _cur  = nullptr;
	const u8* _end  = nullptr;
};

template<class T> SITA_INLINE
void BinDeserializer::_io_fixed(T& value) {
	if (remain() < sizeof(value)) {
		throw SITA_ERROR("BinDeserializer out of range");
	}
	value = ByteOrder::LEtoHost::get(*reinterpret_cast<const T*>(_cur));
	_cur += sizeof(value);
}



} // namespace