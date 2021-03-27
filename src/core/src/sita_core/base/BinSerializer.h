#pragma once

#include "sita_base.h"
#include "Error.h"
#include "ByteOrder.h"

namespace sita {

template<class SE, class T> SITA_INLINE
void BinSerilizer_io(SE& se, T& value) { value.io(se); }

class BinSerializer : public NonCopyable {
public:
	BinSerializer(Vector<u8>& buf) : _buf(&buf) {}

	SITA_INLINE void io(i8 & value)	{ io_vary(value); }
	SITA_INLINE void io(i16& value)	{ io_vary(value); }
	SITA_INLINE void io(i32& value)	{ io_vary(value); }
	SITA_INLINE void io(i64& value)	{ io_vary(value); }

	SITA_INLINE void io(u8 & value)	{ io_vary(value); }
	SITA_INLINE void io(u16& value)	{ io_vary(value); }
	SITA_INLINE void io(u32& value)	{ io_vary(value); }
	SITA_INLINE void io(u64& value)	{ io_vary(value); }

	SITA_INLINE void io(f32& value)	{ io_fixed(value); }
	SITA_INLINE void io(f64& value)	{ io_fixed(value); }

	template<class T> SITA_INLINE
	void io(T& value) { BinSerilizer_io(*this, value); }

	template<class T> SITA_INLINE
	BinSerializer& operator<<(T& value) { io(value); return *this; }

//-----------
	SITA_INLINE void io_fixed(i8 & value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(i16& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(i32& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(i64& value)	{ _io_fixed(value); }

	SITA_INLINE void io_fixed(u8 & value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(u16& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(u32& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(u64& value)	{ _io_fixed(value); }

	SITA_INLINE void io_fixed(f32& value)	{ _io_fixed(value); }
	SITA_INLINE void io_fixed(f64& value)	{ _io_fixed(value); }

	SITA_INLINE void io_vary(i8 & value)	{ _io_vary_signed<u8 >(value); }
	SITA_INLINE void io_vary(i16& value)	{ _io_vary_signed<u16>(value); }
	SITA_INLINE void io_vary(i32& value)	{ _io_vary_signed<u32>(value); }
	SITA_INLINE void io_vary(i64& value)	{ _io_vary_signed<u64>(value); }

	SITA_INLINE void io_vary(u8 & value)	{ _io_vary_unsigned(value); }
	SITA_INLINE void io_vary(u16& value)	{ _io_vary_unsigned(value); }
	SITA_INLINE void io_vary(u32& value)	{ _io_vary_unsigned(value); }
	SITA_INLINE void io_vary(u64& value)	{ _io_vary_unsigned(value); }

	SITA_INLINE u8* _advance(size_t n);

private:
	template<class T> SITA_INLINE void _io_fixed(T& value);
	template<class T> SITA_INLINE void _io_vary_unsigned(T& value);
	template<class U, class T> SITA_INLINE void _io_vary_signed(T& value);

private:
	Vector<u8>* _buf = nullptr;
};

SITA_INLINE
u8* BinSerializer::_advance(size_t n) {
	size_t oldSize = _buf->size();
	_buf->resize(oldSize + n);
	return _buf->data() + oldSize;
}

template<class T> SITA_INLINE
void BinSerializer::_io_fixed(T& value) {
	auto* p = _advance(sizeof(value));
	*reinterpret_cast<T*>(p) = LittleEndian::FromHost::get(value);
}

template<class T> SITA_INLINE
void BinSerializer::_io_vary_unsigned(T& value) {
	T tmp = value;
	for(size_t i = 0; i < sizeof(T) + 1; i++) {
		u8 highBit = tmp >= 0x80 ? 0x80 : 0;
		_buf->push_back(static_cast<u8>(tmp) | highBit);
		tmp >>= 7;
		if (tmp == 0) return;
	}
	throw SITA_ERROR("BinSerializer::_io_vary_unsigned");
}

template<class U, class T> SITA_INLINE
void BinSerializer::_io_vary_signed(T& value) {
// ZigZag encoding - https://developers.google.com/protocol-buffers/docs/encoding
	static_assert(sizeof(U) == sizeof(T));
	U tmp = static_cast<U>( ( value << 1 ) ^ ( value >> (sizeof(T)*8-1) ) );
	_io_vary_unsigned(tmp);
}

} // namespace