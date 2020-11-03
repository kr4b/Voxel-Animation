/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Styling for stdio
 *
 * This exposes a number of styles:
 *   - Plain: (nothing)
 *   - Basic: corresponds to the basic SGR commands
 *   - 256: full, muted-light, muted-dark
 *   - RGB: full, muted-light, muted-dark
 */
/*-***************************************************************** -}}}1- */

#include <flux/sys/stdio-style.hpp>
#include <flux/sys/stdio.hpp>

#include <flux/std/assert.hpp>
#include <flux/std/static_assert.hpp>

#include <atomic>

#include <cstddef>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::sys >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(sys)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	constexpr char const* kEmptyString_ = "";
	constexpr char const* kFullReset_ = "\033[0m";

	constexpr std::size_t kThemeCount_ = 3;
	constexpr std::size_t kSchemeCount_ = 2;

	constexpr std::size_t kFmtVariations_ = 16; // four bits

	FLUX_STATIC_ASSERTM( 
		kSchemeCount_ == std::size_t(EStdioStyleScheme::max),
		"kSchemeCount_ must match EStdioStyleScheme::max"
	);
	
	std::atomic<EStdioStyleScheme> gStyleScheme_{ EStdioStyleScheme::muted };
}

//--    $ plain style                       ///{{{2///////////////////////////
namespace
{
	namespace plain
	{
		constexpr char const* kExplicitFmt_[ std::size_t(EStdioFmt::max) ] =
		{
			/* plain */      kEmptyString_,
			/* bold */       kEmptyString_,
			/* light */      kEmptyString_,
			/* italic */     kEmptyString_,
			/* underline */  kEmptyString_,

		};

		constexpr char const* kExplicitCol_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      kEmptyString_,
			/* black */      kEmptyString_,
			/* red */        kEmptyString_,
			/* green */      kEmptyString_,
			/* yellow */     kEmptyString_,
			/* blue */       kEmptyString_,
			/* magenta */    kEmptyString_,
			/* cyan */       kEmptyString_,
			/* white */      kEmptyString_,
		};

		constexpr char const* kSemanticCol_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kEmptyString_,
			/* attn */       kEmptyString_,
			/* warn */       kEmptyString_,
			/* error */      kEmptyString_,
			/* hint */       kEmptyString_,
			/* note */       kEmptyString_,
			/* aside */      kEmptyString_,
			/* ack */        kEmptyString_,
			/* nak */        kEmptyString_,
			/* cat0 */       kEmptyString_,
			/* cat1 */       kEmptyString_,
			/* cat2 */       kEmptyString_,
			/* cat3 */       kEmptyString_,
		};
	}
}

//--    $ basic style                       ///{{{2///////////////////////////
namespace
{
	namespace basic
	{
		constexpr char const* kExplicitFmt_[ std::size_t(EStdioFmt::max) ] =
		{
			/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
			/* bold */       "\033[1m",
			/* light */      "\033[2m",
			/* italic */     "\033[3m",
			/* underline */  "\033[4m",
		};

		constexpr char const* kExplicitCol_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      "\033[39m",
			/* black */      "\033[30m",
			/* red */        "\033[31m",
			/* green */      "\033[32m",
			/* yellow */     "\033[33m",
			/* blue */       "\033[34m",
			/* magenta */    "\033[35m",
			/* cyan */       "\033[36m",
			/* white */      "\033[37m",
		};

		constexpr char const* kSemanticCol_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kExplicitCol_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitCol_[std::size_t(EStdioColor::yellow)],
			/* warn */       kExplicitCol_[std::size_t(EStdioColor::yellow)],
			/* error */      kExplicitCol_[std::size_t(EStdioColor::red)],
			/* hint */       kExplicitCol_[std::size_t(EStdioColor::plain)],
			/* note */       kExplicitCol_[std::size_t(EStdioColor::plain)],
			/* aside */      kExplicitCol_[std::size_t(EStdioColor::plain)],
			/* ack */        kExplicitCol_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitCol_[std::size_t(EStdioColor::red)],
			/* cat0 */       kExplicitCol_[std::size_t(EStdioColor::cyan)],
			/* cat1 */       kExplicitCol_[std::size_t(EStdioColor::magenta)],
			/* cat2 */       kExplicitCol_[std::size_t(EStdioColor::blue)],
			/* cat3 */       kExplicitCol_[std::size_t(EStdioColor::black)],
		};
	}
}

