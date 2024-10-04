#ifndef EE_UI_DOC_DEFINITION_HPP
#define EE_UI_DOC_DEFINITION_HPP

#include <eepp/config.hpp>
#include <eepp/core/string.hpp>
#include <eepp/ui/doc/foldrangetype.hpp>
#include <eepp/ui/doc/syntaxcolorscheme.hpp>
#include <string>
#include <type_traits>
#include <vector>

namespace EE { namespace UI { namespace Doc {

template <typename T> static auto toSyntaxStyleTypeV( const std::vector<T>& s ) noexcept {
	if constexpr ( std::is_same_v<SyntaxStyleType, String> &&
				   std::is_same_v<T, String> ) {
		return std::vector<T>( s );
	} else if constexpr ( std::is_same_v<SyntaxStyleType, String::HashType> &&
						  std::is_same_v<T, String> ) {
		std::vector<SyntaxStyleType> v;
		v.reserve( s.size() );
		for ( const auto& sv : s )
			v.push_back( String::hash( sv ) );
		return v;
	} else
		return std::vector<SyntaxStyleType>{};
}

struct ZeroShared SyntaxPattern {
	static HashMap<SyntaxStyleType, String> SyntaxStyleTypeCache;

	using DynamicSyntax =
		std::function<String( const SyntaxPattern&, const String_view& )>;

	std::vector<String> patterns;
	std::vector<SyntaxStyleType> types;
	std::vector<String> typesNames;
	String syntax{ "" };
	DynamicSyntax dynSyntax;
	bool isRegEx{ false };

	SyntaxPattern( std::vector<String>&& _patterns, const String& _type,
				   const String& _syntax = "", bool isRegEx = false );

	SyntaxPattern( std::vector<String>&& _patterns, std::vector<String>&& _types,
				   const String& _syntax = "", bool isRegEx = false );

	SyntaxPattern( std::vector<String>&& _patterns, const String& _type,
				   DynamicSyntax&& _syntax, bool isRegEx = false );

	SyntaxPattern( std::vector<String>&& _patterns, std::vector<String>&& _types,
				   DynamicSyntax&& _syntax, bool isRegEx = false );

	bool hasSyntax() const { return !syntax.empty() || dynSyntax; }
};

class ZeroShared SyntaxDefinition {
  public:
	SyntaxDefinition();

	SyntaxDefinition( const String& languageName, std::vector<String>&& files,
					  std::vector<SyntaxPattern>&& patterns,
					  HashMap<String, String>&& symbols = {},
					  const String& comment = "", std::vector<String>&& headers = {},
					  const String& lspName = "" );

	const String& getLanguageName() const;

	String getLanguageNameForFileSystem() const;

	const String::HashType& getLanguageId() const;

	const std::vector<String>& getFiles() const;

	String getFileExtension() const;

	const std::vector<SyntaxPattern>& getPatterns() const;

	const String& getComment() const;

	const HashMap<String, SyntaxStyleType>& getSymbols() const;

	SyntaxStyleType getSymbol( const String& symbol ) const;

	/** Accepts lua patterns and file extensions. */
	SyntaxDefinition& addFileType( const String& fileType );

	SyntaxDefinition& addPattern( const SyntaxPattern& pattern );

	SyntaxDefinition& setPatterns( const std::vector<SyntaxPattern>& patterns );

	SyntaxDefinition& addPatternToFront( const SyntaxPattern& pattern );

	SyntaxDefinition& addPatternsToFront( const std::vector<SyntaxPattern>& patterns );

	SyntaxDefinition& addSymbol( const String& symbolName, const String& typeName );

	SyntaxDefinition& addSymbols( const std::vector<String>& symbolNames,
								  const String& typeName );

	SyntaxDefinition& setSymbols( const HashMap<String, SyntaxStyleType>& symbols );

	/** Sets the comment string used for auto-comment functionality. */
	SyntaxDefinition& setComment( const String& comment );

	const std::vector<String>& getHeaders() const;

	SyntaxDefinition& setHeaders( const std::vector<String>& headers );

	void clearPatterns();

	void clearSymbols();

	const String& getLSPName() const;

	SyntaxDefinition& setVisible( bool visible );

	bool isVisible() const;

	bool getAutoCloseXMLTags() const;

	SyntaxDefinition& setAutoCloseXMLTags( bool autoCloseXMLTags );

	SyntaxDefinition& setLanguageName( const String& languageName );

	SyntaxDefinition& setLSPName( const String& lSPName );

	std::vector<SyntaxPattern> getPatternsOfType( const SyntaxStyleType& type ) const;

	SyntaxDefinition& setFileTypes( const std::vector<String>& types );

	bool hasExtensionPriority() const;

	SyntaxDefinition& setExtensionPriority( bool hasExtensionPriority );

	HashMap<String, String> getSymbolNames() const;

	const Uint16& getLanguageIndex() const { return mLanguageIndex; }

	bool isCaseInsensitive() const;

	SyntaxDefinition& setCaseInsensitive( bool caseInsensitive );

	FoldRangeType getFoldRangeType() const;

	SyntaxDefinition& setFoldRangeType( FoldRangeType foldRangeType );

	std::vector<std::pair<Int64, Int64>> getFoldBraces() const;

	SyntaxDefinition& setFoldBraces( const std::vector<std::pair<Int64, Int64>>& foldBraces );

  protected:
	friend class SyntaxDefinitionManager;

	String mLanguageName;
	String::HashType mLanguageId;
	std::vector<String> mFiles;
	std::vector<SyntaxPattern> mPatterns;
	HashMap<String, SyntaxStyleType> mSymbols;
	HashMap<String, String> mSymbolNames;
	String mComment;
	std::vector<String> mHeaders;
	String mLSPName;
	Uint16 mLanguageIndex{ 0 };
	FoldRangeType mFoldRangeType{ FoldRangeType::Undefined };
	std::vector<std::pair<Int64, Int64>> mFoldBraces;
	bool mAutoCloseXMLTags{ false };
	bool mVisible{ true };
	bool mHasExtensionPriority{ false };
	bool mCaseInsensitive{ false };
};

}}} // namespace EE::UI::Doc

#endif // EE_UI_DOC_SYNTAXSTYLE_HPP
