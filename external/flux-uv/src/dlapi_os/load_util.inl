/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- load_util.hxx implementation
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::dlapi::os::detail >>> ///{{{1///////////////////
FLUX_NAMESPACE_ENTER3(dlapi,os,detail)
//--    ApiRecord                           ///{{{2///////////////////////////
template< class tApi > inline
ApiRecord<tApi>::~ApiRecord()
{
	if( mAutoloadAttempted )
	{
		try
		{
			release();
		}
		catch( ... )
		{}
	}
}

template< class tApi > inline
tApi const* ApiRecord<tApi>::api() const noexcept
{
	return mApi.load();
}

template< class tApi > inline
bool ApiRecord<tApi>::acquire( char const* aDlibName )
{	
	std::unique_lock<std::mutex> guard(mMutex);
	if( auto const* ptr = mApi.load() )
	{
		ex::acquire( ptr );
		return true;
	}

	if( auto const* ptr = ex::load( Identity<tApi>{}, aDlibName ) )
	{
		mApi.store( ptr );
		return true;
	}

	return false;
}
template< class tApi > inline
bool ApiRecord<tApi>::acquire( void* (*aGetFn)(char const*, void*), void* aUser )
{	
	std::unique_lock<std::mutex> guard(mMutex);
	if( auto const* ptr = mApi.load() )
	{
		ex::acquire( ptr );
		return true;
	}

	if( auto const* ptr = ex::load( Identity<tApi>{}, aGetFn, aUser ) )
	{
		mApi.store( ptr );
		return true;
	}

	return false;
}

template< class tApi > inline
void ApiRecord<tApi>::release()
{
	/* TODO: If a debugger/valgrind is attached, we maybe shouldn't unload
	 * the DLL, so that we get the symbol names and so on.
	 */
	
	std::unique_lock<std::mutex> guard(mMutex);
	if( auto const* ptr = mApi.load() )
	{
		bool const last = (1 == ptr->referenceCount_);
		ex::release( ptr );

		if( last )
			mApi.store( nullptr );
	}
}

template< class tApi > inline
tApi const* ApiRecord<tApi>::try_autoload() noexcept
{
	if( mAutoloadAttempted )
		return mApi.load();

	try
	{
		acquire();
	}
	catch( ... )
	{}

	mAutoloadAttempted = true;
	return mApi.load();
}

//--    FnPtrProxy                          ///{{{2///////////////////////////
inline 
FnPtrProxy::FnPtrProxy( void* aPtr ) noexcept
	: mPtr(aPtr)
{}

template< typename tType > inline
FnPtrProxy::operator tType*() const noexcept
{
	return reinterpret_cast<tType*>(mPtr);
}

//--    <<< ~ flux::dlapi::os::detail namespace <<< ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,os,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
