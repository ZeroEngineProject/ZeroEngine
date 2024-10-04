#ifndef EE_UI_CSS_SHORTHANDDEFINITION_HPP
#define EE_UI_CSS_SHORTHANDDEFINITION_HPP

#include <eepp/config.hpp>
#include <eepp/core/string.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <functional>

namespace Zero { namespace UI { namespace CSS {

enum class ShorthandId : Uint32 {
	Margin = String::hash( "margin" ),
	Padding = String::hash( "padding" ),
	Transition = String::hash( "transition" ),
	Background = String::hash( "background" ),
	Foreground = String::hash( "foreground" ),
	BackgroundPosition = String::hash( "background-position" ),
	ForegroundPosition = String::hash( "foreground-position" ),
	LayoutMargin = String::hash( "layout-margin" ),
	LayoutMarginUnderscore = String::hash( "layout_margin" ),
	RotationOriginPoint = String::hash( "rotation-origin-point" ),
	ScaleOriginPoint = String::hash( "scale-origin-point" ),
	BorderColor = String::hash( "border-color" ),
	BorderWidth = String::hash( "border-width" ),
	BorderRadius = String::hash( "border-radius" ),
	MinSize = String::hash( "min-size" ),
	MaxSize = String::hash( "max-size" )
};

typedef std::function<Array<StyleSheetProperty>( const ShorthandDefinition* shorthand,
													   String value )>
	ShorthandParserFunc;

class ZeroShared ShorthandDefinition {
  public:
	static ShorthandDefinition* New( const String& name,
									 const Array<String>& properties,
									 const String& shorthandParserName );

	ShorthandDefinition( const String& name, const Array<String>& properties,
						 const String& shorthandFuncName );

	Array<StyleSheetProperty> parse( String value ) const;

	const String& getName() const;

	const String::HashType& getId() const;

	ShorthandDefinition& addAlias( const String& alias );

	bool isAlias( const String& alias ) const;

	bool isAlias( const String::HashType& id ) const;

	bool isDefinition( const String& name ) const;

	bool isDefinition( const String::HashType& id ) const;

	ShorthandId getShorthandId() const;

	const Array<String>& getProperties() const;

  protected:
	String mName;
	String mFuncName;
	String::HashType mId;
	Array<String> mAliases;
	Array<String::HashType> mAliasesHash;
	Array<String> mProperties;
};

}}} // namespace Zero::UI::CSS

#endif
