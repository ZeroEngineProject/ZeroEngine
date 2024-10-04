#include <eepp/core/string.hpp>
#include <eepp/ui/uiclip.hpp>

namespace Zero { namespace UI {

ClipType UIClip::fromString( String str ) {
	String::toLowerInPlace( str );
	if ( str == "content-box" || str == "true" || str == "1" || str == "t" || str == "y" )
		return ClipType::ContentBox;
	else if ( str == "padding-box" )
		return ClipType::PaddingBox;
	else if ( str == "content-box" )
		return ClipType::BorderBox;
	return ClipType::None;
}

String UIClip::toString( const ClipType& clipType ) {
	switch ( clipType ) {
		case ClipType::ContentBox:
			return "content-box";
		case ClipType::PaddingBox:
			return "padding-box";
		case ClipType::BorderBox:
			return "border-box";
		case ClipType::None:
			return "none";
	}
	return "none";
}

UIClip::UIClip() {}

UIClip::UIClip( const ClipType& clipType ) : mClipType( clipType ) {}

const ClipType& UIClip::getClipType() const {
	return mClipType;
}

void UIClip::setClipType( const ClipType& clipType ) {
	mClipType = clipType;
}

}} // namespace Zero::UI
