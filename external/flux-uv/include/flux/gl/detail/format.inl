/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- OpenGL Format implementation
 *
 *
 * TODO: we can probably support the SFloat/UFloat from integer formats by
 * setting them to e.g. UInt/UFloat => R8, RED_INTEGER, UBYTE
 */
/*-***************************************************************** -}}}1- */

#include <flux/compat/constexpr.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    $ detail                            ///{{{2///////////////////////////
namespace detail
{
	template< class tPixel, class tMatchFn > FLUX_CONSTEXPR_EX inline
	void internal_format( tPixel const& aPixel, tMatchFn&& aMatch )
	{
		using namespace flux::meta;

		using R_ = SList<Red>;
		using RG_ = SList<Red,Green>;
		using RGB_ = SList<Red,Green,Blue>;
		using RGBA_ = SList<Red,Green,Blue,Alpha>;

		using BGR_ = SList<Blue,Green,Red>;
		using BGRA_ = SList<Blue,Green,Red,Alpha>;

		//-/
		//-/ Uniform pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, UniformPixel<__VA_ARGS__>{} ) ) {        \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		// Single channel generic
		FLUXTMP_GLF_MATCH_( R8UI,                   UInt8, UInt, R_ );
		FLUXTMP_GLF_MATCH_( R8,                     UInt8, UUnit, R_ );
		FLUXTMP_GLF_MATCH_( R8I,                    SInt8, SInt, R_ );
		FLUXTMP_GLF_MATCH_( R8_SNORM,               SInt8, SUnit, R_ );

		FLUXTMP_GLF_MATCH_( R16UI,                  UInt16, UInt, R_ );
		FLUXTMP_GLF_MATCH_( R16,                    UInt16, UUnit, R_ );
		FLUXTMP_GLF_MATCH_( R16I,                   SInt16, SInt, R_ );
		FLUXTMP_GLF_MATCH_( R16_SNORM,              SInt16, SUnit, R_ );
		FLUXTMP_GLF_MATCH_( R16F,                   Half, SFloat, R_ );

		FLUXTMP_GLF_MATCH_( R32UI,                  UInt32, UInt, R_ );
		FLUXTMP_GLF_MATCH_( R32I,                   SInt32, SInt, R_ );
		FLUXTMP_GLF_MATCH_( R32F,                   Float, SFloat, R_ );

		// Two channel generic
		FLUXTMP_GLF_MATCH_( RG8UI,                  UInt8, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG8,                    UInt8, UUnit, RG_ );
		FLUXTMP_GLF_MATCH_( RG8I,                   SInt8, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG8_SNORM,              SInt8, SUnit, RG_ );

		FLUXTMP_GLF_MATCH_( RG16UI,                 UInt16, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG16,                   UInt16, UUnit, RG_ );
		FLUXTMP_GLF_MATCH_( RG16I,                  SInt16, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG16_SNORM,             SInt16, SUnit, RG_ );
		FLUXTMP_GLF_MATCH_( RG16F,                  Half, SFloat, RG_ );

		FLUXTMP_GLF_MATCH_( RG32UI,                 UInt32, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG32I,                  SInt32, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG32F,                  Float, SFloat, RG_ );

		// Three channel generic
		FLUXTMP_GLF_MATCH_( RGB8UI,                 UInt8, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB8,                   UInt8, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB8I,                  SInt8, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB8_SNORM,             SInt8, SUnit, RGB_ );

		// Note: BGR textures have the same internal format as RGB ones. It's
		// just the external format that changes.
		FLUXTMP_GLF_MATCH_( RGB8UI,                 UInt8, UInt, BGR_ );
		FLUXTMP_GLF_MATCH_( RGB8,                   UInt8, UUnit, BGR_ );
		FLUXTMP_GLF_MATCH_( RGB8I,                  SInt8, SInt, BGR_ );
		FLUXTMP_GLF_MATCH_( RGB8_SNORM,             SInt8, SUnit, BGR_ );

		FLUXTMP_GLF_MATCH_( RGB16UI,                UInt16, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB16,                  UInt16, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB16I,                 SInt16, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB16_SNORM,            SInt16, SUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB16F,                 Half, SFloat, RGB_ );

		FLUXTMP_GLF_MATCH_( RGB32UI,                UInt32, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB32I,                 SInt32, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB32F,                 Float, SFloat, RGB_ );

		// Four channel generic
		FLUXTMP_GLF_MATCH_( RGBA8UI,                UInt8, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA8,                  UInt8, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA8I,                 SInt8, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA8_SNORM,            SInt8, SUnit, RGBA_ );

		// Note: BGRA textures have the same internal format as the RGB ones.
		// Only the external format changes.
		FLUXTMP_GLF_MATCH_( RGBA8UI,                UInt8, UInt, BGRA_ );
		FLUXTMP_GLF_MATCH_( RGBA8,                  UInt8, UUnit, BGRA_ );
		FLUXTMP_GLF_MATCH_( RGBA8I,                 SInt8, SInt, BGRA_ );
		FLUXTMP_GLF_MATCH_( RGBA8_SNORM,            SInt8, SUnit, BGRA_ );

		FLUXTMP_GLF_MATCH_( RGBA16UI,               UInt16, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA16,                 UInt16, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA16I,                SInt16, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA16_SNORM,           SInt16, SUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA16F,                Half, SFloat, RGBA_ );

		FLUXTMP_GLF_MATCH_( RGBA32UI,               UInt32, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA32I,                SInt32, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA32F,                Float, SFloat, RGBA_ );

		// Stencil only formats
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX8,         UInt8, UInt, SList<Stencil> );
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX16,        UInt16, UInt, SList<Stencil> );

		// Depth only formats
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT16,      UInt16, UUnit, SList<Depth>, DepthSpace );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT24,      UInt24, UUnit, SList<Depth>, DepthSpace );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT32,      UInt32, UUnit, SList<Depth>, DepthSpace );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT32F,     Float, SFloat, SList<Depth>, DepthSpace );

		// SRGB formats
		// See notes regarding BGR vs RGB above
		FLUXTMP_GLF_MATCH_( SRGB8,                  UInt8, UUnit, RGB_, SRGBSpace );
		FLUXTMP_GLF_MATCH_( SRGB8_ALPHA8,           UInt8, UUnit, RGBA_, SRGBSpace, AlphaSpace );

		FLUXTMP_GLF_MATCH_( SRGB8,                  UInt8, UUnit, BGR_, SRGBSpace );
		FLUXTMP_GLF_MATCH_( SRGB8_ALPHA8,           UInt8, UUnit, BGRA_, SRGBSpace, AlphaSpace );
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Packed pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, PackedPixel<__VA_ARGS__>{} ) ) {         \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		// Low bit stencils
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX1,            IList<1>, UInt, SList<Stencil> );
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX4,            IList<4>, UInt, SList<Stencil> );

		// 8-bit formats
		FLUXTMP_GLF_MATCH_( R3_G3_B2,                   IList<3,3,2>, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGBA2,                      IList<2,2,2,2>, UUnit, RGBA_ );

		// Weirdo formats
		FLUXTMP_GLF_MATCH_( RGB4,                       IList<4,4,4>, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB5,                       IList<5,5,5>, UUnit, RGB_ );

		// 16-bit formats
		FLUXTMP_GLF_MATCH_( RGB565,                     IList<5,6,5>, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGBA4,                      IList<4,4,4,4>, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA4,                      IList<4,4,4,4>, UUnit, BGRA_ ); //XXX maybe?
		FLUXTMP_GLF_MATCH_( RGB5_A1,                    IList<5,5,5,1>, UUnit, RGBA_ );

		// More weirdo formats
		FLUXTMP_GLF_MATCH_( RGB10,                      IList<10,10,10>, UUnit, RGB_ );

		// 32-bit formats
		FLUXTMP_GLF_MATCH_( RGB10_A2,                   IList<10,10,10,2>, UUnit, RGBA_, RGBSpace, AlphaSpace );
		FLUXTMP_GLF_MATCH_( RGB10_A2UI,                 IList<10,10,10,2>, UInt, RGBA_, RGBSpace, AlphaSpace );

		FLUXTMP_GLF_MATCH_( RGB10_A2,                   IList<10,10,10,2>, UUnit, BGRA_, RGBSpace, AlphaSpace );
		FLUXTMP_GLF_MATCH_( RGB10_A2UI,                 IList<10,10,10,2>, UInt, BGRA_, RGBSpace, AlphaSpace );

		// Missing: R11F_G11F_B10F
		// Missing: RGB9_E5

		// More more weirdo formats
		FLUXTMP_GLF_MATCH_( RGB12,                      IList<12,12,12>, UUnit, RGB_ );
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Generic pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel.components, __VA_ARGS__ ) ) {             \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		FLUXTMP_GLF_MATCH_( RGB5_A1, SPixelComponents<
			Component< SBits<1>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<5>, UUnit, Blue, RGBSpace >,
			Component< SBits<5>, UUnit, Green, RGBSpace >,
			Component< SBits<5>, UUnit, Red, RGBSpace >
		>{} );


		FLUXTMP_GLF_MATCH_( RGBA8, SPixelComponents<
			Component< SBits<8>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<8>, UUnit, Blue, RGBSpace >,
			Component< SBits<8>, UUnit, Green, RGBSpace >,
			Component< SBits<8>, UUnit, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( RGBA8UI, SPixelComponents<
			Component< SBits<8>, UInt, Alpha, AlphaSpace >,
			Component< SBits<8>, UInt, Blue, RGBSpace >,
			Component< SBits<8>, UInt, Green, RGBSpace >,
			Component< SBits<8>, UInt, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( RGBA8I, SPixelComponents<
			Component< SBits<8>, SInt, Alpha, AlphaSpace >,
			Component< SBits<8>, SInt, Blue, RGBSpace >,
			Component< SBits<8>, SInt, Green, RGBSpace >,
			Component< SBits<8>, SInt, Red, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( SRGB8_ALPHA8, SPixelComponents<
			Component< SBits<8>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<8>, UUnit, Blue, SRGBSpace >,
			Component< SBits<8>, UUnit, Green, SRGBSpace >,
			Component< SBits<8>, UUnit, Red, SRGBSpace >
		>{} );


		FLUXTMP_GLF_MATCH_( RGB10_A2, SPixelComponents<
			Component< SBits<2>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<10>, UUnit, Red, RGBSpace >,
			Component< SBits<10>, UUnit, Green, RGBSpace >,
			Component< SBits<10>, UUnit, Blue, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( RGB10_A2UI, SPixelComponents<
			Component< SBits<2>, UInt, Alpha, AlphaSpace >,
			Component< SBits<10>, UInt, Red, RGBSpace >,
			Component< SBits<10>, UInt, Green, RGBSpace >,
			Component< SBits<10>, UInt, Blue, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( RGB10_A2, SPixelComponents<
			Component< SBits<2>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<10>, UUnit, Blue, RGBSpace >,
			Component< SBits<10>, UUnit, Green, RGBSpace >,
			Component< SBits<10>, UUnit, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( RGB10_A2, SPixelComponents<
			Component< SBits<2>, UInt, Alpha, AlphaSpace >,
			Component< SBits<10>, UInt, Blue, RGBSpace >,
			Component< SBits<10>, UInt, Green, RGBSpace >,
			Component< SBits<10>, UInt, Red, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( DEPTH24_STENCIL8, SPixelComponents<
			Component< UInt24, UUnit, Depth, DepthSpace >,
			Component< UInt8, UInt, Stencil, StencilSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( DEPTH32F_STENCIL8, SPixelComponents<
			Component< Float, SFloat, Depth, DepthSpace >,
			Component< UInt8, UInt, Stencil, StencilSpace >
		>{} );
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Special "don't care about the details" formats
		////TODO Good idea?
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, PackedPixel<__VA_ARGS__>{} ) ) {         \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		FLUXTMP_GLF_MATCH_( STENCIL_INDEX8, IList<8>, AnyDomain, SList<Stencil> );

		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT16, IList<16>, AnyDomain, SList<Depth> );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT32, IList<32>, AnyDomain, SList<Depth> );

		FLUXTMP_GLF_MATCH_( DEPTH24_STENCIL8, IList<24,8>, AnyDomain, SList<Depth,Stencil> );
		FLUXTMP_GLF_MATCH_( DEPTH32F_STENCIL8, IList<32,8>, AnyDomain, SList<Depth,Stencil> );

#		undef FLUXTMP_GLF_MATCH_
	}

	template< class tPixel, class tMatchFn > FLUX_CONSTEXPR_EX inline
	void pixel_format( tPixel const& aPixel, tMatchFn&& aMatch )
	{
		using namespace flux::meta;

		using R_ = SList<Red>;
		using RG_ = SList<Red,Green>;
		using RGB_ = SList<Red,Green,Blue>;
		using RGBA_ = SList<Red,Green,Blue,Alpha>;

		using BGR_ = SList<Blue,Green,Red>;
		using BGRA_ = SList<Blue,Green,Red,Alpha>;

		//-/
		//-/ Uniform pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, UniformPixel<__VA_ARGS__>{} ) ) {        \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		// Single channel generic
		FLUXTMP_GLF_MATCH_( RED_INTEGER,            UInt8, UInt, R_ );
		FLUXTMP_GLF_MATCH_( RED,                    UInt8, UUnit, R_ );
		FLUXTMP_GLF_MATCH_( RED_INTEGER,            SInt8, SInt, R_ );
		FLUXTMP_GLF_MATCH_( RED,                    SInt8, SUnit, R_ );

		FLUXTMP_GLF_MATCH_( RED_INTEGER,            UInt16, UInt, R_ );
		FLUXTMP_GLF_MATCH_( RED,                    UInt16, UUnit, R_ );
		FLUXTMP_GLF_MATCH_( RED_INTEGER,            SInt16, SInt, R_ );
		FLUXTMP_GLF_MATCH_( RED,                    SInt16, SUnit, R_ );
		FLUXTMP_GLF_MATCH_( RED,                    Half, SFloat, R_ );

		FLUXTMP_GLF_MATCH_( RED_INTEGER,            UInt32, UInt, R_ );
		FLUXTMP_GLF_MATCH_( RED_INTEGER,            SInt32, SInt, R_ );
		FLUXTMP_GLF_MATCH_( RED,                    Float, SFloat, R_ );

		// Two channel generic
		FLUXTMP_GLF_MATCH_( RG_INTEGER,             UInt8, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG,                     UInt8, UUnit, RG_ );
		FLUXTMP_GLF_MATCH_( RG_INTEGER,             SInt8, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG,                     SInt8, SUnit, RG_ );

		FLUXTMP_GLF_MATCH_( RG_INTEGER,             UInt16, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG,                     UInt16, UUnit, RG_ );
		FLUXTMP_GLF_MATCH_( RG_INTEGER,             SInt16, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG,                     SInt16, SUnit, RG_ );
		FLUXTMP_GLF_MATCH_( RG,                     Half, SFloat, RG_ );

		FLUXTMP_GLF_MATCH_( RG32UI,                 UInt32, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG32I,                  SInt32, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( RG32F,                  Float, SFloat, RG_ );

		// Three channel generic
		FLUXTMP_GLF_MATCH_( RGB_INTEGER,            UInt8, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                    UInt8, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB_INTEGER,            SInt8, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                    SInt8, SUnit, RGB_ );

		// Note: BGR textures have the same internal format as RGB ones. It's
		// just the external format that changes.
		FLUXTMP_GLF_MATCH_( BGR_INTEGER,            UInt8, UInt, BGR_ );
		FLUXTMP_GLF_MATCH_( BGR,                    UInt8, UUnit, BGR_ );
		FLUXTMP_GLF_MATCH_( BGR_INTEGER,            SInt8, SInt, BGR_ );
		FLUXTMP_GLF_MATCH_( BGR,                    SInt8, SUnit, BGR_ );

		FLUXTMP_GLF_MATCH_( RGB_INTEGER,            UInt16, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                    UInt16, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB_INTEGER,            SInt16, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                    SInt16, SUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                    Half, SFloat, RGB_ );

		FLUXTMP_GLF_MATCH_( RGB_INTEGER,            UInt32, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB_INTEGER,            SInt32, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                    Float, SFloat, RGB_ );

		// Four channel generic
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER,           UInt8, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA,                   UInt8, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER,           SInt8, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA,                   SInt8, SUnit, RGBA_ );

		// Note: BGRA textures have the same internal format as the RGB ones.
		// Only the external format changes.
		FLUXTMP_GLF_MATCH_( BGRA_INTEGER,           UInt8, UInt, BGRA_ );
		FLUXTMP_GLF_MATCH_( BGRA,                   UInt8, UUnit, BGRA_ );
		FLUXTMP_GLF_MATCH_( BGRA_INTEGER,           SInt8, SInt, BGRA_ );
		FLUXTMP_GLF_MATCH_( BGRA,                   SInt8, SUnit, BGRA_ );

		FLUXTMP_GLF_MATCH_( RGBA_INTEGER,           UInt16, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA,                   UInt16, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER,           SInt16, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA,                   SInt16, SUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA,                   Half, SFloat, RGBA_ );

		FLUXTMP_GLF_MATCH_( RGBA_INTEGER,           UInt32, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER,           SInt32, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA,                   Float, SFloat, RGBA_ );

		// Stencil only formats
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX,          UInt8, UInt, SList<Stencil> );
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX,          UInt16, UInt, SList<Stencil> );

		// Depth only formats
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT,        UInt16, UUnit, SList<Depth>, DepthSpace );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT,        UInt24, UUnit, SList<Depth>, DepthSpace );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT,        UInt32, UUnit, SList<Depth>, DepthSpace );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT,        Float, SFloat, SList<Depth>, DepthSpace );

		// SRGB formats
		// See notes regarding BGR vs RGB above
		FLUXTMP_GLF_MATCH_( RGB,                    UInt8, UUnit, RGB_, SRGBSpace );
		FLUXTMP_GLF_MATCH_( RGBA,                   UInt8, UUnit, RGBA_, SRGBSpace, AlphaSpace );

		FLUXTMP_GLF_MATCH_( BGR,                    UInt8, UUnit, BGR_, SRGBSpace );
		FLUXTMP_GLF_MATCH_( BGRA,                   UInt8, UUnit, BGRA_, SRGBSpace, AlphaSpace );
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Packed pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, PackedPixel<__VA_ARGS__>{} ) ) {         \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		// Low bit stencils
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX,         IList<1>, UInt, SList<Stencil> );
		FLUXTMP_GLF_MATCH_( STENCIL_INDEX,         IList<4>, UInt, SList<Stencil> );

		// 8-bit formats
		FLUXTMP_GLF_MATCH_( RGB,                   IList<3,3,2>, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGBA,                  IList<2,2,2,2>, UUnit, RGBA_ );

		// Weirdo formats
		FLUXTMP_GLF_MATCH_( RGB,                   IList<4,4,4>, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                   IList<5,5,5>, UUnit, RGB_ );

		// 16-bit formats
		FLUXTMP_GLF_MATCH_( RGB,                   IList<5,6,5>, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( RGB,                   IList<5,6,5>, UUnit, BGR_ ); //_REV below
		FLUXTMP_GLF_MATCH_( RGBA,                  IList<4,4,4,4>, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( RGBA,                  IList<4,4,4,4>, UUnit, BGRA_ ); //_REV
		FLUXTMP_GLF_MATCH_( RGBA,                  IList<5,5,5,1>, UUnit, RGBA_ );

		// More weirdo formats
		FLUXTMP_GLF_MATCH_( RGB,                   IList<10,10,10>, UUnit, RGB_ );

		// 32-bit formats
		FLUXTMP_GLF_MATCH_( RGBA,                  IList<10,10,10,2>, UUnit, RGBA_, RGBSpace, AlphaSpace );
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER,          IList<10,10,10,2>, UInt, RGBA_, RGBSpace, AlphaSpace );

		FLUXTMP_GLF_MATCH_( BGRA,                  IList<10,10,10,2>, UUnit, BGRA_, RGBSpace, AlphaSpace );
		FLUXTMP_GLF_MATCH_( BGRA_INTEGER,          IList<10,10,10,2>, UInt, BGRA_, RGBSpace, AlphaSpace );

		// Missing: R11F_G11F_B10F
		// Missing: RGB9_E5

		// More more weirdo formats
		FLUXTMP_GLF_MATCH_( RGB,                   IList<12,12,12>, UUnit, RGB_ );
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Generic pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel.components, __VA_ARGS__ ) ) {             \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		FLUXTMP_GLF_MATCH_( RGBA, SPixelComponents<
			Component< SBits<1>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<5>, UUnit, Blue, RGBSpace >,
			Component< SBits<5>, UUnit, Green, RGBSpace >,
			Component< SBits<5>, UUnit, Red, RGBSpace >
		>{} );


		FLUXTMP_GLF_MATCH_( RGBA, SPixelComponents<
			Component< SBits<8>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<8>, UUnit, Blue, RGBSpace >,
			Component< SBits<8>, UUnit, Green, RGBSpace >,
			Component< SBits<8>, UUnit, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER, SPixelComponents<
			Component< SBits<8>, UInt, Alpha, AlphaSpace >,
			Component< SBits<8>, UInt, Blue, RGBSpace >,
			Component< SBits<8>, UInt, Green, RGBSpace >,
			Component< SBits<8>, UInt, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER, SPixelComponents<
			Component< SBits<8>, SInt, Alpha, AlphaSpace >,
			Component< SBits<8>, SInt, Blue, RGBSpace >,
			Component< SBits<8>, SInt, Green, RGBSpace >,
			Component< SBits<8>, SInt, Red, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( RGBA, SPixelComponents<
			Component< SBits<8>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<8>, UUnit, Blue, SRGBSpace >,
			Component< SBits<8>, UUnit, Green, SRGBSpace >,
			Component< SBits<8>, UUnit, Red, SRGBSpace >
		>{} );


		FLUXTMP_GLF_MATCH_( BGRA, SPixelComponents<
			Component< SBits<2>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<10>, UUnit, Red, RGBSpace >,
			Component< SBits<10>, UUnit, Green, RGBSpace >,
			Component< SBits<10>, UUnit, Blue, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( BGRA_INTEGER, SPixelComponents<
			Component< SBits<2>, UInt, Alpha, AlphaSpace >,
			Component< SBits<10>, UInt, Red, RGBSpace >,
			Component< SBits<10>, UInt, Green, RGBSpace >,
			Component< SBits<10>, UInt, Blue, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( RGBA, SPixelComponents<
			Component< SBits<2>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<10>, UUnit, Blue, RGBSpace >,
			Component< SBits<10>, UUnit, Green, RGBSpace >,
			Component< SBits<10>, UUnit, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( RGBA_INTEGER, SPixelComponents<
			Component< SBits<2>, UInt, Alpha, AlphaSpace >,
			Component< SBits<10>, UInt, Blue, RGBSpace >,
			Component< SBits<10>, UInt, Green, RGBSpace >,
			Component< SBits<10>, UInt, Red, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( DEPTH_STENCIL, SPixelComponents<
			Component< UInt24, UUnit, Depth, DepthSpace >,
			Component< UInt8, UInt, Stencil, StencilSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( DEPTH_STENCIL, SPixelComponents<
			Component< Float, SFloat, Depth, DepthSpace >,
			Component< UInt8, UInt, Stencil, StencilSpace >
		>{} );
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Special "don't care about the details" formats
		////TODO Good idea?
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, PackedPixel<__VA_ARGS__>{} ) ) {         \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		FLUXTMP_GLF_MATCH_( STENCIL_INDEX, IList<8>, AnyDomain, SList<Stencil> );

		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT, IList<16>, AnyDomain, SList<Depth> );
		FLUXTMP_GLF_MATCH_( DEPTH_COMPONENT, IList<32>, AnyDomain, SList<Depth> );

		FLUXTMP_GLF_MATCH_( DEPTH_STENCIL, IList<24,8>, AnyDomain, SList<Depth,Stencil> );
		FLUXTMP_GLF_MATCH_( DEPTH_STENCIL, IList<32,8>, AnyDomain, SList<Depth,Stencil> );
#		undef FLUXTMP_GLF_MATCH_
	}

	template< class tPixel, class tMatchFn > FLUX_CONSTEXPR_EX inline
	void pixel_type( tPixel const& aPixel, tMatchFn&& aMatch )
	{
		using namespace flux::meta;

		using R_ = SList<Red>;
		using RG_ = SList<Red,Green>;
		using RGB_ = SList<Red,Green,Blue>;
		using RGBA_ = SList<Red,Green,Blue,Alpha>;

		using BGR_ = SList<Blue,Green,Red>;
		using BGRA_ = SList<Blue,Green,Red,Alpha>;

		//-/
		//-/ Uniform pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, UniformPixel<__VA_ARGS__>{} ) ) {        \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		// Single channel generic
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UInt, R_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, R_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SInt, R_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SUnit, R_ );

		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UInt, R_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UUnit, R_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SInt, R_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SUnit, R_ );
		FLUXTMP_GLF_MATCH_( HALF_FLOAT,             Half, SFloat, R_ ); //XXX- the spec doesn't mention half float here. Not sure if it's legal

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT,           UInt32, UInt, R_ );
		FLUXTMP_GLF_MATCH_( INT,                    SInt32, SInt, R_ );
		FLUXTMP_GLF_MATCH_( FLOAT,                  Float, SFloat, R_ );

		// Two channel generic
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, RG_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SUnit, RG_ );

		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UUnit, RG_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SUnit, RG_ );
		FLUXTMP_GLF_MATCH_( HALF_FLOAT,             Half, SFloat, RG_ ); //XXX-HALF_FLOAT

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT,           UInt32, UInt, RG_ );
		FLUXTMP_GLF_MATCH_( INT,                    SInt32, SInt, RG_ );
		FLUXTMP_GLF_MATCH_( FLOAT,                  Float, SFloat, RG_ );

		// Three channel generic
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SUnit, RGB_ );

		// Note: BGR textures have the same internal format as RGB ones. It's
		// just the external format that changes.
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UInt, BGR_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, BGR_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SInt, BGR_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SUnit, BGR_ );

		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( HALF_FLOAT,             Half, SFloat, RGB_ ); //XXX-HALF-FLOAT

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT,           UInt32, UInt, RGB_ );
		FLUXTMP_GLF_MATCH_( INT,                    SInt32, SInt, RGB_ );
		FLUXTMP_GLF_MATCH_( FLOAT,                  Float, SFloat, RGB_ );

		// Four channel generic
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SUnit, RGBA_ );

		// Note: BGRA textures have the same internal format as the RGB ones.
		// Only the external format changes.
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UInt, BGRA_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, BGRA_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SInt, BGRA_ );
		FLUXTMP_GLF_MATCH_( BYTE,                   SInt8, SUnit, BGRA_ );

		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( SHORT,                  SInt16, SUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( HALF_FLOAT,             Half, SFloat, RGBA_ ); //XXX-HALF_FLOAT

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT,           UInt32, UInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( INT,                    SInt32, SInt, RGBA_ );
		FLUXTMP_GLF_MATCH_( FLOAT,                  Float, SFloat, RGBA_ );

		// Stencil only formats
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UInt, SList<Stencil> );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UInt, SList<Stencil> );

		// Depth only formats
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT,         UInt16, UUnit, SList<Depth>, DepthSpace );
		// Depth24: no match.
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT,           UInt32, UUnit, SList<Depth>, DepthSpace );
		FLUXTMP_GLF_MATCH_( FLOAT,                  Float, SFloat, SList<Depth>, DepthSpace );

		// SRGB formats
		// See notes regarding BGR vs RGB above
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, RGB_, SRGBSpace );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, RGBA_, SRGBSpace, AlphaSpace );

		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, BGR_, SRGBSpace );
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE,          UInt8, UUnit, BGRA_, SRGBSpace, AlphaSpace );
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Packed pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, PackedPixel<__VA_ARGS__>{} ) ) {         \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		// Low bit stencils
		// Stencil1: no match
		// Stencil4: no match

		// 8-bit formats
		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE_3_3_2,   IList<3,3,2>, UUnit, RGB_ );
		// RGBA2: no match

		// Weirdo formats
		// RGB4: no match
		// RGB5: no match

		// 16-bit formats
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT_5_6_5,        IList<5,6,5>, UUnit, RGB_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT_5_6_5_REV,    IList<5,6,5>, UUnit, BGR_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT_4_4_4_4,      IList<4,4,4,4>, UUnit, RGBA_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT_4_4_4_4_REV,  IList<4,4,4,4>, UUnit, BGRA_ );
		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT_5_5_5_1,      IList<5,5,5,1>, UUnit, RGBA_ );

		// More weirdo formats
		// RGB10: no match

		// 32-bit formats
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_10_10_10_2,  IList<10,10,10,2>, UUnit, RGBA_, RGBSpace, AlphaSpace );
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_10_10_10_2,  IList<10,10,10,2>, UInt, RGBA_, RGBSpace, AlphaSpace );

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_10_10_10_2,  IList<10,10,10,2>, UUnit, BGRA_, RGBSpace, AlphaSpace );
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_10_10_10_2,  IList<10,10,10,2>, UInt, BGRA_, RGBSpace, AlphaSpace );

		// Missing: R11F_G11F_B10F
		// Missing: RGB9_E5

		// More more weirdo formats
		// RGB12: no match
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Generic pixel formats
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel.components, __VA_ARGS__ ) ) {             \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT_1_5_5_5_REV, SPixelComponents<
			Component< SBits<1>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<5>, UUnit, Blue, RGBSpace >,
			Component< SBits<5>, UUnit, Green, RGBSpace >,
			Component< SBits<5>, UUnit, Red, RGBSpace >
		>{} );


		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_8_8_8_8_REV, SPixelComponents<
			Component< SBits<8>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<8>, UUnit, Blue, RGBSpace >,
			Component< SBits<8>, UUnit, Green, RGBSpace >,
			Component< SBits<8>, UUnit, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_8_8_8_8_REV, SPixelComponents<
			Component< SBits<8>, UInt, Alpha, AlphaSpace >,
			Component< SBits<8>, UInt, Blue, RGBSpace >,
			Component< SBits<8>, UInt, Green, RGBSpace >,
			Component< SBits<8>, UInt, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_8_8_8_8_REV, SPixelComponents<
			Component< SBits<8>, SInt, Alpha, AlphaSpace >,
			Component< SBits<8>, SInt, Blue, RGBSpace >,
			Component< SBits<8>, SInt, Green, RGBSpace >,
			Component< SBits<8>, SInt, Red, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_8_8_8_8_REV, SPixelComponents<
			Component< SBits<8>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<8>, UUnit, Blue, SRGBSpace >,
			Component< SBits<8>, UUnit, Green, SRGBSpace >,
			Component< SBits<8>, UUnit, Red, SRGBSpace >
		>{} );


		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_2_10_10_10_REV, SPixelComponents<
			Component< SBits<2>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<10>, UUnit, Red, RGBSpace >,
			Component< SBits<10>, UUnit, Green, RGBSpace >,
			Component< SBits<10>, UUnit, Blue, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_2_10_10_10_REV, SPixelComponents<
			Component< SBits<2>, UInt, Alpha, AlphaSpace >,
			Component< SBits<10>, UInt, Red, RGBSpace >,
			Component< SBits<10>, UInt, Green, RGBSpace >,
			Component< SBits<10>, UInt, Blue, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_2_10_10_10_REV, SPixelComponents<
			Component< SBits<2>, UUnit, Alpha, AlphaSpace >,
			Component< SBits<10>, UUnit, Blue, RGBSpace >,
			Component< SBits<10>, UUnit, Green, RGBSpace >,
			Component< SBits<10>, UUnit, Red, RGBSpace >
		>{} );
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_2_10_10_10_REV, SPixelComponents<
			Component< SBits<2>, UInt, Alpha, AlphaSpace >,
			Component< SBits<10>, UInt, Blue, RGBSpace >,
			Component< SBits<10>, UInt, Green, RGBSpace >,
			Component< SBits<10>, UInt, Red, RGBSpace >
		>{} );

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_24_8, SPixelComponents<
			Component< UInt24, UUnit, Depth, DepthSpace >,
			Component< UInt8, UInt, Stencil, StencilSpace >
		>{} );

		// Depth32.Stencil8: no match; FLOAT32_UNSIGNED_INT24_8 would be an option,
		//   but it has the 24 unused bits inbetween, so that's weird.
