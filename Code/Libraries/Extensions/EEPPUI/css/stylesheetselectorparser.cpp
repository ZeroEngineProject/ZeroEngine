#include <eepp/ui/css/stylesheetselectorparser.hpp>

namespace EE { namespace UI { namespace CSS {

StyleSheetSelectorParser::StyleSheetSelectorParser() {}

StyleSheetSelectorParser::StyleSheetSelectorParser( String name ) {
	std::vector<String> sels = String::split( name, ',' );

	for ( auto it = sels.begin(); it != sels.end(); ++it ) {
		String cur = String::trim( *it );
		String::replaceAll( cur, "\n", "" );
		String::replaceAll( cur, "\t", "" );

		selectors.push_back( StyleSheetSelector( cur ) );
	}
}

}}} // namespace EE::UI::CSS
