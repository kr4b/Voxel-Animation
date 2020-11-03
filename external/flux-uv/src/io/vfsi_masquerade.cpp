/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Masking VINode
 */
/*-***************************************************************** -}}}1- */

#include <flux/io/vfsi/masquerade.hpp>

#include <flux/io/buffer.hpp>
#include <flux/io/stream.hpp>
#include <flux/io/errors.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/format.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::io::vfsi >>>          ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	template< class tPointerType >
	class Disguise_ final : public VINode
	{
		public:
			Disguise_(
				tPointerType,
				std::string aPathFromAncestor,
				weak_ptr<VINode>
			);

		public: /* virtuals: generic */
			std::string desc() const override;

			bool update( bool, compat::error_code& ) noexcept override;
			bool remove( bool, compat::error_code& ) noexcept override;

			optional<sys::SysPathStr> native_path( compat::error_code& ) const override;

		public: /* virtuals: file access */
			VfsSize file_size( compat::error_code& ) noexcept override;

			unique_ptr<Buffer> file_as_buffer( 
				EBufferMode,
				bool aMustExist,
				BufferSize aResizeTo,
				compat::error_code&
			) noexcept override;
			unique_ptr<Stream> file_as_stream(
				EStreamMode,
				compat::error_code&
			) noexcept override;

		public: /* virtuals: directory access */
			optional<VfsStat> dir_stat( compat::string_view const&, compat::error_code& ) noexcept override;
			shared_ptr<VINode> dir_open( compat::string_view const&, compat::error_code& ) noexcept override;

			unique_ptr<Buffer> dir_open_buffer( 
				compat::string_view const&,
				EBufferMode,
				bool,
				BufferSize,
				compat::error_code&
			) noexcept override;
			unique_ptr<Stream> dir_open_stream( 
				compat::string_view const&,
				EStreamMode,
				compat::error_code&
			) noexcept override;

			void dir_create( compat::string_view const&, bool ) override;
			bool dir_remove( compat::string_view const&, bool ) override;

		private:
			tPointerType mAnonymous;
	};

	shared_ptr<VINode> const& access_( shared_ptr<VINode> const& aPtr ) noexcept
	{
		return aPtr;
	}
	shared_ptr<VINode> access_( weak_ptr<VINode> const& aWPtr )
	{
		if( auto sp = aWPtr.lock() )
			return sp;

		FLUX_THROW( error::VfsOpNotSupported )
			<< flux::einfo::ErrorDesc( "Masqueraded node no longer exists" )
		;
	}

	shared_ptr<VINode> const& access_( shared_ptr<VINode> const& aPtr, compat::error_code& ) noexcept
	{
		return aPtr;
	}
	shared_ptr<VINode> access_( weak_ptr<VINode> const& aWPtr, compat::error_code& aErrorOut ) noexcept
	{
		if( auto sp = aWPtr.lock() )
			return sp;

		aErrorOut = compat::make_error_code( compat::errc::no_such_file_or_directory );
		return nullptr;
	}

}

//--    $ Disguise_                         ///{{{2///////////////////////////
namespace
{
#	define A_(x) access_(x)
	