#		undef FLUXTMP_GLF_MATCH_

		//-/
		//-/ Special "don't care about the details" formats
		////TODO Good idea?
#		define FLUXTMP_GLF_MATCH_( tgt, ... )                       \
		if( match( aPixel, PackedPixel<__VA_ARGS__>{} ) ) {         \
			if( aMatch( GL::tgt ) )                                 \
				return;                                             \
		} /*ENDM*/

		FLUXTMP_GLF_MATCH_( UNSIGNED_BYTE, IList<8>, AnyDomain, SList<Stencil> );

		FLUXTMP_GLF_MATCH_( UNSIGNED_SHORT, IList<16>, AnyDomain, SList<Depth> );
		FLUXTMP_GLF_MATCH_( UNSIGNED_INT, IList<32>, AnyDomain, SList<Depth> );

		FLUXTMP_GLF_MATCH_( UNSIGNED_INT_24_8, IList<24,8>, AnyDomain, SList<Depth,Stencil> );

		// Depth32.Stencil8: no match (see above)
#		undef FLUXTMP_GLF_MATCH_
	}
}

//--    internal_format*()                   ///{{{2///////////////////////////
template< class tLayout, class tComps, class tFormats > FLUX_GL_FORMAT_CONSTEXPR inline
GLenum internal_format( meta::Pixel<tLayout,tComps,tFormats> const& aPixelDesc )
{
	switch( aPixelDesc.layout )
	{
		case meta::ELayout::interleaved:
		case meta::ELayout::packed: {
			GLenum ret = GL::NONE;
			detail::internal_format( aPixelDesc, [&ret] (GLenum aFormat) {
				ret = aFormat;
				return true;
			} );
			return ret;
		} break;
		case meta::ELayout::special: {
			if( auto const val = aPixelDesc.fixedFormats.template get<meta::OpenGL>() )
				return GLenum(val.value().value);
		} break;
		default: break;
	}

	return GL::NONE;
}