//--    $ full intensity style              ///{{{2///////////////////////////
namespace
{
	namespace full
	{
		constexpr char const* kExplicitCol256_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      "\033[39m",
			/* black */      "\033[38;5;16m",
			/* red */        "\033[38;5;196m",
			/* green */      "\033[38;5;46m",
			/* yellow */     "\033[38;5;226m",
			/* blue */       "\033[38;5;21m",
			/* magenta */    "\033[38;5;201m",
			/* cyan */       "\033[38;5;51m",
			/* white */      "\033[38;5;231m",
		};
		constexpr char const* kExplicitColRGB_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      "\033[39m",
			/* black */      "\033[38;2;0;0;0m",
			/* red */        "\033[38;2;255;0;0m",
			/* green */      "\033[38;2;0;255;0m",
			/* yellow */     "\033[38;2;255;255;0m",
			/* blue */       "\033[38;2;0;0;255m",
			/* magenta */    "\033[38;2;255;0;255m",
			/* cyan */       "\033[38;2;0;255;255m",
			/* white */      "\033[38;2;255;255;255m",
		};

		constexpr char const* kSemanticCol256l_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kExplicitCol256_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitCol256_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;5;208m",
			/* error */      kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;5;110m",
			/* note */       "\033[38;5;238m",
			/* aside */      "\033[38;5;240m",
			/* ack */        kExplicitCol256_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;5;60m",
			/* cat1 */       "\033[38;5;61m",
			/* cat2 */       "\033[38;5;62m",
			/* cat3 */       "\033[38;5;63m",
		};
		constexpr char const* kSemanticCol256d_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kExplicitCol256_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitCol256_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;5;208m",
			/* error */      kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;5;110m",
			/* note */       "\033[38;5;240m",
			/* aside */      "\033[38;5;238m",
			/* ack */        kExplicitCol256_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;5;60m",
			/* cat1 */       "\033[38;5;61m",
			/* cat2 */       "\033[38;5;62m",
			/* cat3 */       "\033[38;5;63m",
		};

		constexpr char const* kSemanticColRGBl_[std::size_t(EStdioSemanticColor::max)] =
		{
			/* plain */      kExplicitColRGB_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitColRGB_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;2;255;135;0m",
			/* error */      kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;2;135;175;255m",
			/* note */       "\033[38;2;68;68;68m",
			/* aside */      "\033[38;2;88;88;88m",
			/* ack */        kExplicitColRGB_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;2;87;87;135m",
			/* cat1 */       "\033[38;2;87;87;175m",
			/* cat2 */       "\033[38;2;87;87;215m",
			/* cat3 */       "\033[38;2;87;87;255m",
		};
		constexpr char const* kSemanticColRGBd_[std::size_t(EStdioSemanticColor::max)] =
		{
			/* plain */      kExplicitColRGB_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitColRGB_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;2;255;135;0m",
			/* error */      kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;2;135;175;255m",
			/* note */       "\033[38;2;88;88;88m",
			/* aside */      "\033[38;2;68;68;68m",
			/* ack */        kExplicitColRGB_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;2;87;87;135m",
			/* cat1 */       "\033[38;2;87;87;175m",
			/* cat2 */       "\033[38;2;87;87;215m",
			/* cat3 */       "\033[38;2;87;87;255m",
		};

	}
}

//--    $ muted-light style                 ///{{{2///////////////////////////
namespace
{
	namespace mutedLight
	{
		constexpr char const* kExplicitCol256_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      "\033[39m",
			/* black */      "\033[38;5;233m",
			/* red */        "\033[38;5;124m",
			/* green */      "\033[38;5;106m",
			/* yellow */     "\033[38;5;214m",
			/* blue */       "\033[38;5;19m",
			/* magenta */    "\033[38;5;91m",
			/* cyan */       "\033[38;5;26m",
			/* white */      "\033[38;5;246m",
		};
		constexpr char const* kExplicitColRGB_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      "\033[39m",
			/* black */      "\033[38;2;75;75;75m",
			/* red */        "\033[38;2;140;10;10m",
			/* green */      "\033[38;2;10;140;18m",
			/* yellow */     "\033[38;2;140;126;10m",
			/* blue */       "\033[38;2;14;10;140m",
			/* magenta */    "\033[38;2;140;10;140m",
			/* cyan */       "\033[38;2;10;140;140m",
			/* white */      "\033[38;2;180;180;180m",
		};

		constexpr char const* kSemanticCol256_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kExplicitCol256_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitCol256_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;5;166m",
			/* error */      kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;5;110m",
			/* note */       "\033[38;5;238m",
			/* aside */      "\033[38;5;240m",
			/* ack */        kExplicitCol256_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;5;60m",
			/* cat1 */       "\033[38;5;61m",
			/* cat2 */       "\033[38;5;62m",
			/* cat3 */       "\033[38;5;63m",
		};
		constexpr char const* kSemanticColRGB_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kExplicitColRGB_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitColRGB_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;2;140;77;10m",
			/* error */      kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;2;44;96;106m",
			/* note */       "\033[38;2;69;79;91m",
			/* aside */      "\033[38;2;112;128;131m",
			/* ack */        kExplicitColRGB_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;2;87;87;135m",
			/* cat1 */       "\033[38;2;87;87;175m",
			/* cat2 */       "\033[38;2;87;87;215m",
			/* cat3 */       "\033[38;2;87;87;255m",
		};
	}
}

