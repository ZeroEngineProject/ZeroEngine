#ifndef EE_UI_MODEL_MODELSELECTION_HPP
#define EE_UI_MODEL_MODELSELECTION_HPP

#include <algorithm>
#include <eepp/ui/models/modelindex.hpp>
#include <functional>

namespace Zero { namespace UI { namespace Abstract {
class UIAbstractView;
}}} // namespace Zero::UI::Abstract

using Zero::UI::Abstract;

namespace Zero { namespace UI { namespace Models {

class ZeroShared ModelSelection {
  public:
	ModelSelection( UIAbstractView* view ) : mView( view ) {}

	int size() const { return mIndexes.size(); }
	bool isEmpty() const { return mIndexes.empty(); }
	bool contains( const ModelIndex& index ) const {
		return std::find( mIndexes.begin(), mIndexes.end(), index ) != mIndexes.end();
	}
	bool containsRow( int row ) const {
		for ( auto& index : mIndexes ) {
			if ( index.row() == row )
				return true;
		}
		return false;
	}

	void set( const ModelIndex& );
	void set( const Array<ModelIndex>& indexes, bool notify = true );
	void add( const ModelIndex& );
	void toggle( const ModelIndex& );
	bool remove( const ModelIndex& );
	void clear( bool notify = true );

	template <typename Callback> void forEachIndex( Callback callback ) {
		for ( auto& index : indexes() )
			callback( index );
	}

	template <typename Callback> void forEachIndex( Callback callback ) const {
		for ( auto& index : indexes() )
			callback( index );
	}

	Array<ModelIndex> indexes() const {
		Array<ModelIndex> indexes;
		for ( auto& index : mIndexes )
			indexes.push_back( index );
		return indexes;
	}

	ModelIndex first() const {
		if ( mIndexes.empty() )
			return {};
		return *mIndexes.begin();
	}

	void removeAllMatching( std::function<bool( ModelIndex const& )> filter );

	template <typename Function> void changeFromModel( Function f ) {
		{
			mDisableNotify = true;
			mNotifyPending = false;
			f( *this );
			mDisableNotify = false;
		}
		if ( mNotifyPending )
			notifySelectionChanged();
	}

  protected:
	UIAbstractView* mView;
	Array<ModelIndex> mIndexes;
	bool mDisableNotify{ false };
	bool mNotifyPending{ false };
	void notifySelectionChanged();
};

}}} // namespace Zero::UI::Models

#endif // EE_UI_MODEL_MODELSELECTION_HPP
