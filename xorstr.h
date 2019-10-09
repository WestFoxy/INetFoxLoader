
#ifndef JMxorstr__HPP
#define JMxorstr__HPP

#include <immintrin.h>
#include <cstdint>
#include <cstddef>
#include <utility>
#define JMxorstr__DISABLE_AVX_INTRINSICS
#define xorstr(str)                                              \
    ::jm::makexorstr_(                                           \
        []() {                                                   \
            struct {                                             \
                const std::decay_t<decltype(*str)>* value = str; \
            } a;                                                 \
            return a;                                            \
        }(),                                                     \
        std::make_index_sequence<sizeof(str) / sizeof(*str)>{},  \
        std::make_index_sequence<::jm::detail::_buffer_size<sizeof(str)>()>{})
#define xorstr_(str) xorstr(str).crypt_get()

#ifdef _MSC_VER
#define XORSTR_FORCEINLINE __forceinline
#else
#define XORSTR_FORCEINLINE __attribute__((always_inline))
#endif


#if !defined(XORSTR_ALLOW_DATA)

#if defined(__clang__) || defined(__GNUC__)
#define XORSTR_VOLATILE volatile
#endif

#endif
#ifndef XORSTR_VOLATILE
#define XORSTR_VOLATILE
#endif

namespace jm {

	namespace detail {

		template<std::size_t S>
		struct unsigned_;

		template<>
		struct unsigned_<1> {
			using type = std::uint8_t;
		};
		template<>
		struct unsigned_<2> {
			using type = std::uint16_t;
		};
		template<>
		struct unsigned_<4> {
			using type = std::uint32_t;
		};

		template<auto C, auto...>
		struct pack_value_type {
			using type = decltype(C);
		};

		template<std::size_t Size>
		constexpr std::size_t _buffer_size()
		{
			return ((Size / 16) + (Size % 16 != 0)) * 2;
		}

		template<auto... Cs>
		struct tstring_ {
			using value_type = typename pack_value_type<Cs...>::type;
			constexpr static std::size_t size = sizeof...(Cs);
			constexpr static value_type  str[size] = { Cs... };

			constexpr static std::size_t buffer_size = _buffer_size<sizeof(str)>();
			constexpr static std::size_t buffer_align =
#ifndef JMxorstr__DISABLE_AVX_INTRINSICS
			((sizeof(str) > 16) ? 32 : 16);
#else
				16;
#endif
		};

		template<std::uint32_t Seed>
		constexpr std::uint32_t key4() noexcept
		{
			std::uint32_t value = Seed;
			for (char c : __TIME__)
				value = static_cast<std::uint32_t>((value ^ c) * 16777619ull);
			return value;
		}

		template<std::size_t S>
		constexpr std::uint64_t key8()
		{
			constexpr auto first_part = key4<2166136261 + S>();
			constexpr auto second_part = key4<first_part>();
			return (static_cast<std::uint64_t>(first_part) << 32) | second_part;
		}

		
		template<class T, std::uint64_t... Keys>
		struct string_storage {
			std::uint64_t storage[T::buffer_size];

			XORSTR_FORCEINLINE constexpr string_storage() noexcept : storage{ Keys... }
			{
				using cast_type =
					typename unsigned_<sizeof(typename T::value_type)>::type;
				constexpr auto value_size = sizeof(typename T::value_type);
				
				for (std::size_t i = 0; i < T::size; ++i)
					storage[i / (8 / value_size)] ^=
					(std::uint64_t{ static_cast<cast_type>(T::str[i]) }
				<< ((i % (8 / value_size)) * 8 * value_size));
			}
		};

	} 

	template<class T, std::uint64_t... Keys>
	class xor_string {
		alignas(T::buffer_align) XORSTR_VOLATILE std::uint64_t _storage[T::buffer_size];

		template<std::size_t N>
		XORSTR_FORCEINLINE void _crypt(XORSTR_VOLATILE std::uint64_t* keys) noexcept
		{
			if constexpr (T::buffer_size > N) {
#ifndef JMxorstr__DISABLE_AVX_INTRINSICS
				if constexpr ((T::buffer_size - N) >= 4) {
					_mm256_store_si256(
						(__m256i*)(_storage + N),
						_mm256_xor_si256(
							_mm256_load_si256((const __m256i*)(_storage + N)),
							_mm256_load_si256((const __m256i*)(keys + N))));
					_crypt<N + 4>(keys);
				}
				else
#endif
				{
					_mm_store_si128(
						(__m128i*)(_storage + N),
						_mm_xor_si128(_mm_load_si128((const __m128i*)(_storage + N)),
							_mm_load_si128((const __m128i*)(keys + N))));
					_crypt<N + 2>(keys);
				}
			}
		}

		XORSTR_FORCEINLINE constexpr void _copy_single(XORSTR_VOLATILE std::uint64_t& dst,
			std::uint64_t val) noexcept
		{
			dst = val;
		}

		
		template<std::size_t... Indices>
		XORSTR_FORCEINLINE constexpr void _copy(std::index_sequence<Indices...>) noexcept
		{
			constexpr detail::string_storage<T, Keys...> storage;
			(_copy_single(_storage[Indices], storage.storage[Indices]), ...);
		}

		template<std::size_t... Indices>
		XORSTR_FORCEINLINE constexpr void _copy_keys(
			XORSTR_VOLATILE std::uint64_t* keys, std::index_sequence<Indices...>) noexcept
		{
			(_copy_single(keys[Indices], Keys), ...);
		}

	public:
		using value_type = typename T::value_type;
		using size_type = std::size_t;
		using pointer = value_type * ;
		using const_pointer = const pointer;

		XORSTR_FORCEINLINE xor_string() noexcept
		{
			_copy(std::make_index_sequence<T::buffer_size>{});
		}

		XORSTR_FORCEINLINE constexpr size_type size() const noexcept
		{
			return T::size - 1;
		}

		XORSTR_FORCEINLINE void crypt() noexcept
		{
			alignas(T::buffer_align) XORSTR_VOLATILE std::uint64_t keys[T::buffer_size];
			_copy_keys(keys, std::make_index_sequence<sizeof...(Keys)>());
			_crypt<0>(keys);
		}

		XORSTR_FORCEINLINE const_pointer get() const noexcept
		{
			
			return (const_pointer)(_storage);
		}

		XORSTR_FORCEINLINE const_pointer crypt_get() noexcept
		{
			crypt();
			
			return (const_pointer)(_storage);
		}
	};

	template<class Tstr, std::size_t... StringIndices, std::size_t... KeyIndices>
	XORSTR_FORCEINLINE constexpr auto
		makexorstr_(Tstr,
			std::index_sequence<StringIndices...>,
			std::index_sequence<KeyIndices...>) noexcept
	{
		return xor_string<
			detail::tstring_<Tstr().value[StringIndices]...>,
			detail::key8<KeyIndices>()...>{};
	}

} 

#endif
