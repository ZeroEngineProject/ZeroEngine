#include <eepp/core/core.hpp>
#include <eepp/system/log.hpp>
#include <eepp/ui/css/propertyspecification.hpp>

namespace EE { namespace UI { namespace CSS {

SINGLETON_DECLARE_IMPLEMENTATION( PropertySpecification )

PropertySpecification::~PropertySpecification() {}

PropertyDefinition& PropertySpecification::registerProperty( const String& propertyVame,
															 const String& defaultValue,
															 bool inherited ) {
	PropertyDefinition* property = const_cast<PropertyDefinition*>( getProperty( propertyVame ) );

	if ( nullptr != property ) {
		Log::warning( "Property \"%s\" already registered.", propertyVame.c_str() );
		return *property;
	}

	PropertyDefinition* propDef = new PropertyDefinition( propertyVame, defaultValue, inherited );

	mProperties[propDef->getId()] = std::shared_ptr<PropertyDefinition>( propDef );

	for ( auto& sep : {"-", "_"} ) {
		if ( propDef->getName().find( sep ) != String::npos ) {
			String alias( propDef->getName() );
			String::replaceAll( alias, sep, "" );
			propDef->addAlias( alias );
		}
	}

	return *propDef;
}

const PropertyDefinition* PropertySpecification::getProperty( const Uint32& id ) const {
	auto it = mProperties.find( id );

	if ( it != mProperties.end() )
		return it->second.get();

	return nullptr;
}

const PropertyDefinition* PropertySpecification::getProperty( const String& name ) const {
	return getProperty( String::hash( name ) );
}

ShorthandDefinition&
PropertySpecification::registerShorthand( const String& name,
										  const std::vector<String>& properties,
										  const String& shorthandParserName ) {
	ShorthandDefinition* shorthand = const_cast<ShorthandDefinition*>( getShorthand( name ) );

	if ( nullptr != shorthand ) {
		Log::warning( "Shorthand %s already registered.", name.c_str() );
		return *shorthand;
	}

	ShorthandDefinition* shorthandDef =
		new ShorthandDefinition( name, properties, shorthandParserName );

	mShorthands[shorthandDef->getId()] = std::shared_ptr<ShorthandDefinition>( shorthandDef );

	return *shorthandDef;
}

const ShorthandDefinition* PropertySpecification::getShorthand( const String::HashType& id ) const {
	auto it = mShorthands.find( id );

	if ( it != mShorthands.end() )
		return it->second.get();

	return nullptr;
}

const ShorthandDefinition* PropertySpecification::getShorthand( const String& name ) const {
	return getShorthand( String::hash( name ) );
}

bool PropertySpecification::isShorthand( const String& name ) const {
	return getShorthand( name ) != nullptr;
}

bool PropertySpecification::isShorthand( const Uint32& id ) const {
	return getShorthand( id ) != nullptr;
}

const PropertyDefinition*
PropertySpecification::addPropertyAlias( Uint32 aliasId, const PropertyDefinition* propDef ) {
	if ( getProperty( aliasId ) == nullptr ) {
		auto it = mProperties.find( propDef->getId() );
		if ( it != mProperties.end() ) {
			mProperties[aliasId] = it->second;
		}
	}
	return propDef;
}

}}} // namespace EE::UI::CSS
