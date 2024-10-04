#include <algorithm>
#include <eepp/core.hpp>
#include <eepp/system/color.hpp>
#include <eepp/ui/css/shorthanddefinition.hpp>
#include <eepp/ui/css/stylesheetspecification.hpp>

using Zero::System;

namespace Zero { namespace UI { namespace CSS {

ShorthandDefinition* ShorthandDefinition::New( const String& name,
											   const Array<String>& properties,
											   const String& shorthandParserName ) {
	return eeNew( ShorthandDefinition, ( name, properties, shorthandParserName ) );
}

ShorthandDefinition::ShorthandDefinition( const String& name,
										  const Array<String>& properties,
										  const String& shorthandParserName ) :
	mName( name ),
	mFuncName( shorthandParserName ),
	mId( String::hash( name ) ),
	mProperties( properties ) {
	for ( auto& sep : {"-", "_"} ) {
		if ( mName.find( sep ) != String::npos ) {
			String alias( name );
			String::replaceAll( alias, sep, "" );
			addAlias( alias );
		}
	}
}

Array<StyleSheetProperty> ShorthandDefinition::parse( String value ) const {
	return StyleSheetSpecification::instance()->getShorthandParser( mFuncName )( this, value );
}

const String& ShorthandDefinition::getName() const {
	return mName;
}

const String::HashType& ShorthandDefinition::getId() const {
	return mId;
}

ShorthandId ShorthandDefinition::getShorthandId() const {
	return static_cast<ShorthandId>( mId );
}

const Array<String>& ShorthandDefinition::getProperties() const {
	return mProperties;
}

ShorthandDefinition& ShorthandDefinition::addAlias( const String& alias ) {
	mAliases.push_back( alias );
	mAliasesHash.push_back( String::hash( alias ) );
	return *this;
}

bool ShorthandDefinition::isAlias( const String& alias ) const {
	return isAlias( String::hash( alias ) );
}

bool ShorthandDefinition::isAlias( const String::HashType& id ) const {
	return std::find( mAliasesHash.begin(), mAliasesHash.end(), id ) != mAliasesHash.end();
}

bool ShorthandDefinition::isDefinition( const String& name ) const {
	return isDefinition( String::hash( name ) );
}

bool ShorthandDefinition::isDefinition( const String::HashType& id ) const {
	return mId == id || isAlias( id );
}

}}} // namespace Zero::UI::CSS
