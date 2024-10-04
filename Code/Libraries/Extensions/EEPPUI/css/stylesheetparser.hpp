#ifndef EE_UI_CSS_STYLESHEETPARSER
#define EE_UI_CSS_STYLESHEETPARSER

#include <eepp/core/string.hpp>
#include <eepp/system/iostream.hpp>
#include <eepp/ui/css/mediaquery.hpp>
#include <eepp/ui/css/stylesheet.hpp>
#include <eepp/ui/css/stylesheetpropertiesparser.hpp>
#include <string>
using namespace EE;
using namespace EE::System;

namespace EE { namespace System {
class Pack;
}} // namespace EE::System

namespace EE { namespace UI { namespace CSS {

class ZeroShared StyleSheetParser {
  public:
	StyleSheetParser();

	bool loadFromStream( IOStream& stream );

	bool loadFromFile( const String& file );

	bool loadFromMemory( const Uint8* RAWData, const Uint32& size );

	bool loadFromPack( Pack* pack, String filePackPath );

	bool loadFromString( const String& str );

	bool loadFromString( const String_view& str );

	void print();

	StyleSheet& getStyleSheet();

	const bool& isLoaded() const;

  protected:
	enum ReadState { ReadingSelector, ReadingProperty, ReadingComment };

	String mCSS;
	StyleSheet mStyleSheet;
	std::vector<String> mComments;
	MediaQueryList::ptr mMediaQueryList;
	bool mLoaded;

	bool parse( String& css, std::vector<String>& importedList );

	int readSelector( const String& css, ReadState& rs, std::size_t pos, String& buffer );

	int readComment( const String& css, ReadState& rs, std::size_t pos, String& buffer );

	int readProperty( const String& css, ReadState& rs, std::size_t pos, String& buffer );

	String importCSS( String path, std::vector<String>& importedList );

	void mediaParse( String& css, ReadState& rs, std::size_t& pos, String& buffer,
					 std::vector<String>& importedList );

	void importParse( String& css, std::size_t& pos, String& buffer,
					  std::vector<String>& importedList );

	void keyframesParse( String& css, ReadState& rs, std::size_t& pos, String& buffer );
};

}}} // namespace EE::UI::CSS

#endif
