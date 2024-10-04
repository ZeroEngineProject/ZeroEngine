#ifndef EE_UICUISELECTBUTTON_HPP
#define EE_UICUISELECTBUTTON_HPP

#include <eepp/ui/uipushbutton.hpp>

namespace EE { namespace UI {

class ZeroShared UISelectButton : public UIPushButton {
  public:
	static UISelectButton* New();

	static UISelectButton* NewWithTag( const String& tag );

	UISelectButton();

	explicit UISelectButton( const String& tag );

	virtual ~UISelectButton();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual bool isSelected() const;

	virtual void unselect();

	virtual void select();

	void toggleSelection();

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	virtual String getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex ) const;

	virtual std::vector<PropertyId> getPropertiesImplemented() const;

	virtual void setSelected( bool set );

	void setSelectOnClick( bool set );

	bool hasSelectOnClick() const;
  protected:
	Uint32 mSelectOnClickCbId{ 0 };

	virtual void onStateChange();
};

}} // namespace EE::UI

#endif
