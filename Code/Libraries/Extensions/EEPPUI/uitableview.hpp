#ifndef EE_UI_UITABLEVIEW_HPP
#define EE_UI_UITABLEVIEW_HPP

#include <eepp/ui/abstract/uiabstracttableview.hpp>

using namespace EE::UI::Abstract;

namespace EE { namespace UI {

class ZeroShared UITableView : public UIAbstractTableView {
  public:
	static UITableView* New();

	Uint32 getType() const;

	bool isType( const Uint32& type ) const;

	virtual void drawChilds();

	virtual Node* overFind( const Vector2f& point );

	Float getMaxColumnContentWidth( const size_t& colIndex, bool bestGuess = false );

	virtual ModelIndex findRowWithText( const String& text, const bool& caseSensitive = false,
										const bool& exactMatch = false ) const;

  protected:
	Sizef mContentSize;

	UITableView();

	UITableView( const String& tag );

	virtual void createOrUpdateColumns( bool resetColumnData );

	void updateContentSize();

	void onColumnSizeChange( const size_t&, bool );

	virtual Uint32 onKeyDown( const KeyEvent& event );
};

}} // namespace EE::UI

#endif // EE_UI_UITABLEVIEW_HPP
