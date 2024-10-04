#ifndef EE_UI_DOC_SYNTAXSTYLEMANAGER_HPP
#define EE_UI_DOC_SYNTAXSTYLEMANAGER_HPP

#include <eepp/config.hpp>
#include <eepp/system/iostream.hpp>
#include <eepp/system/pack.hpp>
#include <eepp/system/singleton.hpp>
#include <eepp/ui/doc/syntaxdefinition.hpp>
#include <optional>
#include <vector>

using namespace EE::System;

namespace EE { namespace UI { namespace Doc {

class ZeroShared SyntaxDefinitionManager {
	SINGLETON_DECLARE_HEADERS( SyntaxDefinitionManager )
  public:
	static std::pair<String, String> toCPP( const SyntaxDefinition& def );

	SyntaxDefinition& add( SyntaxDefinition&& syntaxStyle );

	const SyntaxDefinition& getPlainDefinition() const;

	std::vector<const SyntaxDefinition*>
	languagesThatSupportExtension( String extension ) const;

	bool extensionCanRepresentManyLanguages( String extension ) const;

	const SyntaxDefinition& getByExtension( const String& filePath,
											bool hFileAsCPP = false ) const;

	const SyntaxDefinition& getByHeader( const String& header, bool hFileAsCPP = false ) const;

	const SyntaxDefinition& find( const String& filePath, const String& header,
								  bool hFileAsCPP = false );

	const SyntaxDefinition& findFromString( const String& str ) const;

	SyntaxDefinition& getByExtensionRef( const String& filePath );

	const SyntaxDefinition& getByLanguageName( const String& name ) const;

	const SyntaxDefinition& getByLanguageIndex( const Uint32& index ) const;

	const SyntaxDefinition& getByLanguageNameInsensitive( String name ) const;

	const SyntaxDefinition& getByLSPName( const String& name ) const;

	const SyntaxDefinition& getByLanguageId( const String::HashType& id ) const;

	SyntaxDefinition& getByLanguageNameRef( const String& name );

	std::vector<String> getLanguageNames() const;

	std::vector<String> getExtensionsPatternsSupported() const;

	const SyntaxDefinition* getPtrByLanguageName( const String& name ) const;

	const SyntaxDefinition* getPtrByLSPName( const String& name ) const;

	const SyntaxDefinition* getPtrByLanguageId( const String::HashType& id ) const;

	bool loadFromStream( IOStream& stream, std::vector<String>* addedLangs );

	bool loadFromStream( IOStream& stream );

	bool loadFromFile( const String& fpath );

	bool loadFromMemory( const Uint8* data, const Uint32& dataSize );

	bool loadFromPack( Pack* Pack, const String& filePackPath );

	void loadFromFolder( const String& folderPath );

	const std::vector<SyntaxDefinition>& getDefinitions() const;

	/* empty = all */
	bool save( const String& path, const std::vector<SyntaxDefinition>& def = {} );

	void setLanguageExtensionsPriority( const std::map<String, String>& priorities );

	const std::map<String, String>& getLanguageExtensionsPriority() {
		return mPriorities;
	}

  protected:
	SyntaxDefinitionManager();

	std::vector<SyntaxDefinition> mDefinitions;
	std::map<String, String> mPriorities;

	std::optional<size_t> getLanguageIndex( const String& langName );
};

}}} // namespace EE::UI::Doc

#endif // EE_UI_DOC_SYNTAXSTYLEMANAGER_HPP