template< class tLayout, class tComps, class tFormats > inline
ftl::Vector<GLenum> internal_formats( meta::Pixel<tLayout,tComps,tFormats> const& aPixelDesc )
{
	switch( aPixelDesc.layout )
	{
		case meta::ELayout::interleaved:
		case meta::ELayout::packed: {
			ftl::Vector<GLenum> ret;
			detail::internal_format( aPixelDesc, [&ret] (GLenum aFormat) {
				ret.emplace_back( aFormat );
				return false;
			} );
			return ret;
		} break;
		case meta::ELayout::special: {
			if( auto const val = aPixelDesc.fixedFormats.template get<meta::OpenGL>() )
				return { GLenum(val.value().value) };
		} break;
		default: break;
	}

	return {};
}

//--    pixel_format*()                     ///{{{2///////////////////////////
template< class tLayout, class tComps, class tFormats > FLUX_GL_FORMAT_CONSTEXPR inline
GLenum pixel_format( meta::Pixel<tLayout,tComps,tFormats> const& aPixelDesc )
{
	switch( aPixelDesc.layout )
	{
		case meta::ELayout::interleaved:
		case meta::ELayout::packed: {
			GLenum ret = GL::NONE;
			detail::pixel_format( aPixelDesc, [&ret] (GLenum aFormat) {
				ret = aFormat;
				return true;
			} );
			return ret;
		} break;
		case meta::ELayout::special: {
			//TODO
			//TODO
			//TODO
			//TODO
		} break;
		default: break;
	}

	return GL::NONE;
}

