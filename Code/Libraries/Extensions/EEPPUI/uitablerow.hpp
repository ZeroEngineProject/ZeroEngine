#ifndef EE_UI_UITABLEROW_HPP
#define EE_UI_UITABLEROW_HPP

#include <eepp/ui/models/modelindex.hpp>
#include "uiwidget.hpp"

using Zero::UI::Models;

namespace Zero { namespace UI {

class ZeroShared UITableRow : public UIWidget {
  public:
	static UITableRow* New( const String& tag ) { return eeNew( UITableRow, ( tag ) ); }

	static UITableRow* New() { return eeNew( UITableRow, () ); }

	ModelIndex getCurIndex() const { return mCurIndex; }

	void setCurIndex( const ModelIndex& curIndex ) { mCurIndex = curIndex; }

	void setTheme( UITheme* Theme ) {
		UIWidget::setTheme( Theme );
		setThemeSkin( Theme, "tablerow" );
		onThemeLoaded();
	}

  protected:
	UITableRow( const String& tag ) : UIWidget( tag ) { applyDefaultTheme(); }

	UITableRow() : UIWidget( "table::row" ) {}

	static Event::EventType isMouseEvent( Uint32 msg ) {
		switch ( msg ) {
			case NodeMessage::MouseClick:
				return Event::MouseClick;
			case NodeMessage::MouseDoubleClick:
				return Event::MouseDoubleClick;
			case NodeMessage::MouseDown:
				return Event::MouseDown;
			case NodeMessage::MouseUp:
				return Event::MouseUp;
		}
		return Event::NoEvent;
	}

	virtual Uint32 onMessage( const NodeMessage* msg ) {
		EventDispatcher* eventDispatcher = getEventDispatcher();
		Node* mouseDownNode = eventDispatcher->getMouseDownNode();
		Node* draggingNode = eventDispatcher->getNodeDragging();
		auto eventType = isMouseEvent( msg->getMsg() );
		if ( eventType != Event::NoEvent &&
			 ( mouseDownNode == nullptr || mouseDownNode == this || isParentOf( mouseDownNode ) ) &&
			 draggingNode == nullptr ) {
			sendMouseEvent( eventType, eventDispatcher->getMousePos(), msg->getFlags() );
		}
		return 0;
	}

	ModelIndex mCurIndex;
};

}} // namespace Zero::UI

#endif // EE_UI_UITABLEROW_HPP
