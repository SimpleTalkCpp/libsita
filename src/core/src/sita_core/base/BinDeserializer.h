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

	template<class T> void io(T& value);

//----------
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


	SITA_INLINE size_t remain() const { return _end - _cur; }
	SITA_INLINE const u8* _advance(size_t n);

private:
	template<class T> SITA_INLINE void _io_fixed(T& value);
	template<class T> SITA_INLINE void _io_vary_unsigned(T& value);
	template<class U, class T> SITA_INLINE void _io_vary_signed(T& value);

	const u8* _data = nullptr;
	const u8* _cur  = nullptr;
	const u8* _end  = nullptr;
};

SITA_INLINE const u8* BinDeserializer::_advance(size_t n) {
	if (remain() < n) {
		throw SITA_ERROR("BinDeserializer out of range");
	}
	const u8* old = _cur;
	_cur += n;
	return old;
}

template<class T> SITA_INLINE
void BinDeserializer::_io_fixed(T& value) {
	auto* p = _advance(sizeof(value));
	value = ByteOrder::LEtoHost::get(*reinterpret_cast<const T*>(p));
}

template<class T> SITA_INLINE
void BinDeserializer::_io_vary_unsigned(T& value) {
	size_t bit = 0;
	u8 t;
	value = 0;
	for(;;){
		t = *_advance(1);
		value |= static_cast<T>( t & 0x7F ) << bit;
		if( !( t & 0x80 ) ) return;
		bit += 7;
		if( bit > sizeof(T) * 8 ) {
			throw SITA_ERROR("BinDeserializer out of range");
		}
	}
}

template<class U, class T> SITA_INLINE
void BinDeserializer::_io_vary_signed(T& value) {
	U tmp;
	_io_vary_unsigned(tmp);
	value = static_cast<T>((tmp >> 1) ^ ( -(tmp & 1)));
}

} // namespace