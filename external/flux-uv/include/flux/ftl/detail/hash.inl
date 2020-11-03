/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- hash()
 */
/*-***************************************************************** -}}}1- */

#include <flux/std/inttypes.hpp>
#include <flux/std/process_random.hpp>

#include <flux/compat/constexpr.hpp>

#include <cstring>
#include <functional>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(ftl)
//--    hash_combine()                      ///{{{2///////////////////////////
inline constexpr
Hash hash_combine( Hash aX, Hash aY ) noexcept
{
	// Based of Boost's hash_combine(), and http://stackoverflow.com/a/2595226
	// Also recommended in N3876, which includes a reference to a more in-depth
	// analysis. See
	// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3876.pdf
	return aX ^ (aY + Hash(0x9e3779b9) + (aY<<6) + (aY>>2));
}

//--    $ local                             ///{{{2///////////////////////////
namespace detail
{
	template<> struct Has_Std_Hash<bool> : std::true_type {};
	template<> struct Has_Std_Hash<char> : std::true_type {};
	template<> struct Has_Std_Hash<signed char> : std::true_type {};
	template<> struct Has_Std_Hash<unsigned char> : std::true_type {};
	template<> struct Has_Std_Hash<wchar_t> : std::true_type {};
	template<> struct Has_Std_Hash<short> : std::true_type {};
	template<> struct Has_Std_Hash<unsigned short> : std::true_type {};
	template<> struct Has_Std_Hash<int> : std::true_type {};
	template<> struct Has_Std_Hash<unsigned int> : std::true_type {};
	template<> struct Has_Std_Hash<long> : std::true_type {};
	template<> struct Has_Std_Hash<unsigned long> : std::true_type {};
	template<> struct Has_Std_Hash<long long> : std::true_type {};
	template<> struct Has_Std_Hash<unsigned long long> : std::true_type {};
	template<> struct Has_Std_Hash<float> : std::true_type {};
	template<> struct Has_Std_Hash<double> : std::true_type {};
	template<> struct Has_Std_Hash<long double> : std::true_type {};
	//template<> struct Has_Std_Hash<std::string> : std::true_type {};

	// pointers
	template< typename tType >
	struct Has_Std_Hash< tType* > : std::true_type {};
	template< typename tType >
	struct Has_Std_Hash< tType const* > : std::true_type {};

	// special: c-strings
	template<> struct Has_Std_Hash< char* > : std::false_type {};
	template<> struct Has_Std_Hash< char const* > : std::false_type {};

	// bit access
	template< typename tType > inline
	FLUX_ENABLE_IF_C( sizeof(tType) <= sizeof(Hash) )
	(Hash) hash_from_bits_( tType const& aValue ) noexcept
	{
		Hash ret = 0;
		std::memcpy( &ret, &aValue, sizeof(tType) );
		return ret;
	}
	
	template< typename tType > inline
	FLUX_DISABLE_IF_C( sizeof(tType) <= sizeof(Hash) )
	(Hash) hash_from_bits_( tType const& aValue ) noexcept
	{
		auto const* ptr = reinterpret_cast<std::uint8_t const*>(&aValue);

		Hash ret = 0;
		for( std::size_t i = 0; i < sizeof(tType); i += sizeof(Hash) )
		{
			Hash bits = 0; 
			std::memcpy( &bits, ptr+i, std::min(sizeof(Hash),sizeof(tType)-i) );
			ret = hash_combine( ret, bits );
		}
		return ret;
	}


#	if 0
	template< typename tType > inline
	FLUX_ENABLE_IF_C( sizeof(tType) > sizeof(Hash) && sizeof(tType) <= 2*sizeof(Hash) )
	(Hash) hash_from_bits_( tType const& aValue ) noexcept
	{
		/* XXX- Uh, this is probably terrible */
		Hash a = 0, b = 0;
		std::memcpy( &a, reinterpret_cast<char const*>(&aValue)+0, sizeof(Hash) );
		std::memcpy( &b, reinterpret_cast<char const*>(&aValue)+sizeof(Hash), sizeof(tType)-sizeof(Hash) );
		return a^b;
	}
#	endif

	// scramble bits
	// libstdc++ returns integers unmolested through std::hash<>, which is
	// not necessarily what we expect.
	// Based on http://stackoverflow.com/a/12996028
	inline FLUX_CONSTEXPR_EX
	Hash hash_scramble_( uint32_t aX ) noexcept
	{
		aX = ((aX >> 16) ^ aX) * uint32_t(0x45d9f3b);
		aX = ((aX >> 16) ^ aX) * uint32_t(0x45d9f3b);
		return (aX >> 16) ^ aX;
	}
	inline FLUX_CONSTEXPR_EX
	Hash hash_scramble_( uint64_t aX ) noexcept
	{
		aX = (aX ^ (aX >> 30)) * uint64_t(0xbf58476d1ce4e5b9);
		aX = (aX ^ (aX >> 27)) * uint64_t(0x94d049bb133111eb);
		return static_cast<Hash>(aX ^ (aX >> 31));
	}
}

//--    hash() - default                    ///{{{2///////////////////////////
template< typename tType > inline
FLUX_ENABLE_IF( detail::Has_Std_Hash< compat::decay_t<tType> > )
(Hash) hash( tType const& aValue ) noexcept
{
	return hash_combine( detail::hash_scramble_( detail::hash_from_bits_(aValue) ), kProcessRandom );
}

//--    hash() - strings                    ///{{{2///////////////////////////
inline
Hash hash( char const* aStr ) noexcept
{
	// See http://stackoverflow.com/a/629127
	if( !aStr ) 
		return hash( static_cast<void*>(nullptr) );


	Hash x = kProcessRandom;
    while( *aStr )
	{
        x = x * 101 + Hash(*aStr++);
	}

    return x;
}

inline
Hash hash( std::string const& aStr ) noexcept
{
	if( aStr.empty() )
		return hash( static_cast<void*>(nullptr) );

	return hash( aStr.c_str() );
}

//--    hash() - enum                       ///{{{2///////////////////////////
template< typename tType > inline
FLUX_ENABLE_IF( std::is_enum<tType> )
(Hash) hash( tType const& aEnum ) noexcept
{
	return hash( static_cast<compat::underlying_type_t<tType>>(aEnum) );
}

//--    <<< ~ flux::ftl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(ftl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
