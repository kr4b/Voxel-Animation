/*-******************************************************* -- HEADER -{{{1- */
/*-	VFSinternal - union of directories
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_IO_VFSI_UNION_HPP_934F7EBC_A0D0_4402_AB76_F34D5B863E67
#define FLUX_IO_VFSI_UNION_HPP_934F7EBC_A0D0_4402_AB76_F34D5B863E67

//--//////////////////////////////////////////////////////////////////////////
//--    Include             ///{{{1///////////////////////////////////////////

#include <flux/setup.pkg>
#include <flux/ftl/vector.hpp>

#include "inode.hpp"

//--    >>> namespace = flux::io::vfsi >>>      ///{{{1///////////////////////
FLUX_NAMESPACE_ENTER2(io,vfsi)
//--    UnionDir                        ///{{{2///////////////////////////////
/** Union of several directories
 *
 * Creates an union of several directories. For example, given the following
 * three directories:
 *   - `A` containing `a` and `b`
 *   - `B` containing `c`
 *   - `C` containing `b` and `d`
 * a `UnionDir` constructed as follows
   \code
   auto none = flux::weak_ptr<vfsi::VINode>();
   auto ud = flux::make_shared<vfsi::UnionDir>( "", none );
   ud->add_back( A );
   ud->add_back( B );
   ud->add_front( C ); // highest "priority"
   \endcode
 * will return valid results items `a`, `b`, `c`, `d`. Furthermore, the `b`
 * returned will be `C`'s `b`.
 *
 * \note <b>Limitation:</b> If item `a` is another directory, the following
 * code may not find item `c` (variable `cd` below):
   \code
   auto ad = ud->dir_open( "a" );
   auto cd = ad->dir_open( "../c" );
   \endcode
 * since the parent of directory `a` does <b>not</b> resolve to the UnionDir.
 * This is a limitation of the current VFS design (although similar issues 
 * exist with symlinks in real file systems).
 */
class UnionDir final : public VINode
{
	public:
		UnionDir( std::string, weak_ptr<VINode> );

	public:
		void add_back( shared_ptr<VINode> );
		void add_front( shared_ptr<VINode> );

		void remove( shared_ptr<VINode> );

		std::size_t count() const noexcept;
		shared_ptr<VINode const> get( std::size_t ) const noexcept;

	public: /* virtuals: generic */
		std::string desc() const override;

		bool update( bool, compat::error_code& ) noexcept override;
		bool remove( bool, compat::error_code& ) noexcept override;

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
		ftl::Vector<shared_ptr<VINode>> mStack;
};

//--    <<< ~ flux::io::vfsi namespace <<<      ///{{{1///////////////////////
FLUX_NAMESPACE_LEAVE2(io,vfsi)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_IO_UNION_HPP_934F7EBC_A0D0_4402_AB76_F34D5B863E67
