#ifndef EE_UI_CSS_STYLESHEET_HPP
#define EE_UI_CSS_STYLESHEET_HPP

#include <eepp/ui/css/elementdefinition.hpp>
#include <eepp/ui/css/keyframesdefinition.hpp>
#include <eepp/ui/css/mediaquery.hpp>
#include <eepp/ui/css/stylesheetstyle.hpp>
#include <memory>

namespace Zero { namespace UI { namespace CSS {

class ZeroShared StyleSheet {
  public:
	StyleSheet();

	void clear();

	void addStyle( std::shared_ptr<StyleSheetStyle> node );

	bool isEmpty() const;

	String print();

	void combineStyleSheet( const StyleSheet& styleSheet );

	std::shared_ptr<ElementDefinition> getElementStyles( UIWidget* element,
														 const bool& applyPseudo = false ) const;

	const Array<std::shared_ptr<StyleSheetStyle>>& getStyles() const;

	Array<std::shared_ptr<StyleSheetStyle>>
	getStylesFromSelector( const String& selector ) const;

	std::shared_ptr<StyleSheetStyle> getStyleFromSelector( const String& selector,
														   bool searchBySpecificity = false ) const;

	bool updateMediaLists( const MediaFeatures& features );

	bool isMediaQueryListEmpty() const;

	StyleSheetStyleVector getStyleSheetStyleByAtRule( const AtRuleType& atRuleType ) const;

	bool isKeyframesDefined( const String& keyframesName ) const;

	const KeyframesDefinition& getKeyframesDefinition( const String& keyframesName ) const;

	void addKeyframes( const KeyframesDefinition& keyframes );

	void addKeyframes( const KeyframesDefinitionMap& keyframesMap );

	const KeyframesDefinitionMap& getKeyframes() const;

	void invalidateCache();

	const Uint32& getMarker() const;

	void setMarker( const Uint32& marker );

	void removeAllWithMarker( const Uint32& marker );

	bool markerExists( const Uint32& marker ) const;

	StyleSheet getAllWithMarker( const Uint32& marker ) const;

	Array<std::shared_ptr<StyleSheetStyle>>
	findStyleFromSelectorName( const String& selector ) const;

	bool refreshCacheFromStyles( const Array<std::shared_ptr<StyleSheetStyle>>& styles );

	const Uint64& getVersion() const;

	StyleSheet& operator=( const StyleSheet& other );

  protected:
	Uint64 mVersion{ 1 };
	Uint32 mMarker{ 0 };
	Array<std::shared_ptr<StyleSheetStyle>> mNodes;
	HashMap<size_t, StyleSheetStyleVector> mNodeIndex;
	MediaQueryList::vector mMediaQueryList;
	KeyframesDefinitionMap mKeyframesMap;
	using ElementDefinitionCache = HashMap<size_t, std::shared_ptr<ElementDefinition>>;
	mutable ElementDefinitionCache mNodeCache;

	static size_t nodeHash( const String& tag, const String& id );

	void addMediaQueryList( MediaQueryList::ptr list );

	bool addStyleToNodeIndex( StyleSheetStyle* style );
};

}}} // namespace Zero::UI::CSS

#endif
