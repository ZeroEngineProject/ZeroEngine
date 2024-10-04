#ifndef EE_UICUICOMBOBOX_HPP
#define EE_UICUICOMBOBOX_HPP

#include <eepp/ui/uidropdownlist.hpp>

namespace Zero { namespace UI {

class ZeroShared UIComboBox : public UIWidget {
  public:
	static UIComboBox* New();

	UIComboBox();

	virtual ~UIComboBox();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual void setTheme( UITheme* Theme );

	UIListBox* getListBox();

	UIDropDownList* getDropDownList() const { return mDropDownList; }

	UINode* getButton() const { return mButton; }

	const String& getText();

	void setText( const String& text );

	virtual bool applyProperty( const StyleSheetProperty& attribute );

	virtual String getPropertyString( const PropertyDefinition* propertyDef,
										   const Uint32& propertyIndex = 0 ) const;

	virtual Array<PropertyId> getPropertiesImplemented() const;

	void loadFromXmlNode( const pugi::xml_node& node );

  protected:
	UIDropDownList* mDropDownList;
	UINode* mButton;

	Uint32 onMessage( const NodeMessage* Msg );

	void updateWidgets();

	virtual void onSizeChange();

	virtual void onPositionChange();

	virtual void onPaddingChange();

	virtual void onAutoSize();
};

}} // namespace Zero::UI

#endif
