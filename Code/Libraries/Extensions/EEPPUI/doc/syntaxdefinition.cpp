#include <eepp/core/memorymanager.hpp>
#include <eepp/core/string.hpp>
#include <eepp/ui/doc/syntaxdefinition.hpp>

namespace Zero { namespace UI { namespace Doc {

HashMap<SyntaxStyleType, String> SyntaxPattern::SyntaxStyleTypeCache = {};

template <typename SyntaxStyleType> void updateCache( const SyntaxPattern& ptrn ) {
	if constexpr ( std::is_same_v<SyntaxStyleType, String> ) {
		return;
	} else {
		for ( size_t i = 0; i < ptrn.typesNames.size(); i++ ) {
			if ( SyntaxStyleTypes::needsToBeCached( ptrn.types[i] ) ) {
				auto it = SyntaxPattern::SyntaxStyleTypeCache.find( ptrn.types[i] );
				if ( it == SyntaxPattern::SyntaxStyleTypeCache.end() )
					SyntaxPattern::SyntaxStyleTypeCache[ptrn.types[i]] = ptrn.typesNames[i];
			}
		}
	}
}

SyntaxDefinition::SyntaxDefinition() {}

SyntaxDefinition::SyntaxDefinition( const String& languageName,
									Array<String>&& files,
									Array<SyntaxPattern>&& patterns,
									HashMap<String, String>&& symbols,
									const String& comment, Array<String>&& headers,
									const String& lspName ) :
	mLanguageName( languageName ),
	mLanguageId( String::hash( String::toLower( languageName ) ) ),
	mFiles( std::move( files ) ),
	mPatterns( std::move( patterns ) ),
	mSymbolNames( std::move( symbols ) ),
	mComment( comment ),
	mHeaders( std::move( headers ) ),
	mLSPName( lspName.empty() ? String::toLower( mLanguageName ) : lspName ) {
	mSymbols.reserve( mSymbolNames.size() );
	for ( const auto& symbol : mSymbolNames )
		mSymbols.insert( { symbol.first, toSyntaxStyleType( symbol.second ) } );
}

const Array<String>& SyntaxDefinition::getFiles() const {
	return mFiles;
}

String SyntaxDefinition::getFileExtension() const {
	if ( !mFiles.empty() ) {
		String ext( mFiles[0] );
		String::replaceAll( ext, "%", "" );
		String::replaceAll( ext, "$", "" );
		String::replaceAll( ext, "?", "" );
		return ext;
	}
	return "";
}

Array<SyntaxPattern>
SyntaxDefinition::getPatternsOfType( const SyntaxStyleType& type ) const {
	Array<SyntaxPattern> patterns;
	for ( const auto& pattern : mPatterns ) {
		if ( pattern.types.size() == 1 && pattern.types[0] == type )
			patterns.emplace_back( pattern );
	}
	return patterns;
}

SyntaxDefinition& SyntaxDefinition::setFileTypes( const Array<String>& types ) {
	mFiles = types;
	return *this;
}

bool SyntaxDefinition::hasExtensionPriority() const {
	return mHasExtensionPriority;
}

SyntaxDefinition& SyntaxDefinition::setExtensionPriority( bool hasExtensionPriority ) {
	mHasExtensionPriority = hasExtensionPriority;
	return *this;
}

HashMap<String, String> SyntaxDefinition::getSymbolNames() const {
	return mSymbolNames;
}

bool SyntaxDefinition::isCaseInsensitive() const {
	return mCaseInsensitive;
}

SyntaxDefinition& SyntaxDefinition::setCaseInsensitive( bool caseInsensitive ) {
	mCaseInsensitive = caseInsensitive;
	return *this;
}

FoldRangeType SyntaxDefinition::getFoldRangeType() const {
	return mFoldRangeType;
}

SyntaxDefinition& SyntaxDefinition::setFoldRangeType( FoldRangeType foldRangeType ) {
	mFoldRangeType = foldRangeType;
	return *this;
}

Array<std::pair<Int64, Int64>> SyntaxDefinition::getFoldBraces() const {
	return mFoldBraces;
}

SyntaxDefinition&
SyntaxDefinition::setFoldBraces( const Array<std::pair<Int64, Int64>>& foldBraces ) {
	mFoldBraces = foldBraces;
	return *this;
}

const Array<SyntaxPattern>& SyntaxDefinition::getPatterns() const {
	return mPatterns;
}

const String& SyntaxDefinition::getComment() const {
	return mComment;
}

const HashMap<String, SyntaxStyleType>& SyntaxDefinition::getSymbols() const {
	return mSymbols;
}

SyntaxStyleType SyntaxDefinition::getSymbol( const String& symbol ) const {
	auto it = mSymbols.find( mCaseInsensitive ? String::toLower( symbol ) : symbol );
	if ( it != mSymbols.end() )
		return it->second;
	return SyntaxStyleEmpty();
}

SyntaxDefinition& SyntaxDefinition::addFileType( const String& fileType ) {
	mFiles.push_back( fileType );
	return *this;
}

SyntaxDefinition& SyntaxDefinition::addPattern( const SyntaxPattern& pattern ) {
	mPatterns.push_back( pattern );
	return *this;
}

SyntaxDefinition& SyntaxDefinition::setPatterns( const Array<SyntaxPattern>& patterns ) {
	mPatterns = patterns;
	return *this;
}

SyntaxDefinition& SyntaxDefinition::addPatternToFront( const SyntaxPattern& pattern ) {
	mPatterns.insert( mPatterns.begin(), pattern );
	return *this;
}

SyntaxDefinition&
SyntaxDefinition::addPatternsToFront( const Array<SyntaxPattern>& patterns ) {
	mPatterns.insert( mPatterns.begin(), patterns.begin(), patterns.end() );
	return *this;
}

SyntaxDefinition& SyntaxDefinition::addSymbol( const String& symbolName,
											   const String& typeName ) {
	mSymbols[symbolName] = toSyntaxStyleType( typeName );
	mSymbolNames[symbolName] = typeName;
	return *this;
}

SyntaxDefinition& SyntaxDefinition::addSymbols( const Array<String>& symbolNames,
												const String& typeName ) {
	for ( auto& symbol : symbolNames )
		addSymbol( symbol, typeName );
	return *this;
}

SyntaxDefinition&
SyntaxDefinition::setSymbols( const HashMap<String, SyntaxStyleType>& symbols ) {
	mSymbols = symbols;
	return *this;
}

SyntaxDefinition& SyntaxDefinition::setComment( const String& comment ) {
	mComment = comment;
	return *this;
}

const Array<String>& SyntaxDefinition::getHeaders() const {
	return mHeaders;
}

SyntaxDefinition& SyntaxDefinition::setHeaders( const Array<String>& headers ) {
	mHeaders = headers;
	return *this;
}

void SyntaxDefinition::clearPatterns() {
	mPatterns.clear();
}

void SyntaxDefinition::clearSymbols() {
	mSymbols.clear();
}

const String& SyntaxDefinition::getLSPName() const {
	return mLSPName;
}

SyntaxDefinition& SyntaxDefinition::setVisible( bool visible ) {
	mVisible = visible;
	return *this;
}

bool SyntaxDefinition::isVisible() const {
	return mVisible;
}

bool SyntaxDefinition::getAutoCloseXMLTags() const {
	return mAutoCloseXMLTags;
}

SyntaxDefinition& SyntaxDefinition::setAutoCloseXMLTags( bool autoCloseXMLTags ) {
	mAutoCloseXMLTags = autoCloseXMLTags;
	return *this;
}

SyntaxDefinition& SyntaxDefinition::setLanguageName( const String& languageName ) {
	mLanguageName = languageName;
	mLSPName = String::toLower( languageName );
	mLanguageId = String::hash( mLSPName );
	return *this;
}

SyntaxDefinition& SyntaxDefinition::setLSPName( const String& lSPName ) {
	mLSPName = lSPName;
	return *this;
}

const String& SyntaxDefinition::getLanguageName() const {
	return mLanguageName;
}

String SyntaxDefinition::getLanguageNameForFileSystem() const {
	String lang( mLanguageName );
	String::replaceAll( lang, " ", "" );
	String::replaceAll( lang, ".", "" );
	String::replaceAll( lang, "!", "" );
	String::replaceAll( lang, "[", "" );
	String::replaceAll( lang, "]", "" );
	String::replaceAll( lang, "+", "p" );
	String::replaceAll( lang, "#", "sharp" );
	String::toLowerInPlace( lang );
	return lang;
}

const String::HashType& SyntaxDefinition::getLanguageId() const {
	return mLanguageId;
}

SyntaxPattern::SyntaxPattern( Array<String>&& _patterns, const String& _type,
							  const String& _syntax, bool isRegEx ) :
	patterns( std::move( _patterns ) ),
	types( toSyntaxStyleTypeV( Array<String>{ _type } ) ),
	typesNames( { _type } ),
	syntax( _syntax ),
	isRegEx( isRegEx ) {
	updateCache<SyntaxStyleType>( *this );
}

SyntaxPattern::SyntaxPattern( Array<String>&& _patterns,
							  Array<String>&& _types, const String& _syntax,
							  bool isRegEx ) :
	patterns( std::move( _patterns ) ),
	types( toSyntaxStyleTypeV( _types ) ),
	typesNames( std::move( _types ) ),
	syntax( _syntax ),
	isRegEx( isRegEx ) {
	updateCache<SyntaxStyleType>( *this );
}

SyntaxPattern::SyntaxPattern( Array<String>&& _patterns, const String& _type,
							  DynamicSyntax&& _syntax, bool isRegEx ) :
	patterns( std::move( _patterns ) ),
	types( toSyntaxStyleTypeV( Array<String>{ _type } ) ),
	typesNames( { _type } ),
	dynSyntax( std::move( _syntax ) ),
	isRegEx( isRegEx ) {
	updateCache<SyntaxStyleType>( *this );
}

SyntaxPattern::SyntaxPattern( Array<String>&& _patterns,
							  Array<String>&& _types, DynamicSyntax&& _syntax,
							  bool isRegEx ) :
	patterns( std::move( _patterns ) ),
	types( toSyntaxStyleTypeV( _types ) ),
	typesNames( std::move( _types ) ),
	dynSyntax( std::move( _syntax ) ),
	isRegEx( isRegEx ) {
	updateCache<SyntaxStyleType>( *this );
}

}}} // namespace Zero::UI::Doc