//--    $ muted-dark style                  ///{{{2///////////////////////////
namespace
{
	namespace mutedDark
	{
		constexpr char const* kExplicitCol256_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      "\033[39m",
			/* black */      "\033[38;5;243m",
			/* red */        "\033[38;5;124m",
			/* green */      "\033[38;5;106m",
			/* yellow */     "\033[38;5;214m",
			/* blue */       "\033[38;5;27m",
			/* magenta */    "\033[38;5;128m",
			/* cyan */       "\033[38;5;45m",
			/* white */      "\033[38;5;254m",
		};
		constexpr char const* kExplicitColRGB_[ std::size_t(EStdioColor::max) ] = 
		{
			/* plain */      "\033[39m",
			/* black */      "\033[38;2;110;110;110m",
			/* red */        "\033[38;2;242;48;48m",
			/* green */      "\033[38;2;50;240;61m",
			/* yellow */     "\033[38;2;240;207;50m",
			/* blue */       "\033[38;2;56;50;240m",
			/* magenta */    "\033[38;2;240;50;240m",
			/* cyan */       "\033[38;2;50;240;240m",
			/* white */      "\033[38;2;240;240;240m",
		};

		constexpr char const* kSemanticCol256_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kExplicitCol256_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitCol256_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;5;208m",
			/* error */      kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;5;110m",
			/* note */       "\033[38;5;240m",
			/* aside */      "\033[38;5;238m",
			/* ack */        kExplicitCol256_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitCol256_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;5;60m",
			/* cat1 */       "\033[38;5;61m",
			/* cat2 */       "\033[38;5;62m",
			/* cat3 */       "\033[38;5;63m",
		};
		constexpr char const* kSemanticColRGB_[ std::size_t(EStdioSemanticColor::max) ] =
		{
			/* plain */      kExplicitColRGB_[std::size_t(EStdioColor::plain)],
			/* attn */       kExplicitColRGB_[std::size_t(EStdioColor::yellow)],
			/* warn */       "\033[38;2;240;152;50m",
			/* error */      kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* hint */       "\033[38;2;100;176;190m",
			/* note */       "\033[38;2;88;88;88m",
			/* aside */      "\033[38;2;68;68;68m",
			/* ack */        kExplicitColRGB_[std::size_t(EStdioColor::green)],
			/* nak */        kExplicitColRGB_[std::size_t(EStdioColor::red)],
			/* cat0 */       "\033[38;2;87;87;135m",
			/* cat1 */       "\033[38;2;87;87;175m",
			/* cat2 */       "\033[38;2;87;87;215m",
			/* cat3 */       "\033[38;2;87;87;255m",
		};
	}
}

