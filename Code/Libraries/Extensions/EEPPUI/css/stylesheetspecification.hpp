#ifndef EE_UI_CSS_STYLESHEETSPECIFICATION_HPP
#define EE_UI_CSS_STYLESHEETSPECIFICATION_HPP

#include <eepp/core.hpp>
#include <eepp/system/functionstring.hpp>
#include <eepp/system/singleton.hpp>
#include <eepp/ui/css/drawableimageparser.hpp>
#include <eepp/ui/css/shorthanddefinition.hpp>
#include <functional>

namespace Zero { namespace UI {
class UIWidget;
}} // namespace Zero::UI

namespace Zero { namespace UI { namespace CSS {

class PropertySpecification;

typedef std::function<bool( const UIWidget* node, int a, int b, const FunctionString& data )>
	StyleSheetNodeSelector;

struct StructuralSelector {
	StructuralSelector( StyleSheetNodeSelector selector, int a = 0, int b = 0,
						const FunctionString& data = FunctionString::parse( "" ) ) :
		selector( selector ), a( a ), b( b ), data( data ) {}
	StyleSheetNodeSelector selector;
	int a;
	int b;
	FunctionString data;
};

class ZeroShared StyleSheetSpecification {
	SINGLETON_DECLARE_HEADERS( StyleSheetSpecification )
  public:
	StyleSheetSpecification();

	~StyleSheetSpecification();

	PropertyDefinition& registerProperty( const String& propertyVame,
										  const String& defaultValue, bool inherited = false );

	const PropertyDefinition* getProperty( const Uint32& id ) const;

	const PropertyDefinition* getProperty( const String& name ) const;

	ShorthandDefinition& registerShorthand( const String& name,
											const Array<String>& properties,
											const String& shorthandFuncName );

	const ShorthandDefinition* getShorthand( const Uint32& id ) const;

	const ShorthandDefinition* getShorthand( const String& name ) const;

	bool isShorthand( const Uint32& id ) const;

	bool isShorthand( const String& name ) const;

	void registerNodeSelector( const String& name, StyleSheetNodeSelector nodeSelector );

	StructuralSelector getStructuralSelector( const String& name );

	void registerShorthandParser( const String& name,
								  ShorthandParserFunc shorthandParserFunc );

	ShorthandParserFunc getShorthandParser( const String& name );

	DrawableImageParser& getDrawableImageParser();

  protected:
	PropertySpecification* mPropertySpecification;
	DrawableImageParser mDrawableImageParser;
	HashMap<String, ShorthandParserFunc> mShorthandParsers;
	HashMap<String, StyleSheetNodeSelector> mNodeSelectors;

	void registerDefaultShorthandParsers();

	void registerDefaultProperties();

	void registerDefaultNodeSelectors();
};

}}} // namespace Zero::UI::CSS

#endif
