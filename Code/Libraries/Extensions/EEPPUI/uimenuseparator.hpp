#ifndef EE_UICUISEPARATOR
#define EE_UICUISEPARATOR

#include "uiwidget.hpp"

namespace Zero { namespace UI {

class ZeroShared UIMenuSeparator : public UIWidget {
  public:
	static UIMenuSeparator* New();

	UIMenuSeparator();

	virtual ~UIMenuSeparator();

	virtual Uint32 getType() const;

	virtual bool isType( const Uint32& type ) const;

	virtual void setTheme( UITheme* Theme );

  protected:
};

}} // namespace Zero::UI

#endif
