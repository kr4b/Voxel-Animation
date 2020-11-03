/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	system_error fallback
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    macros                               ///{{{2///////////////////////////
#undef FLUX_GLOBAL_NIH_MAKE_ERROR_CODE_ENUM
#define FLUX_GLOBAL_NIH_MAKE_ERROR_CODE_ENUM(e)                    \
	FLUX_NAMESPACE_ENTER1(nih_)                                    \
		template<> struct is_error_code_enum<e>                    \
			: std::true_type                                       \
		{};                                                        \
	FLUX_NAMESPACE_LEAVE1(nih_) /*ENDM*/

#undef FLUX_GLOBAL_NIH_MAKE_ERROR_CONDITION_ENUM
#define FLUX_GLOBAL_NIH_MAKE_ERROR_CONDITION_ENUM(e)               \
	FLUX_NAMESPACE_ENTER1(nih_)                                    \
		template<> struct is_error_condition_enum<e>               \
			: std::true_type                                       \
		{};                                                        \
	FLUX_NAMESPACE_LEAVE1(nih_) /*ENDM*/

//--    errc                                ///{{{2///////////////////////////
template<> 
struct is_error_condition_enum<errc>
	: std::true_type 
{};

//--    error_code                          ///{{{2///////////////////////////
template< typename tErrorEnum > inline
error_code::error_code( tErrorEnum aEnum, FLUX_ENABLE_IF(is_error_code_enum<tErrorEnum>)(void*) ) noexcept
	: mValue( static_cast<int>(aEnum) )
	, mCategory( &make_error_code(aEnum).category() )
{}

template< typename tErrorEnum > inline
FLUX_ENABLE_IF( is_error_code_enum<tErrorEnum> )
(error_code&) error_code::operator= (tErrorEnum aEnum) noexcept
{
	mValue = static_cast<int>(aEnum);
	mCategory = &make_error_code(aEnum).category();
	return *this;
}

//--    error_condition                     ///{{{2///////////////////////////
template< typename tErrorEnum > inline
error_condition::error_condition( tErrorEnum aEnum, FLUX_ENABLE_IF(is_error_condition_enum<tErrorEnum>)(void*) ) noexcept
	: mValue( static_cast<int>(aEnum) )
	, mCategory( &make_error_condition(aEnum).category() )
{}

template< typename tErrorEnum > inline
FLUX_ENABLE_IF( is_error_condition_enum<tErrorEnum> )
(error_condition&) error_condition::operator= (tErrorEnum aEnum) noexcept
{
	mValue = static_cast<int>(aEnum);
	mCategory = &make_error_condition(aEnum).category();
	return *this;
}

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
