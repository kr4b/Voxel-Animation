/*-******************************************************* -- HEADER -{{{1- */
/*-	variant & co
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_COMPAT_VARIANT_HPP_484C3587_3AB0_436B_B2AD_AB0F63BBEC4C
#define FLUX_COMPAT_VARIANT_HPP_484C3587_3AB0_436B_B2AD_AB0F63BBEC4C

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup/namespace.hpp>
#include <flux/setup/stdfeat.hpp>

//--    variant<>                   ///{{{1///////////////////////////////////

#if FLUX_STDFEAT_CXX17_VARIANT
#	include <variant>

FLUX_NAMESPACE_ENTER1(compat)
    using std::variant;
    using std::monostate;
    using std::bad_variant_access;
    using std::variant_size;
    //using std::variant_size_v; //excluded for compat
    using std::variant_alternative;
    using std::variant_alternative_t;

    using std::in_place;
    using std::in_place_type;
    using std::in_place_index;
    using std::in_place_t;
    //using std::in_place_type_t; //XXX-not in variant-lite
    //using std::in_place_index_t; //XXX-not in variant-lite

    using std::visit;
    using std::holds_alternative;
    using std::get;
    using std::get_if;

    using std::variant_npos;
FLUX_NAMESPACE_LEAVE1(compat)

#else // !CXX17_VARIANT
#	include <flux_ext/variant.hpp>

FLUX_NAMESPACE_ENTER1(compat)
    using ::nonstd::variant;
    using ::nonstd::monostate;
    using ::nonstd::bad_variant_access;
    using ::nonstd::variant_size;
    //using ::nonstd::variant_size_v; // C++17 only,
    using ::nonstd::variant_alternative;
    using ::nonstd::variant_alternative_t;

    using ::nonstd::in_place;
    using ::nonstd::in_place_type;
    using ::nonstd::in_place_index;
    using ::nonstd::in_place_t;
    //using ::nonstd::in_place_type_t; //XXX?
    //using ::nonstd::in_place_index_t; //XXX?

    using ::nonstd::visit;
    using ::nonstd::holds_alternative;
    using ::nonstd::get;
    using ::nonstd::get_if;

    using ::nonstd::variant_npos;
FLUX_NAMESPACE_LEAVE1(compat)
#endif // ~ CXX17_VARIANT

//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_COMPAT_VARIANT_HPP_484C3587_3AB0_436B_B2AD_AB0F63BBEC4C
