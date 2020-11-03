/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- meta::List implementation
 */
/*-***************************************************************** -}}}1- */

#include <type_traits>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::ftl::meta >>>         ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(ftl,meta)
//--    list_size<>                         ///{{{2///////////////////////////
template< typename... tElems >
struct list_size< List<tElems...> > 
	: std::integral_constant< std::size_t, sizeof...(tElems) >
{};

//--    list_equal<>                        ///{{{2///////////////////////////
template< typename... tItemsA, typename... tItemsB >
struct list_equal< List<tItemsA...>, List<tItemsB...> >
	: std::false_type
{};

template< typename... tItems >
struct list_equal< List<tItems...>, List<tItems...> >
	: std::true_type
{};

//--    list_at<>                           ///{{{2///////////////////////////
namespace detail
{
	template< std::size_t, typename... > struct ListAt0;
	template< std::size_t tIndex, typename tHead, typename... tList > 
	struct ListAt0<tIndex,tHead,tList...>
	{
		using type = typename ListAt0<tIndex-1,tList...>::type;
	};
	template< typename tHead, typename... tList >
	struct ListAt0<0,tHead,tList...>
	{
		using type = tHead;
	};

	template< std::size_t tIndex >
	struct ListAt0<tIndex>
	{};
}

template< typename... tElems, std::size_t tIndex >
struct list_at< List<tElems...>, tIndex >
	: detail::ListAt0<tIndex, tElems...>
{};

//--    list_head<>                         ///{{{2///////////////////////////
namespace detail
{
	template< typename... > struct ListHead0;
	template< typename tHead, typename... tTail >
	struct ListHead0< tHead, tTail... >
	{
		using type = tHead;
	};
}

template< typename... tElems >
struct list_head< List<tElems...> >
	: detail::ListHead0<tElems...>
{};

//--    list_tail<>                         ///{{{2///////////////////////////
namespace detail
{
	template< typename... > struct ListTail0;
	template< typename tHead, typename... tTail >
	struct ListTail0< tHead, tTail... >
	{
		using type = List<tTail...>;
	};
}

template< typename... tElems >
struct list_tail< List<tElems...> >
	: detail::ListTail0<tElems...>
{};

//--    list_take<>                         ///{{{2///////////////////////////
namespace detail
{
	template< std::size_t tN, class, class > struct ListTake0;
	template< std::size_t tN, typename... tItems, class tList >
	struct ListTake0< tN, List<tItems...>, tList >
		: ListTake0< tN-1, List<tItems..., list_head_t<tList>>, list_tail_t<tList> >
	{};

	template< typename... tItems, class tList >
	struct ListTake0< 0, List<tItems...>, tList >
	{
		using type = List<tItems...>;
	};
}

template< class tList, std::size_t tN >
struct list_take
	: detail::ListTake0< tN, List<>, tList >
{};
template< class tList >
struct list_take< tList, 0 >
{
	using type = List<>;
};

//--    list_drop<>                         ///{{{2///////////////////////////
namespace detail
{
	template< std::size_t tN, class tList >
	struct ListDrop0
		: ListDrop0< tN-1, list_tail_t<tList> >
	{};
	template< class tList >
	struct ListDrop0< 0, tList >
	{
		using type = tList;
	};
}

template< class tList, std::size_t tN >
struct list_drop
	: detail::ListDrop0< tN, tList >
{};
template< class tList >
struct list_drop< tList, 0 >
{
	using type = tList;
};

//--    list_contains<>                     ///{{{2///////////////////////////
namespace detail
{
	template< typename, typename... > struct ListContains0;
	template< typename tTarget, typename tHead, typename... tTail >
	struct ListContains0<tTarget,tHead,tTail...>
		: ListContains0<tTarget,tTail...>
	{};

	template< typename tTarget, typename... tTail >
	struct ListContains0<tTarget,tTarget,tTail...>
		: std::true_type
	{};

	template< typename tTarget >
	struct ListContains0<tTarget>
		: std::false_type
	{};
}

template< class tList, typename tTarget, std::size_t tBegin >
struct list_contains
	: list_contains< list_drop_t<tList,tBegin>, tTarget, 0 >
{};
template< typename... tItems, typename tTarget >
struct list_contains< List<tItems...>, tTarget, 0 >
	: detail::ListContains0< tTarget, tItems... >
{};

//--    list_index_of<>                     ///{{{2///////////////////////////
namespace detail
{
	template< typename, std::size_t tIdx, typename... > struct ListIndexOf0;
	template< typename tTarget, std::size_t tIdx, typename tHead, typename... tTail >
	struct ListIndexOf0<tTarget,tIdx,tHead,tTail...>
		: ListIndexOf0<tTarget,tIdx+1,tTail...>
	{};

	template< typename tTarget, std::size_t tIdx, typename... tTail >
	struct ListIndexOf0<tTarget,tIdx,tTarget,tTail...>
		: std::integral_constant< std::size_t, tIdx >
	{};
}

template< class tList, typename tTarget, std::size_t tBegin >
struct list_index_of
	: std::integral_constant< std::size_t, tBegin + list_index_of< list_drop_t<tList,tBegin>, tTarget, 0 >::value >
{};
template< typename... tItems, typename tTarget >
struct list_index_of< List<tItems...>, tTarget, 0 >
	: detail::ListIndexOf0< tTarget, 0, tItems... >
{};

