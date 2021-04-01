#pragma once

#define SITA_COMMA ,
#define SITA_EMPTY
#define SITA_STRINGIFY(...)	#__VA_ARGS__

#define SITA_IDENTITY(x) x
#define SITA_CALL(M, ARGS) SITA_IDENTITY( M(ARGS) )

#define SITA_VA_ARGS_COUNT(...) SITA_IDENTITY( SITA_VA_ARGS_COUNT_INTERNAL(__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1) )
#define SITA_VA_ARGS_COUNT_INTERNAL(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, COUNT, ...) COUNT

#define SITA_DUMP_VAR_1(v0)				do{ SITA_LOG("DUMP_VAR: {}=[{}]",								#v0, (v0)); } while(false)
#define SITA_DUMP_VAR_2(v0, v1)			do{ SITA_LOG("DUMP_VAR: {}=[{}], {}=[{}]",						#v0, (v0), #v1, (v1)); } while(false)
#define SITA_DUMP_VAR_3(v0, v1, v2)		do{ SITA_LOG("DUMP_VAR: {}=[{}], {}=[{}], {}=[{}]",				#v0, (v0), #v1, (v1), #v2, (v2)); } while(false)
#define SITA_DUMP_VAR_4(v0, v1, v2, v3)	do{ SITA_LOG("DUMP_VAR: {}=[{}], {}=[{}], {}=[{}], {}=[{}]",	#v0, (v0), #v1, (v1), #v2, (v2), #v3, (v3)); } while(false)

#define SITA_DUMP_VAR_SELECT(COUNT) SITA_DUMP_VAR_##COUNT
#define SITA_DUMP_VAR(...) SITA_IDENTITY(SITA_CALL(SITA_DUMP_VAR_SELECT, SITA_VA_ARGS_COUNT(__VA_ARGS__) (__VA_ARGS__)))

#define SITA_DUMP_HEX(v) \
	do{ \
		String tmp; \
		StringUtil::binToHex(tmp, v); \
		SITA_LOG("DUMP_HEX: {}\n{}", #v, tmp); \
	} while(false) \
//------

#define SITA_FORWARD(a)	::std::forward< decltype(a) >(a)
#define SITA_ASSERT(...)	assert(__VA_ARGS__)

#define SITA_SRC_LOC	SrcLoc(__FILE__, __LINE__, SITA_FUNC_NAME_SZ)

#define SITA_ENUM_BITWISE_OPERATOR(T) \
	SITA_INLINE T operator~ (T  a)      { return static_cast<T>(~enumInt(a)); } \
	SITA_INLINE T operator| (T  a, T b) { return static_cast<T>(enumInt(a) | enumInt(b)); } \
	SITA_INLINE T operator& (T  a, T b) { return static_cast<T>(enumInt(a) & enumInt(b)); } \
	SITA_INLINE T operator^ (T  a, T b) { return static_cast<T>(enumInt(a) ^ enumInt(b)); } \
	SITA_INLINE void operator|=(T& a, T b) { a = static_cast<T>(enumInt(a) | enumInt(b)); } \
	SITA_INLINE void operator&=(T& a, T b) { a = static_cast<T>(enumInt(a) & enumInt(b)); } \
	SITA_INLINE void operator^=(T& a, T b) { a = static_cast<T>(enumInt(a) ^ enumInt(b)); } \
//--------

#define SITA_ENUM_ARITHMETIC_OPERATOR(T) \
	SITA_INLINE T operator+ (T  a, T b) { return static_cast<T>(enumInt(a) + enumInt(b)); } \
	SITA_INLINE T operator- (T  a, T b) { return static_cast<T>(enumInt(a) - enumInt(b)); } \
	SITA_INLINE T operator* (T  a, T b) { return static_cast<T>(enumInt(a) * enumInt(b)); } \
	SITA_INLINE T operator/ (T  a, T b) { return static_cast<T>(enumInt(a) / enumInt(b)); } \
	SITA_INLINE void operator+=(T& a, T b) { a = static_cast<T>(enumInt(a) + enumInt(b)); } \
	SITA_INLINE void operator-=(T& a, T b) { a = static_cast<T>(enumInt(a) - enumInt(b)); } \
	SITA_INLINE void operator*=(T& a, T b) { a = static_cast<T>(enumInt(a) * enumInt(b)); } \
	SITA_INLINE void operator/=(T& a, T b) { a = static_cast<T>(enumInt(a) / enumInt(b)); } \
//--------

#define SITA_ENUM_ALL_OPERATOR(T) \
	SITA_ENUM_BITWISE_OPERATOR(T) \
	SITA_ENUM_ARITHMETIC_OPERATOR(T) \
//-------
