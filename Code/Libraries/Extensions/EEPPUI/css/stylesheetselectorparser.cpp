#include <eepp/ui/css/stylesheetselectorparser.hpp>

namespace Zero { namespace UI { namespace CSS {

StyleSheetSelectorParser::StyleSheetSelectorParser() {}

StyleSheetSelectorParser::StyleSheetSelectorParser( String name ) {
	Array<String> sels = String::split( name, ',' );

	for ( auto it = sels.begin(); it != sels.end(); ++it ) {
		String cur = String::trim( *it );
		String::replaceAll( cur, "\n", "" );
		String::replaceAll( cur, "\t", "" );

		selectors.push_back( StyleSheetSelector( cur ) );
	}
}

}}} // namespace Zero::UI::CSS