template< class tLayout, class tComps, class tFormats > inline
ftl::Vector<GLenum> pixel_formats( meta::Pixel<tLayout,tComps,tFormats> const& aPixelDesc )
{
	switch( aPixelDesc.layout )
	{
		case meta::ELayout::interleaved:
		case meta::ELayout::packed: {
			ftl::Vector<GLenum> ret;
			detail::pixel_format( aPixelDesc, [&ret] (GLenum aFormat) {
				ret.emplace_back( aFormat );
				return false;
			} );
			return ret;
		} break;
		case meta::ELayout::special: {
			//TODO 
			//TODO 
			//TODO 
			//TODO 
		} break;
		default: break;
	}

	return {};
}

//--    pixel_type*()                       ///{{{2///////////////////////////
template< class tLayout, class tComps, class tFormats > FLUX_GL_FORMAT_CONSTEXPR inline
GLenum pixel_type( meta::Pixel<tLayout,tComps,tFormats> const& aPixelDesc )
{
	switch( aPixelDesc.layout )
	{
		case meta::ELayout::interleaved:
		case meta::ELayout::packed: {
			GLenum ret = GL::NONE;
			detail::pixel_type( aPixelDesc, [&ret] (GLenum aFormat) {
				ret = aFormat;
				return true;
			} );
			return ret;
		} break;
		case meta::ELayout::special: {
			//TODO
			//TODO
			//TODO
			//TODO
		} break;
		default: break;
	}

	return GL::NONE;
}

template< class tLayout, class tComps, class tFormats > inline
ftl::Vector<GLenum> pixel_types( meta::Pixel<tLayout,tComps,tFormats> const& aPixelDesc )
{
	switch( aPixelDesc.layout )
	{
		case meta::ELayout::interleaved:
		case meta::ELayout::packed: {
			ftl::Vector<GLenum> ret;
			detail::pixel_type( aPixelDesc, [&ret] (GLenum aFormat) {
				ret.emplace_back( aFormat );
				return false;
			} );
			return ret;
		} break;
		case meta::ELayout::special: {
			//TODO
			//TODO
			//TODO
			//TODO
		} break;
		default: break;
	}

	return {};
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