	// [cd]tor -{{{3-
	template< class tPointerType >
	Disguise_<tPointerType>::Disguise_( tPointerType aAnon, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
		: VINode( std::move(aPathFromAncestor), std::move(aAncestor) )
		, mAnonymous( std::move(aAnon) )
	{
		mStat = A_(mAnonymous)->stat();
	}

	// virtuals: generic -{{{3-
	template< class tPointerType >
	std::string Disguise_<tPointerType>::desc() const
	{
		auto desc = A_(mAnonymous)->desc();
		if( 0 == std::strncmp( "masked-", desc.c_str(), 7 ) )
			return desc;

		return format( "masked-{}", desc );
	}

	template< class tPointerType >
	bool Disguise_<tPointerType>::update( bool aRecursive, compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
		if( auto const& sp = access_(mAnonymous, aErrorOut) )
		{
			bool ret = sp->update( aRecursive, aErrorOut );
			mStat = sp->stat();
			return ret;
		}

		return false;
	}
	template< class tPointerType >
	bool Disguise_<tPointerType>::remove( bool aRecursive, compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
		if( auto const& sp = access_(mAnonymous, aErrorOut) )
			return sp->remove( aRecursive, aErrorOut );

		return false;
	}

	template< class tPointerType >
	optional<sys::SysPathStr> Disguise_<tPointerType>::native_path( compat::error_code& aErrorOut ) const
	{
		if( auto const& sp = access_(mAnonymous, aErrorOut) )
			return sp->native_path( aErrorOut );
		
		return {};
	}

	// virtuals: file access -{{{3-
	template< class tPointerType >
	VfsSize Disguise_<tPointerType>::file_size( compat::error_code& aErrorOut ) noexcept
	{
		FLUX_ASSERT( !aErrorOut );
		if( auto const& sp = access_(mAnonymous, aErrorOut) )
			return sp->file_size( aErrorOut );

		return kInvalidVfsSize;
	}

	template< class tPointerType >
	unique_ptr<Buffer> Disguise_<tPointerType>::file_as_buffer( EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
	{
		return A_(mAnonymous)->file_as_buffer( aMode, aMustExist, aResizeTo, aErrorOut );
	}
	template< class tPointerType >
	unique_ptr<Stream> Disguise_<tPointerType>::file_as_stream( EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
	{
		return A_(mAnonymous)->file_as_stream( aMode, aErrorOut );
	}

	// virtuals: directory access -{{{3-
	template< class tPointerType >
	optional<VfsStat> Disguise_<tPointerType>::dir_stat( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_stat_defaults( aPath, false, nullptr, aErrorOut ) )
			return def.value();

		return A_(mAnonymous)->dir_stat( aPath, aErrorOut );
	}
	template< class tPointerType >
	shared_ptr<VINode> Disguise_<tPointerType>::dir_open( compat::string_view const& aPath, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_defaults( aPath, false, nullptr, aErrorOut ) )
			return def.value();

		return A_(mAnonymous)->dir_open( aPath, aErrorOut );
	}

	template< class tPointerType >
	unique_ptr<Buffer> Disguise_<tPointerType>::dir_open_buffer( compat::string_view const& aPath, EBufferMode aMode, bool aMustExist, BufferSize aResizeTo, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_buffer_defaults( aPath, aMode, aMustExist, aResizeTo, false, nullptr, aErrorOut ) )
			return std::move(def.value());

		return A_(mAnonymous)->dir_open_buffer( aPath, aMode, aMustExist, aResizeTo, aErrorOut );
	}
	template< class tPointerType >
	unique_ptr<Stream> Disguise_<tPointerType>::dir_open_stream( compat::string_view const& aPath, EStreamMode aMode, compat::error_code& aErrorOut ) noexcept
	{
		if( auto def = dir_open_stream_defaults( aPath, aMode, false, nullptr, aErrorOut ) )
			return std::move(def.value());

		return A_(mAnonymous)->dir_open_stream( aPath, aMode, aErrorOut );
	}

	template< class tPointerType >
	void Disguise_<tPointerType>::dir_create( compat::string_view const& aPath, bool aRecursive )
	{
		if( dir_create_defaults( aPath, aRecursive ) )
			return;

		return A_(mAnonymous)->dir_create( aPath, aRecursive );
	}
	template< class tPointerType >
	bool Disguise_<tPointerType>::dir_remove( compat::string_view const& aPath, bool aRecursive )
	{
		if( auto def = dir_remove_defaults( aPath, aRecursive ) )
			return def.value();

		return A_(mAnonymous)->dir_remove( aPath, aRecursive );
	}
#	undef A_
}


//--    inode_masquerade()                  ///{{{2///////////////////////////
shared_ptr<VINode> inode_masquerade( weak_ptr<VINode> aAnon, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
{
	return make_shared<Disguise_<weak_ptr<VINode>>>( std::move(aAnon), std::move(aPathFromAncestor), std::move(aAncestor) );
}
shared_ptr<VINode> inode_masquerade( shared_ptr<VINode> aAnon, std::string aPathFromAncestor, weak_ptr<VINode> aAncestor )
{
	return make_shared<Disguise_<shared_ptr<VINode>>>( std::move(aAnon), std::move(aPathFromAncestor), std::move(aAncestor) );
}

//--    <<< ~ flux::io::vfsi namespace <<<          ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