//--    $ formatting                        ///{{{2///////////////////////////
namespace
{
	namespace fmts
	{
		constexpr char const* kSematicFormats[kFmtVariations_][ std::size_t(EStdioSemanticFmt::max) ] =
		{
			// 0: Nothing
			{
				/* plain */      kEmptyString_,
				/* heading */    kEmptyString_,
				/* paragraph */  kEmptyString_,
				/* emph */       kEmptyString_,
				/* note */       kEmptyString_,
			},
			// 1: Only: bold
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1m",
				/* paragraph */  kEmptyString_,
				/* emph */       kEmptyString_,
				/* note */       kEmptyString_,
			},
			// 2: Only: faint
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    kEmptyString_,
				/* paragraph */  kEmptyString_,
				/* emph */       kEmptyString_,
				/* note */       "\033[2m",
			},
			// 3: Only: bold|faint
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1m",
				/* paragraph */  kEmptyString_,
				/* emph */       kEmptyString_,
				/* note */       "\033[2m",
			},
			// 4: Only: italic
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[3m",
				/* paragraph */  kEmptyString_,
				/* emph */       "\033[3m",
				/* note */       kEmptyString_,
			},
			// 5: Only: bold|italic
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1m",
				/* paragraph */  kEmptyString_,
				/* emph */       "\033[3m",
				/* note */       kEmptyString_,
			},
			// 6: Only: light|italic
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[3m",
				/* paragraph */  kEmptyString_,
				/* emph */       "\033[3m",
				/* note */       "\033[2m",
			},
			// 7: Only: bold|light|italic
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1m",
				/* paragraph */  kEmptyString_,
				/* emph */       "\033[3m",
				/* note */       "\033[2m",
			},
			// 8: Only: underline
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[4m",
				/* paragraph */  kEmptyString_,
				/* emph */       kEmptyString_,
				/* note */       kEmptyString_,
			},
			// 9: Only: bold|underline
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1;4m",
				/* paragraph */  "\033[1m",
				/* emph */       "\033[4m",
				/* note */       kEmptyString_,
			},
			// 10: Only: light|underline
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[4m",
				/* paragraph */  kEmptyString_,
				/* emph */       kEmptyString_,
				/* note */       "\033[2m",
			},
			// 11: Only: bold|light|underline
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1;4m",
				/* paragraph */  "\033[1m",
				/* emph */       "\033[4m",
				/* note */       "\033[2m",
			},
			// 12: Only: italic|underline
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[3;4m",
				/* paragraph */  "\033[4m",
				/* emph */       "\033[3m",
				/* note */       kEmptyString_,
			},
			// 13: Only: bold|italic|underline
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1;4m",
				/* paragraph */  "\033[1m",
				/* emph */       "\033[3m",
				/* note */       kEmptyString_,
			},
			// 14: Only: light|italic|underline
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[3;4m",
				/* paragraph */  "\033[4m",
				/* emph */       "\033[3m",
				/* note */       "\033[2m",
			},
			// 15: All
			{
				/* plain */      "\033[22;23;24m", //XXX-does this work on Win32?
				/* heading */    "\033[1;4m",
				/* paragraph */  "\033[1m",
				/* emph */       "\033[3m",
				/* note */       "\033[2m",
			},
		};
	}
}

//--    $ styles                            ///{{{2///////////////////////////
namespace
{
	constexpr char const* const* explicit256[kThemeCount_][kSchemeCount_] =
	{
		// EStdioColorTheme::plain
		{
			plain::kExplicitCol_,
			plain::kExplicitCol_,
		},
		// EStdioColorTheme::light
		{
			mutedLight::kExplicitCol256_,
			full::kExplicitCol256_,
		},
		// EStdioColorTheme::dark
		{
			mutedDark::kExplicitCol256_,
			full::kExplicitCol256_,
		},
	};
	constexpr char const* const* explicitRGB[kThemeCount_][kSchemeCount_] =
	{
		// EStdioColorTheme::plain
		{
			plain::kExplicitCol_,
			plain::kExplicitCol_,
		},
		// EStdioColorTheme::light
		{
			mutedLight::kExplicitColRGB_,
			full::kExplicitColRGB_,
		},
		// EStdioColorTheme::dark
		{
			mutedDark::kExplicitColRGB_,
			full::kExplicitColRGB_,
		},
	};

	constexpr char const* const* semantic256[kThemeCount_][kSchemeCount_] =
	{
		// EStdioColorTheme::plain
		{
			plain::kSemanticCol_,
			plain::kSemanticCol_,
		},
		// EStdioColorTheme::light
		{
			mutedLight::kSemanticCol256_,
			full::kSemanticCol256l_,
		},
		// EStdioColorTheme::dark
		{
			mutedDark::kSemanticCol256_,
			full::kSemanticCol256d_,
		},
	};
	constexpr char const* const* semanticRGB[kThemeCount_][kSchemeCount_] =
	{
		// EStdioColorTheme::plain
		{
			plain::kSemanticCol_,
			plain::kSemanticCol_,
		},
		// EStdioColorTheme::light
		{
			mutedLight::kSemanticColRGB_,
			full::kSemanticColRGBl_,
		},
		// EStdioColorTheme::dark
		{
			mutedDark::kSemanticColRGB_,
			full::kSemanticColRGBd_,
		},
	};
}

//--    stdio_style()                       ///{{{2///////////////////////////

#define FLUXTMP_ENSURE_CAPS_(c)                        \
	StdioCaps localCaps;                               \
	if( !c ) {                                         \
		localCaps = stdio_probe( EStdioStream::out );  \
		c = &localCaps;                                \
	} FLUX_ASSERT( aCaps ); /*ENDM*/

char const* stdio_style( StdioStyleReset, StdioCaps const* aCaps ) noexcept
{
	FLUXTMP_ENSURE_CAPS_( aCaps );
	return aCaps->hasAnsi ? kFullReset_ : kEmptyString_;
}

