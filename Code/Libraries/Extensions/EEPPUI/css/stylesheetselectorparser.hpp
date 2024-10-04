#ifndef EE_UI_CSS_STYLESHEETSELECTORPARSER_HPP
#define EE_UI_CSS_STYLESHEETSELECTORPARSER_HPP

#include <eepp/ui/css/stylesheetselector.hpp>

namespace Zero { namespace UI { namespace CSS {

class ZeroShared StyleSheetSelectorParser {
  public:
	StyleSheetSelectorParser();

	explicit StyleSheetSelectorParser( String name );

	Array<StyleSheetSelector> selectors;
};

}}} // namespace Zero::UI::CSS
#endif
