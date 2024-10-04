#include <eepp/graphics/drawable.hpp>
#include <eepp/graphics/font.hpp>
#include <eepp/graphics/ninepatch.hpp>
#include <eepp/graphics/ninepatchmanager.hpp>
#include <eepp/graphics/sprite.hpp>
#include <eepp/graphics/statelistdrawable.hpp>
#include <eepp/graphics/textureatlas.hpp>
#include <eepp/graphics/textureatlasloader.hpp>
#include <eepp/graphics/textureatlasmanager.hpp>
#include <eepp/graphics/texturefactory.hpp>
#include <eepp/system/filesystem.hpp>
#include <eepp/system/log.hpp>
#include <eepp/ui/css/stylesheetparser.hpp>
#include <eepp/ui/uiicontheme.hpp>
#include <eepp/ui/uistate.hpp>
#include <eepp/ui/uitheme.hpp>
#include <eepp/ui/uithememanager.hpp>

namespace Zero { namespace UI {

static String elemNameFromSkin( const Array<String>& nameParts ) {
	String str;
	int lPart = (int)nameParts.size() - 1;

	for ( int i = 0; i < lPart; i++ ) {
		str += nameParts[i];

		if ( i != lPart - 1 ) {
			str += "_";
		}
	}

	return str;
}

UITheme* UITheme::New( const String& name, const String& abbr, Font* defaultFont ) {
	return eeNew( UITheme, ( name, abbr, defaultFont ) );
}

UITheme* UITheme::load( const String& name, const String& abbr,
						const String& textureAtlasPath, Font* defaultFont,
						const String& styleSheetPath ) {
	UITheme* theme = UITheme::New( name, abbr, defaultFont );

	CSS::StyleSheetParser styleSheetParser;

	if ( styleSheetParser.loadFromFile( styleSheetPath ) ) {
		theme->setStyleSheet( styleSheetParser.getStyleSheet() );
		theme->setStyleSheetPath( styleSheetPath );
	}

	if ( textureAtlasPath.empty() )
		return theme;

	TextureAtlasLoader tgl( textureAtlasPath );

	return loadFromTextureAtlas( theme, tgl.getTextureAtlas() );
}

UITheme* UITheme::loadFromTextureAtlas( UITheme* tTheme, Graphics::TextureAtlas* textureAtlas ) {
	eeASSERT( NULL != tTheme && NULL != textureAtlas );

	/** Themes use nearest filter by default, force the filter to the textures. */
	for ( Uint32 tC = 0; tC < textureAtlas->getTexturesCount(); tC++ ) {
		textureAtlas->getTexture( tC )->setFilter( Texture::Filter::Nearest );
	}

	Clock TE;

	tTheme->setTextureAtlas( textureAtlas );

	auto& resources = textureAtlas->getResources();
	String sAbbr( tTheme->getAbbr() + "_" );
	String sAbbrIcon( tTheme->getAbbr() + "_icon_" );
	std::map<String, UISkin*> skins;

	for ( auto& it : resources ) {
		TextureRegion* textureRegion = it.second;

		String name( textureRegion->getName() );

		if ( String::startsWith( name, sAbbrIcon ) ) {
			auto* icon = UIIcon::New( name.substr( sAbbrIcon.size() ) );
			icon->setSize( textureRegion->getPixelsSize().getWidth(), textureRegion );
			tTheme->getIconTheme()->add( icon );
		} else if ( String::startsWith( name, sAbbr ) ) {
			Array<String> dotParts = String::split( name, '.' );

			if ( dotParts.size() >= 3 && dotParts[dotParts.size() - 1] == "9" ) {
				String realName;

				for ( size_t i = 0; i < dotParts.size() - 2; i++ ) {
					realName += dotParts[i];

					if ( i != dotParts.size() - 3 ) {
						realName += ".";
					}
				}

				Array<String> nameParts = String::split( realName, '_' );

				Array<String> srcRect =
					String::split( dotParts[dotParts.size() - 2], '_' );
				int l = 0, t = 0, r = 0, b = 0;

				if ( srcRect.size() == 4 ) {
					String::fromString<int>( l, srcRect[0] );
					String::fromString<int>( t, srcRect[1] );
					String::fromString<int>( r, srcRect[2] );
					String::fromString<int>( b, srcRect[3] );
				}

				String skinName( elemNameFromSkin( nameParts ) );

				Drawable* drawable = NinePatchManager::instance()->add(
					NinePatch::New( textureRegion, l, t, r, b, realName ) );

				if ( skins.find( skinName ) == skins.end() )
					skins[skinName] = tTheme->add( UISkin::New( skinName ) );

				int stateNum = UIState::getStateNumber( nameParts[nameParts.size() - 1] );

				if ( -1 != stateNum )
					skins[skinName]->setStateDrawable( stateNum, drawable );
			} else {
				Array<String> nameParts = String::split( name, '_' );

				if ( nameParts.size() >= 3 ) {
					int lPart = nameParts.size() - 1;

					if ( UIState::isStateName( nameParts[lPart] ) ) {
						String skinName( elemNameFromSkin( nameParts ) );
						int stateNum = UIState::getStateNumber( nameParts[lPart] );

						if ( skins.find( skinName ) == skins.end() )
							skins[skinName] = tTheme->add( UISkin::New( skinName ) );

						if ( -1 != stateNum )
							skins[skinName]->setStateDrawable( stateNum, textureRegion );
					}
				}
			}
		}
	}

	Log::info( "UI Theme Loaded in: %4.3f ms ( from TextureAtlas )",
			   TE.getElapsedTimeAndReset().asMilliseconds() );

	return tTheme;
}

UITheme* UITheme::loadFromDirectroy( UITheme* tTheme, const String& Path,
									 const Float& pixelDensity ) {
	Clock TE;

	String RPath( Path );

	FileSystem::dirAddSlashAtEnd( RPath );

	if ( !FileSystem::isDirectory( RPath ) )
		return NULL;

	Graphics::TextureAtlas* tSG = Graphics::TextureAtlas::New( tTheme->getAbbr() );

	tTheme->setTextureAtlas( tSG );

	Array<String> resources = FileSystem::filesGetInPath( RPath );
	Array<String>::iterator it;
	String sAbbr( tTheme->getAbbr() + "_" );
	String sAbbrIcon( tTheme->getAbbr() + "_icon_" );
	std::map<String, UISkin*> skins;

	for ( it = resources.begin(); it != resources.end(); ++it ) {
		String fpath( RPath + ( *it ) );
		String name( FileSystem::fileRemoveExtension( *it ) );

		if ( !FileSystem::isDirectory( fpath ) ) {
			if ( String::startsWith( name, sAbbrIcon ) ) {
				auto* drawable =
					TextureRegion::New( TextureFactory::instance()->loadFromFile( fpath ), name );
				tSG->add( drawable );
				auto* icon = UIIcon::New( name.substr( sAbbrIcon.size() ) );
				icon->setSize( drawable->getPixelsSize().getWidth(), drawable );
				tTheme->getIconTheme()->add( icon );
			} else if ( String::startsWith( name, sAbbr ) ) {
				Array<String> dotParts = String::split( name, '.' );

				if ( dotParts.size() >= 3 && dotParts[dotParts.size() - 1] == "9" ) {
					String realName;

					for ( size_t i = 0; i < dotParts.size() - 2; i++ ) {
						realName += dotParts[i];

						if ( i != dotParts.size() - 3 ) {
							realName += ".";
						}
					}

					Array<String> nameParts = String::split( realName, '_' );

					Array<String> srcRect =
						String::split( dotParts[dotParts.size() - 2], '_' );
					int l = 0, t = 0, r = 0, b = 0;

					if ( srcRect.size() == 4 ) {
						String::fromString<int>( l, srcRect[0] );
						String::fromString<int>( t, srcRect[1] );
						String::fromString<int>( r, srcRect[2] );
						String::fromString<int>( b, srcRect[3] );
					}

					String skinName( elemNameFromSkin( nameParts ) );

					Drawable* drawable = NinePatchManager::instance()->add(
						NinePatch::New( TextureFactory::instance()->loadFromFile( fpath ), l, t, r,
										b, pixelDensity, realName ) );

					if ( skins.find( skinName ) == skins.end() )
						skins[skinName] = tTheme->add( UISkin::New( skinName ) );

					int stateNum = UIState::getStateNumber( nameParts[nameParts.size() - 1] );

					if ( -1 != stateNum )
						skins[skinName]->setStateDrawable( stateNum, drawable );
				} else {
					Array<String> nameParts = String::split( name, '_' );

					if ( nameParts.size() >= 3 ) {
						int lPart = nameParts.size() - 1;

						if ( UIState::isStateName( nameParts[lPart] ) ) {
							TextureRegion* textureRegion = tSG->add( TextureRegion::New(
								TextureFactory::instance()->loadFromFile( fpath ), name ) );

							String skinName( elemNameFromSkin( nameParts ) );
							int stateNum = UIState::getStateNumber( nameParts[lPart] );

							if ( skins.find( skinName ) == skins.end() )
								skins[skinName] = tTheme->add( UISkin::New( skinName ) );

							if ( -1 != stateNum )
								skins[skinName]->setStateDrawable( stateNum, textureRegion );
						}
					}
				}
			}
		}
	}

	if ( tSG->getCount() )
		TextureAtlasManager::instance()->add( tSG );
	else
		eeSAFE_DELETE( tSG );

	Log::info( "UI Theme Loaded in: %4.3f ms ( from path )",
			   TE.getElapsedTimeAndReset().asMilliseconds() );

	return tTheme;
}

UITheme* UITheme::loadFromDirectroy( const String& Path, const String& Name,
									 const String& NameAbbr, const Float& pixelDensity ) {
	return loadFromDirectroy( UITheme::New( Name, NameAbbr ), Path, pixelDensity );
}

UITheme* UITheme::loadFromTextureAtlas( Graphics::TextureAtlas* TextureAtlas,
										const String& Name, const String& NameAbbr ) {
	return loadFromTextureAtlas( UITheme::New( Name, NameAbbr ), TextureAtlas );
}

UITheme::UITheme( const String& name, const String& Abbr, Graphics::Font* defaultFont ) :
	ResourceManagerMulti<UISkin>(),
	mName( name ),
	mNameHash( String::hash( mName ) ),
	mAbbr( Abbr ),
	mTextureAtlas( NULL ),
	mDefaultFont( defaultFont ),
	mDefaultFontSize( PixelDensity::dpToPx( PixelDensity::getPixelDensity() > 1.4 ? 11 : 12 ) ),
	mIconTheme( UIIconTheme::New( name ) ) {}

UITheme::~UITheme() {
	eeSAFE_DELETE( mIconTheme );
}

const String& UITheme::getName() const {
	return mName;
}

void UITheme::setName( const String& name ) {
	mName = name;
	mNameHash = String::hash( mName );
}

const String::HashType& UITheme::getId() const {
	return mNameHash;
}

const String& UITheme::getAbbr() const {
	return mAbbr;
}

UISkin* UITheme::add( UISkin* Resource ) {
	return ResourceManagerMulti<UISkin>::add( Resource );
}

Graphics::TextureAtlas* UITheme::getTextureAtlas() const {
	return mTextureAtlas;
}

void UITheme::setTextureAtlas( Graphics::TextureAtlas* SG ) {
	mTextureAtlas = SG;
}

UIIcon* UITheme::getIconByName( const String& name ) {
	return mIconTheme->getIcon( name );
}

UISkin* UITheme::getSkin( const String& widgetName ) {
	return getByName( mAbbr + "_" + widgetName );
}

CSS::StyleSheet& UITheme::getStyleSheet() {
	return mStyleSheet;
}

const CSS::StyleSheet& UITheme::getStyleSheet() const {
	return mStyleSheet;
}

void UITheme::setStyleSheet( const CSS::StyleSheet& styleSheet ) {
	mStyleSheet = styleSheet;
}

const Float& UITheme::getDefaultFontSize() const {
	return mDefaultFontSize;
}

void UITheme::setDefaultFontSize( const Float& defaultFontSize ) {
	mDefaultFontSize = defaultFontSize;
}

UIIconTheme* UITheme::getIconTheme() const {
	return mIconTheme;
}

const String& UITheme::getStyleSheetPath() const {
	return mStyleSheetPath;
}

void UITheme::setStyleSheetPath( const String& styleSheetPath ) {
	mStyleSheetPath = styleSheetPath;
}

bool UITheme::reloadStyleSheet() {
	if ( mStyleSheetPath.empty() )
		return false;

	CSS::StyleSheetParser styleSheetParser;

	if ( styleSheetParser.loadFromFile( mStyleSheetPath ) ) {
		setStyleSheet( styleSheetParser.getStyleSheet() );
		return true;
	}

	return false;
}

Font* UITheme::getDefaultFont() const {
	return mDefaultFont;
}

void UITheme::setDefaultFont( Font* font ) {
	mDefaultFont = font;
}

}} // namespace Zero::UI
