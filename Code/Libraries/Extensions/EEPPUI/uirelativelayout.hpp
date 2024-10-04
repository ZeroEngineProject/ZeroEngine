#ifndef UI_UIRELATIVELAYOUT_HPP
#define UI_UIRELATIVELAYOUT_HPP

#include <eepp/ui/uilayout.hpp>

namespace Zero { namespace UI {

class ZeroShared UIRelativeLayout : public UILayout {
  public:
	static UIRelativeLayout* New();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	UIRelativeLayout* add( UIWidget* widget );

	virtual void updateLayout();

  protected:
	UIRelativeLayout( const String& tagName );

	UIRelativeLayout();

	virtual Uint32 onMessage( const NodeMessage* Msg );

	void fixChildPos( UIWidget* widget );

	void fixChildSize( UIWidget* widget );
};

}} // namespace Zero::UI

#endif
