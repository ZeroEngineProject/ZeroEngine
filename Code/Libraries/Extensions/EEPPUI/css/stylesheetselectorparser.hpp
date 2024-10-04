#ifndef EE_UI_CSS_STYLESHEETSELECTORPARSER_HPP
#define EE_UI_CSS_STYLESHEETSELECTORPARSER_HPP

#include <eepp/ui/css/stylesheetselector.hpp>

namespace EE { namespace UI { namespace CSS {

class ZeroShared StyleSheetSelectorParser {
  public:
	StyleSheetSelectorParser();

	explicit StyleSheetSelectorParser( String name );

	std::vector<StyleSheetSelector> selectors;
};

}}} // namespace EE::UI::CSS
#endif