char const* stdio_style( EStdioFmt aFmt, StdioCaps const* aCaps ) noexcept
{
	FLUXTMP_ENSURE_CAPS_( aCaps );
	FLUX_ASSERT( std::size_t(aFmt) < std::size_t(EStdioFmt::max) );

	char const* const* selectedTable = plain::kExplicitFmt_;
	if( aCaps->hasAnsi )
	{
		selectedTable = basic::kExplicitFmt_;
	}

	FLUX_ASSERT( selectedTable );
	return selectedTable[ std::size_t(aFmt) ];
}
char const* stdio_style( EStdioColor aCol, StdioCaps const* aCaps ) noexcept
{
	FLUXTMP_ENSURE_CAPS_( aCaps );
	FLUX_ASSERT( std::size_t(aCol) < std::size_t(EStdioColor::max) );

	char const* const* selectedTable = plain::kExplicitCol_;
	if( aCaps->hasAnsi )
	{
		switch( aCaps->colorMode )
		{
			case EStdioColorMode::none:
				selectedTable = plain::kExplicitCol_;
				break;
			case EStdioColorMode::basic:
				selectedTable = basic::kExplicitCol_;
				break;
			case EStdioColorMode::_8bit: {
				auto const theme = std::size_t(aCaps->colorTheme);
				FLUX_ASSERT( theme < kThemeCount_ );

				auto const scheme = std::size_t(gStyleScheme_.load());
				FLUX_ASSERT( scheme < kSchemeCount_ );

				selectedTable = explicit256[theme][scheme];
			} break;
			
			case EStdioColorMode::rgb: {
				auto const theme = std::size_t(aCaps->colorTheme);
				FLUX_ASSERT( theme < kThemeCount_ );

				auto const scheme = std::size_t(gStyleScheme_.load());
				FLUX_ASSERT( scheme < kSchemeCount_ );

				selectedTable = explicitRGB[theme][scheme];
			} break;
		}
	}

	FLUX_ASSERT( selectedTable );
	return selectedTable[ std::size_t(aCol) ];
}

char const* stdio_style( EStdioSemanticFmt aFmt, StdioCaps const* aCaps ) noexcept
{
	FLUX_ASSERT( std::size_t(aFmt) < std::size_t(EStdioSemanticFmt::max) );
	FLUXTMP_ENSURE_CAPS_( aCaps );
	FLUX_ASSERT( std::size_t(aCaps->usableFmt) < kFmtVariations_ );
	return fmts::kSematicFormats[std::size_t(aCaps->usableFmt)][std::size_t(aFmt)];
}
char const* stdio_style( EStdioSemanticColor aCol, StdioCaps const* aCaps ) noexcept
{
	FLUXTMP_ENSURE_CAPS_( aCaps );
	FLUX_ASSERT( std::size_t(aCol) < std::size_t(EStdioSemanticColor::max) );

	char const* const* selectedTable = plain::kSemanticCol_;
	if( aCaps->hasAnsi )
	{
		switch( aCaps->colorMode )
		{
			case EStdioColorMode::none:
				selectedTable = plain::kSemanticCol_;
				break;
			case EStdioColorMode::basic:
				selectedTable = basic::kSemanticCol_;
				break;
			case EStdioColorMode::_8bit: {
				auto const theme = std::size_t(aCaps->colorTheme);
				FLUX_ASSERT( theme < kThemeCount_ );

				auto const scheme = std::size_t(gStyleScheme_.load());
				FLUX_ASSERT( scheme < kSchemeCount_ );

				selectedTable = semantic256[theme][scheme];
			} break;
			
			case EStdioColorMode::rgb: {
				auto const theme = std::size_t(aCaps->colorTheme);
				FLUX_ASSERT( theme < kThemeCount_ );

				auto const scheme = std::size_t(gStyleScheme_.load());
				FLUX_ASSERT( scheme < kSchemeCount_ );

				selectedTable = semanticRGB[theme][scheme];
			} break;
		}
	}

	FLUX_ASSERT( selectedTable );
	return selectedTable[ std::size_t(aCol) ];
}

#undef FLUXTMP_ENSURE_CAPS_

//--    stdio_style_config()                ///{{{2///////////////////////////

EStdioStyleScheme stdio_style_config( EStdioStyleScheme aScheme ) noexcept
{
	FLUX_ASSERT( std::size_t(aScheme) < kSchemeCount_ );
	return gStyleScheme_.exchange( aScheme );
}

//--    <<< ~ flux::sys namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(sys)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
