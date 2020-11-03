/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	Basic Consumer
 */
/*-***************************************************************** -}}}1- */

#include <flux/report/instr/consumer_basic.hpp>

#include <flux/report/instr/info.hpp>
#include <flux/report/instr/event.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::report::instr >>>     ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(report,instr)
//--    $ local                         ///{{{2///////////////////////////////
namespace
{
	class BasicConsumer_ final : public Consumer
	{
		public:
			BasicConsumer_( std::FILE*, bool ) noexcept;
			~BasicConsumer_();

		public: /* virtuals from Consumer */
			EConsumerDataFormat data_formats() override;
			void feed_raw( Info const&, EventBuffer const& ) override;

		private:
			std::FILE* mFile;
			bool mCloseOnDestroy;
	};
}

//--    functions                       ///{{{2///////////////////////////////

unique_ptr<Consumer> consumer_basic_create( ftl::not_null<std::FILE*> aFile, bool aCloseOnDestroy )
{
	return make_unique<BasicConsumer_>( aFile, aCloseOnDestroy );
}

//--    $ BasicConsumer_                ///{{{2///////////////////////////////
namespace
{
	BasicConsumer_::BasicConsumer_( std::FILE* aFile, bool aDestroy ) noexcept
		: mFile(aFile)
		, mCloseOnDestroy( aDestroy )
	{}

	BasicConsumer_::~BasicConsumer_()
	{
		if( mCloseOnDestroy )
			std::fclose( mFile );
	}


	EConsumerDataFormat BasicConsumer_::data_formats() 
	{
		return EConsumerDataFormat::raw;
	}

	void BasicConsumer_::feed_raw( Info const& aInfo, EventBuffer const& aBuffer )
	{
		std::fprintf( mFile, "%s [%s ; %s] ;; %zu methods\n", aInfo.ident, aInfo.thread, aInfo.stage, aInfo.numMethods );

		double frameBegin, frameEnd;

		int indent = 0;
		for( std::size_t i = 0; i < aBuffer.types.size(); ++i )
		{
			switch( aBuffer.types[i] )
			{
				case EEvent::frameBegin: {
					std::fprintf( mFile, "{\n" );
					++indent;
					frameBegin = aBuffer.values[0][i];
				} break;
				case EEvent::frameEnd: {
					double frameTotal = (frameEnd = aBuffer.values[0][i]) - frameBegin;
					std::fprintf( mFile, "} %.2f%s\n", frameTotal, aInfo.methodUnits[0] );
					--indent;
				} break;
				case EEvent::frameProcessed: {
					double overhead = aBuffer.values[0][i]-frameEnd;
					std::fprintf( mFile, "Processing overhead for frame: %.2fms\n", overhead );
				} break;

				default: {
					std::fprintf( mFile, "%*s%4d: %s = %.2f%s\n", indent, "", int(aBuffer.types[i]), aInfo.methodNames[0], aBuffer.values[0][i], aInfo.methodUnits[0] );
					break;
				}
			}
		}
	}
}

//--    <<< ~ flux::report::instr namespace <<<     ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(report,instr)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
