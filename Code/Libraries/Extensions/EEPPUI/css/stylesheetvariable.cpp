#include <algorithm>
#include <eepp/core/string.hpp>
#include <eepp/ui/css/stylesheetvariable.hpp>

namespace EE { namespace UI { namespace CSS {

StyleSheetVariable::StyleSheetVariable() : mNameHash( 0 ), mSpecificity( 0 ) {}

StyleSheetVariable::StyleSheetVariable( const String& name, const String& value ) :
	mName( String::toLower( String::trim( name ) ) ),
	mNameHash( String::hash( mName ) ),
	mValue( String::trim( value ) ),
	mSpecificity( 0 ) {}

StyleSheetVariable::StyleSheetVariable( const String& name, const String& value,
										const Uint32& specificity ) :
	mName( String::toLower( String::trim( name ) ) ),
	mNameHash( String::hash( mName ) ),
	mValue( String::trim( value ) ),
	mSpecificity( specificity ) {}

const String& StyleSheetVariable::getName() const {
	return mName;
}

const String& StyleSheetVariable::getValue() const {
	return mValue;
}

const String& StyleSheetVariable::value() const {
	return mValue;
}

const Uint32& StyleSheetVariable::getSpecificity() const {
	return mSpecificity;
}

void StyleSheetVariable::setSpecificity( const Uint32& specificity ) {
	mSpecificity = specificity;
}

bool StyleSheetVariable::isEmpty() const {
	return mName.empty();
}

void StyleSheetVariable::setName( const String& name ) {
	mName = name;
	mNameHash = String::hash( mName );
}

void StyleSheetVariable::setValue( const String& value ) {
	mValue = value;
}

bool StyleSheetVariable::operator==( const StyleSheetVariable& variable ) {
	return mNameHash == variable.mNameHash && mValue == variable.mValue;
}

const String::HashType& StyleSheetVariable::getNameHash() const {
	return mNameHash;
}

}}} // namespace EE::UI::CSS
