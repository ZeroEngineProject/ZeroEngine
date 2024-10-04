#ifndef EE_UI_CSS_STYLESHEETSELECTOR_HPP
#define EE_UI_CSS_STYLESHEETSELECTOR_HPP

#include <eepp/ui/css/stylesheetselectorrule.hpp>

namespace Zero { namespace UI {
class UIWidget;
}} // namespace Zero::UI

namespace Zero { namespace UI { namespace CSS {

class ZeroShared StyleSheetSelector {
  public:
	StyleSheetSelector();

	explicit StyleSheetSelector( const String& selectorName );

	const String& getName() const;

	const Uint32& getSpecificity() const;

	bool select( UIWidget* element, const bool& applyPseudo = true ) const;

	bool isCacheable() const;

	bool hasPseudoClasses() const;

	Array<UIWidget*> getRelatedElements( UIWidget* element, bool applyPseudo = true ) const;

	bool isStructurallyVolatile() const;

	const StyleSheetSelectorRule& getRule( const Uint32& index );

	const String& getSelectorId() const;

	const String& getSelectorTagName() const;

  protected:
	String mName;
	Uint32 mSpecificity;
	Array<StyleSheetSelectorRule> mSelectorRules;
	bool mCacheable;
	bool mStructurallyVolatile;

	void addSelectorRule( String& buffer,
						  StyleSheetSelectorRule::PatternMatch& curPatternMatch,
						  const StyleSheetSelectorRule::PatternMatch& newPatternMatch );

	void parseSelector( String selector );
};

}}} // namespace Zero::UI::CSS

#endif