//--    list_flatten<>                      ///{{{2///////////////////////////
namespace detail
{
	template< class, class... > struct ListFlatten0;
	template< typename... tResults, typename... tHeadItems, class... tTail >
	struct ListFlatten0< List<tResults...>, List<tHeadItems...>, tTail... >
		: ListFlatten0< List<tResults..., tHeadItems...>, tTail... >
	{};

	template< class tResult >
	struct ListFlatten0< tResult >
	{
		using type = tResult;
	};
}

template< class... tLists >
struct list_flatten
	: detail::ListFlatten0< List<>, tLists... >
{};

//--    list_filter<>                       ///{{{2///////////////////////////
namespace detail
{
	template< class tFunct, typename... tOptional >
	struct ListFilterEval
	{
		template< typename tItem >
		struct Eval
			: tFunct::template Eval<tItem, tOptional...>
		{};
	};

	template< class, class, typename... > struct ListFilter0;
	template< bool, class tFunct, class tResult, typename, typename... tTail >
	struct ListFilter1
		: ListFilter0< tFunct, tResult, tTail... >
	{};
	template< class tFunct, typename... tResults, typename tHead, typename... tTail >
	struct ListFilter1< true, tFunct, List<tResults...>, tHead, tTail... >
		: ListFilter0< tFunct, List<tResults..., tHead>, tTail... >
	{};

	template< class tFunct, class tResult, typename tHead, typename... tTail >
	struct ListFilter0< tFunct, tResult, tHead, tTail... >
		: ListFilter1< 
			tFunct::template Eval<tHead>::value,
			tFunct, 
			tResult,
			tHead,
			tTail...
		>
	{};
	template< class tFunct, class tResult >
	struct ListFilter0< tFunct, tResult >
	{
		using type = tResult;
	};
}

template< typename... tItems, class tFunct, typename... tOptional >
struct list_filter< List<tItems...>, tFunct, tOptional... >
	: detail::ListFilter0< detail::ListFilterEval<tFunct, tOptional...>, List<>, tItems... >
{};

//--    list_filter0<>                      ///{{{2///////////////////////////
namespace detail
{
	template< template<typename...> class tInner >
	struct ListFilterWrap
	{
		template< typename... tStuff >
		struct Eval : tInner<tStuff...>
		{};
	};
}

template< class tList, template<typename...> class tEval, typename... tOptional >
struct list_filter0
	: list_filter< tList, detail::ListFilterWrap<tEval>, tOptional... >
{};

//--    list_transform<>                    ///{{{2///////////////////////////
namespace detail
{
template< class tFunct, typename... tOptional >
	struct ListTransformEval
	{
		template< typename tItem >
		struct Eval
			: tFunct::template Eval<tItem, tOptional...>
		{};
	};

	template< class, class, typename... > struct ListTransform0;
	template< class tFunct, typename... tResults, typename tHead, typename... tTail >
	struct ListTransform0< tFunct, List<tResults...>, tHead, tTail... >
		: ListTransform0< 
			tFunct,
			List< tResults..., typename tFunct::template Eval<tHead>::type >,
			tTail...
		>
	{};

	template< class tFunct, class tResult >
	struct ListTransform0< tFunct, tResult >
	{
		using type = tResult;
	};
}

template< typename... tItems, class tFunct, typename... tOptional >
struct list_transform< List<tItems...>, tFunct, tOptional... >
	: detail::ListTransform0< detail::ListTransformEval<tFunct, tOptional...>, List<>, tItems... >
{};

//--    list_transform0<>                   ///{{{2///////////////////////////
namespace detail
{
	template< template<typename...> class tInner >
	struct ListTransformWrap
	{
		template< typename... tStuff >
		struct Eval : tInner<tStuff...>
		{};
	};
}

template< class tList, template<typename...> class tEval, typename... tOptional >
struct list_transform0
	: list_transform< tList, detail::ListTransformWrap<tEval>, tOptional... >
{};

//--    list_enact_for_each<>               ///{{{2///////////////////////////
namespace detail
{
	template< class, typename... > struct ListEnact0;
	template< class tFun, typename tHead, typename... tTail >
	struct ListEnact0< tFun, tHead, tTail... >
	{
		template< typename... tArgs >
		static void enact( tArgs&&... aArgs )
		{
			tFun::enact( List<tHead>{}, aArgs... );
			ListEnact0<tFun, tTail...>::template enact( std::forward<tArgs>(aArgs)... );
		}
	};

	template< class tFun >
	struct ListEnact0< tFun >
	{
		template< typename... tArgs >
		static void enact( tArgs&&... )
		{}
	};
}

template< typename... tItems, class tFun >
struct list_enact_for_each< List<tItems...>, tFun >
{
	template< typename... tArgs >
	static void enact( tArgs&&... aArgs )
	{
		detail::ListEnact0< tFun, tItems... >::enact( std::forward<tArgs>(aArgs)... );
	}
};

//--    list_to_tuple<>                     ///{{{2///////////////////////////
template< typename... tItems >
struct list_to_tuple< List<tItems...> >
{
	using type = std::tuple<tItems...>;
};

//--    <<< ~ flux::ftl::meta namespace <<<         ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(ftl,meta)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
