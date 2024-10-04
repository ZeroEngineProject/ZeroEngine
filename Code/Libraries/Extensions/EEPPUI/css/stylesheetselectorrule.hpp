#ifndef STYLESHEETSELECTORRULE_HPP
#define STYLESHEETSELECTORRULE_HPP

#include <eepp/core.hpp>
#include <eepp/ui/css/stylesheetspecification.hpp>
#include <stdint.h>

namespace Zero { namespace UI {
class UIWidget;
}} // namespace Zero::UI

namespace Zero { namespace UI { namespace CSS {

class ZeroShared StyleSheetSelectorRule {
  public:
	enum TypeIdentifier {
		TAG = 0,
		GLOBAL = '*',
		CLASS = '.',
		ID = '#',
		PSEUDO_CLASS = ':',
		STRUCTURAL_PSEUDO_CLASS = ':'
	};

	enum SelectorType {
		TagName = 1 << 0,
		Id = 1 << 1,
		Class = 1 << 2,
		PseudoClass = 1 << 3,
		StructuralPseudoClass = 1 << 4
	};

	enum SpecificityVal {
		SpecificityImportant = UINT32_MAX,
		SpecificityInline = UINT32_MAX - 1,
		SpecificityId = 1000000,
		SpecificityClass = 100000,
		SpecificityTag = 10000,
		SpecificityPseudoClass = 100,
		SpecificityStructuralPseudoClass = 50,
		SpecificityGlobal = 1
	};

	enum PatternMatch {
		ANY = '*',
		DESCENDANT = ' ',
		CHILD = '>',
		DIRECT_SIBLING = '+',
		SIBLING = '~',
		PREVIOUS_SIBLING = '|',
	};

	StyleSheetSelectorRule( const String& selectorFragment, PatternMatch mPatternMatch );

	void pushSelectorTypeIdentifier( TypeIdentifier selectorTypeIdentifier, String name );

	void parseFragment( const String& selectorFragment );

	const PatternMatch& getPatternMatch() const { return mPatternMatch; }

	const int& getSpecificity() const { return mSpecificity; }

	bool matches( UIWidget* element, const bool& applyPseudo = true ) const;

	bool hasClass( const String& cls ) const;

	bool hasPseudoClasses() const;

	bool hasPseudoClass( const String& cls ) const;

	const Array<String>& getPseudoClasses() const;

	bool hasStructuralPseudoClasses() const;

	const Array<String>& getStructuralPseudoClasses() const;

	bool hasStructuralPseudoClass( const String& cls ) const;

	const String& getTagName() const;

	const String& getId() const;

  protected:
	int mSpecificity;
	PatternMatch mPatternMatch;
	String mTagName;
	String mId;
	Array<String> mClasses;
	Array<String> mPseudoClasses;
	Array<String> mStructuralPseudoClasses;
	Array<StructuralSelector> mStructuralSelectors;
	Uint32 mRequirementFlags;
};

}}} // namespace Zero::UI::CSS

#endif
