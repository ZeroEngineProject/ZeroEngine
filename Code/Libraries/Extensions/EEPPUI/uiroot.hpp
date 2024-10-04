#ifndef EE_UI_UIROOT_HPP
#define EE_UI_UIROOT_HPP

#include "uiwidget.hpp"

namespace Zero { namespace UI {

class ZeroShared UIRoot : public UIWidget {
  public:
	static UIRoot* New();

	String getPropertyString( const PropertyDefinition* propertyDef,
								   const Uint32& propertyIndex ) const;

	bool applyProperty( const StyleSheetProperty& attribute );

	virtual Array<PropertyId> getPropertiesImplemented() const;

  protected:
	UIRoot();

	Color mDroppableHoveringColor{ Color::Transparent };
};

}} // namespace Zero::UI

#endif
