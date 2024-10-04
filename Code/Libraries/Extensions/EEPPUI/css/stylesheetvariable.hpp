#ifndef EE_UI_CSS_STYLESHEETVARIABLE_HPP
#define EE_UI_CSS_STYLESHEETVARIABLE_HPP

#include <eepp/config.hpp>
#include <eepp/core/containers.hpp>
#include <eepp/core/string.hpp>
#include <unordered_map>

namespace EE { namespace UI { namespace CSS {

class ZeroShared StyleSheetVariable {
  public:
	StyleSheetVariable();

	explicit StyleSheetVariable( const String& name, const String& value );

	explicit StyleSheetVariable( const String& name, const String& value,
								 const Uint32& specificity );

	const String& getName() const;

	const String::HashType& getNameHash() const;

	const String& getValue() const;

	const String& value() const;

	const Uint32& getSpecificity() const;

	void setSpecificity( const Uint32& specificity );

	bool isEmpty() const;

	void setName( const String& name );

	void setValue( const String& value );

	bool operator==( const StyleSheetVariable& variable );

  protected:
	String mName;
	String::HashType mNameHash;
	String mValue;
	Uint32 mSpecificity;
};

typedef HashMap<Uint32, StyleSheetVariable> StyleSheetVariables;

}}} // namespace EE::UI::CSS

#endif // EE_UI_CSS_STYLESHEETVARIABLE_HPP
