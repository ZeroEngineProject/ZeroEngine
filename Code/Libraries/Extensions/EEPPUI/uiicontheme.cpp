#include <eepp/core/core.hpp>
#include <eepp/ui/uiicontheme.hpp>

namespace EE { namespace UI {

UIIconTheme* UIIconTheme::New( const String& name ) {
	return eeNew( UIIconTheme, ( name ) );
}

UIIconTheme::~UIIconTheme() {
	for ( auto icon : mIcons )
		eeDelete( icon.second );
}

UIIconTheme::UIIconTheme( const String& name ) : mName( name ) {}

UIIconTheme* UIIconTheme::add( UIIcon* icon ) {
	auto iconExists = mIcons.find( icon->getName() );
	if ( iconExists != mIcons.end() )
		eeDelete( iconExists->second );
	mIcons[icon->getName()] = icon;
	return this;
}

UIIconTheme* UIIconTheme::add( const std::unordered_map<String, UIIcon*>& icons ) {
	mIcons.insert( icons.begin(), icons.end() );
	return this;
}

const String& UIIconTheme::getName() const {
	return mName;
}

UIIcon* UIIconTheme::getIcon( const String& name ) const {
	auto it = mIcons.find( name );
	return it != mIcons.end() ? it->second : nullptr;
}

}} // namespace EE::UI
