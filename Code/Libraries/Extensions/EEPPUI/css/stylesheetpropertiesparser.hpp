#ifndef EE_UI_CSS_STYLESHEETPROPERTIESPARSER_HPP
#define EE_UI_CSS_STYLESHEETPROPERTIESPARSER_HPP

#include <eepp/core/string.hpp>
#include <eepp/ui/css/stylesheetproperty.hpp>
#include <eepp/ui/css/stylesheetvariable.hpp>
#include <map>
#include <vector>

namespace Zero { namespace UI { namespace CSS {

class ZeroShared StyleSheetPropertiesParser {
  public:
	StyleSheetPropertiesParser();

	explicit StyleSheetPropertiesParser( const String& propsstr );

	void parse( const String& propsstr );

	void print();

	const StyleSheetProperties& getProperties() const;

	const StyleSheetVariables& getVariables() const;

  protected:
	enum ReadState { ReadingPropertyName, ReadingPropertyValue, ReadingComment };

	ReadState mPrevRs;

	StyleSheetProperties mProperties;
	StyleSheetVariables mVariables;

	int readPropertyName( ReadState& rs, std::size_t pos, String& buffer,
						  const String& str );

	int readPropertyValue( ReadState& rs, std::size_t pos, String& buffer,
						   const String& str );

	int readComment( ReadState& rs, std::size_t pos, String& buffer, const String& str );

	void addProperty( String name, String value );
};

}}} // namespace Zero::UI::CSS

#endif
