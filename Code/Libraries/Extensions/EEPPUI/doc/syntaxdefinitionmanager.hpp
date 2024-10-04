#ifndef EE_UI_DOC_SYNTAXSTYLEMANAGER_HPP
#define EE_UI_DOC_SYNTAXSTYLEMANAGER_HPP

#include <eepp/config.hpp>
#include <eepp/system/iostream.hpp>
#include <eepp/system/pack.hpp>
#include <eepp/system/singleton.hpp>
#include <eepp/ui/doc/syntaxdefinition.hpp>
#include <optional>
#include <vector>

using Zero::System;

namespace Zero { namespace UI { namespace Doc {

class ZeroShared SyntaxDefinitionManager {
	SINGLETON_DECLARE_HEADERS( SyntaxDefinitionManager )
  public:
	static std::pair<String, String> toCPP( const SyntaxDefinition& def );

	SyntaxDefinition& add( SyntaxDefinition&& syntaxStyle );

	const SyntaxDefinition& getPlainDefinition() const;

	Array<const SyntaxDefinition*>
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

	Array<String> getLanguageNames() const;

	Array<String> getExtensionsPatternsSupported() const;

	const SyntaxDefinition* getPtrByLanguageName( const String& name ) const;

	const SyntaxDefinition* getPtrByLSPName( const String& name ) const;

	const SyntaxDefinition* getPtrByLanguageId( const String::HashType& id ) const;

	bool loadFromStream( IOStream& stream, Array<String>* addedLangs );

	bool loadFromStream( IOStream& stream );

	bool loadFromFile( const String& fpath );

	bool loadFromMemory( const Uint8* data, const Uint32& dataSize );

	bool loadFromPack( Pack* Pack, const String& filePackPath );

	void loadFromFolder( const String& folderPath );

	const Array<SyntaxDefinition>& getDefinitions() const;

	/* empty = all */
	bool save( const String& path, const Array<SyntaxDefinition>& def = {} );

	void setLanguageExtensionsPriority( const std::map<String, String>& priorities );

	const std::map<String, String>& getLanguageExtensionsPriority() {
		return mPriorities;
	}

  protected:
	SyntaxDefinitionManager();

	Array<SyntaxDefinition> mDefinitions;
	std::map<String, String> mPriorities;

	std::optional<size_t> getLanguageIndex( const String& langName );
};

}}} // namespace Zero::UI::Doc

#endif // EE_UI_DOC_SYNTAXSTYLEMANAGER_HPP
