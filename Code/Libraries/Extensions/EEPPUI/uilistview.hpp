#ifndef EE_UI_UILISTVIEW_HPP
#define EE_UI_UILISTVIEW_HPP

#include <eepp/ui/uitableview.hpp>

namespace Zero { namespace UI {

class ZeroShared UIListView : public UITableView {
  public:
	static UIListView* New();

	Uint32 getType() const;

	bool isType( const Uint32& type ) const;

	void setTheme( UITheme* Theme );

  protected:
	UIListView();
};

}} // namespace Zero::UI

#endif // EE_UI_UILISTVIEW_HPP
