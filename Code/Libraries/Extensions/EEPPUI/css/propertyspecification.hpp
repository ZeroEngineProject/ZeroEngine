#ifndef EE_UI_CSS_PROPERTYSPECIFICATION_HPP
#define EE_UI_CSS_PROPERTYSPECIFICATION_HPP

#include <eepp/system/singleton.hpp>
#include <eepp/ui/css/propertydefinition.hpp>
#include <eepp/ui/css/shorthanddefinition.hpp>
#include <memory>
#include <unordered_map>

namespace Zero { namespace UI { namespace CSS {

class ZeroShared PropertySpecification {
	SINGLETON_DECLARE_HEADERS( PropertySpecification )
  public:
	~PropertySpecification();

	PropertyDefinition& registerProperty( const String& propertyVame,
										  const String& defaultValue, bool inherited );

	const PropertyDefinition* getProperty( const Uint32& id ) const;

	const PropertyDefinition* getProperty( const String& name ) const;

	ShorthandDefinition& registerShorthand( const String& name,
											const Array<String>& properties,
											const String& shorthandParserName );

	const ShorthandDefinition* getShorthand( const String::HashType& id ) const;

	const ShorthandDefinition* getShorthand( const String& name ) const;

	bool isShorthand( const String& name ) const;

	bool isShorthand( const Uint32& id ) const;

  protected:
	friend class PropertyDefinition;
	std::unordered_map<Uint32, std::shared_ptr<PropertyDefinition>> mProperties;
	std::unordered_map<Uint32, std::shared_ptr<ShorthandDefinition>> mShorthands;

	const PropertyDefinition* addPropertyAlias( Uint32 aliasId, const PropertyDefinition* propDef );
};

}}} // namespace Zero::UI::CSS

#endif
