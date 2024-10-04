#ifndef EE_UI_MODELS_ITEMLISTMODEL_HPP
#define EE_UI_MODELS_ITEMLISTMODEL_HPP

#include <eepp/ui/models/model.hpp>
#include <memory>
#include <vector>

namespace Zero { namespace UI { namespace Models {

template <typename T> class ItemListModel final : public Model {
  public:
	static std::shared_ptr<ItemListModel> create( const Array<T>& data ) {
		return std::make_shared<ItemListModel<T>>( data );
	}

	explicit ItemListModel( const Array<T>& data ) : mData( data ) {}

	virtual ~ItemListModel() {}

	virtual size_t rowCount( const ModelIndex& ) const { return mData.size(); }

	virtual size_t columnCount( const ModelIndex& ) const { return 1; }

	virtual String columnName( const size_t& ) const { return "Data"; }

	virtual ModelIndex index( int row, int column, const ModelIndex& parent = ModelIndex() ) const {
		if ( row >= (int)rowCount( parent ) || column >= (int)columnCount( parent ) )
			return {};
		return Model::index( row, column, parent );
	}

	virtual Variant data( const ModelIndex& index, ModelRole role = ModelRole::Display ) const {
		if ( role == ModelRole::Display )
			return Variant( mData[index.row()] );
		return {};
	}

  private:
	const Array<T>& mData;
};

template <typename K, typename V> class ItemPairListModel final : public Model {
  public:
	static std::shared_ptr<ItemPairListModel> create( Array<std::pair<K, V>>& data ) {
		return std::make_shared<ItemPairListModel<K, V>>( data );
	}

	explicit ItemPairListModel( Array<std::pair<K, V>>& data ) : mData( data ) {}

	virtual ~ItemPairListModel() {}

	virtual size_t rowCount( const ModelIndex& ) const { return mData.size(); }

	virtual size_t columnCount( const ModelIndex& ) const { return 2; }

	virtual String columnName( const size_t& index ) const {
		eeASSERT( index < 2 );
		return mColumnNames[index];
	}

	virtual void setColumnName( const size_t& index, const String& name ) {
		eeASSERT( index < 2 );
		mColumnNames[index] = name;
	}

	virtual ModelIndex index( int row, int column, const ModelIndex& parent = ModelIndex() ) const {
		if ( row >= (int)rowCount( parent ) || column >= (int)columnCount( parent ) )
			return {};
		return Model::index( row, column, parent );
	}

	virtual Variant data( const ModelIndex& index, ModelRole role = ModelRole::Display ) const {
		if ( role == ModelRole::Display ) {
			switch ( index.column() ) {
				case 0:
					return Variant( mData[index.row()].first );
				case 1:
				default:
					return Variant( mData[index.row()].second );
			}
		}
		return {};
	}

	virtual bool isEditable( const ModelIndex& ) const { return mIsEditable; }

	void setIsEditable( bool isEditable ) { mIsEditable = isEditable; }

	// TODO: Fix this
	virtual void setData( const ModelIndex& index, const Variant& val ) {
		if ( mIsEditable && index.row() < (int)mData.size() ) {
			if ( index.column() == 0 ) {
				mData[index.row()].first = val.toString();
			} else {
				mData[index.row()].second = val.toString();
			}
		}
	}

  private:
	Array<std::pair<K, V>>& mData;
	Array<String> mColumnNames{ "Title", "Description" };
	bool mIsEditable{ false };
};

template <typename T> class ItemListOwnerModel final : public Model {
  public:
	static std::shared_ptr<ItemListOwnerModel> create( const Array<T>& data ) {
		return std::make_shared<ItemListOwnerModel<T>>( data );
	}

	explicit ItemListOwnerModel( const Array<T>& data ) : mData( data ) {}

	virtual ~ItemListOwnerModel() {}

	virtual size_t rowCount( const ModelIndex& ) const { return mData.size(); }

	virtual size_t columnCount( const ModelIndex& ) const { return 1; }

	virtual String columnName( const size_t& ) const { return "Data"; }

	virtual ModelIndex index( int row, int column, const ModelIndex& parent = ModelIndex() ) const {
		if ( row >= (int)rowCount( parent ) || column >= (int)columnCount( parent ) )
			return {};
		return Model::index( row, column, parent );
	}

	virtual Variant data( const ModelIndex& index, ModelRole role = ModelRole::Display ) const {
		if ( role == ModelRole::Display )
			return Variant( mData[index.row()] );
		return {};
	}

	virtual bool isEditable( const ModelIndex& ) const { return mIsEditable; }

	void setIsEditable( bool isEditable ) { mIsEditable = isEditable; }

	// TODO: Fix this
	virtual void setData( const ModelIndex& index, const Variant& val ) {
		if ( mIsEditable && index.row() < (int)mData.size() )
			mData[index.row()] = val.toString();
	}

  private:
	Array<T> mData;
	bool mIsEditable{ false };
};

template <typename K, typename V> class ItemPairListOwnerModel final : public Model {
  public:
	static std::shared_ptr<ItemPairListOwnerModel>
	create( const Array<std::pair<K, V>>& data ) {
		return std::make_shared<ItemPairListOwnerModel<K, V>>( data );
	}

	explicit ItemPairListOwnerModel( const Array<std::pair<K, V>>& data ) : mData( data ) {}

	virtual ~ItemPairListOwnerModel() {}

	virtual size_t rowCount( const ModelIndex& ) const { return mData.size(); }

	virtual size_t columnCount( const ModelIndex& ) const { return 2; }

	virtual String columnName( const size_t& index ) const {
		eeASSERT( index < 2 );
		return mColumnNames[index];
	}

	virtual void setColumnName( const size_t& index, const String& name ) {
		eeASSERT( index < 2 );
		mColumnNames[index] = name;
	}

	virtual ModelIndex index( int row, int column, const ModelIndex& parent = ModelIndex() ) const {
		if ( row >= (int)rowCount( parent ) || column >= (int)columnCount( parent ) )
			return {};
		return Model::index( row, column, parent );
	}

	virtual Variant data( const ModelIndex& index, ModelRole role = ModelRole::Display ) const {
		if ( role == ModelRole::Display ) {
			switch ( index.column() ) {
				case 0:
					return Variant( mData[index.row()].first );
				case 1:
				default:
					return Variant( mData[index.row()].second );
			}
		}
		return {};
	}

	virtual bool isEditable( const ModelIndex& ) const { return mIsEditable; }

	void setIsEditable( bool isEditable ) { mIsEditable = isEditable; }

	// TODO: Fix this
	virtual void setData( const ModelIndex& index, const Variant& val ) {
		if ( mIsEditable && index.row() < (int)mData.size() ) {
			if ( index.column() == 0 ) {
				mData[index.row()].first = val.toString();
			} else {
				mData[index.row()].second = val.toString();
			}
		}
	}

  private:
	Array<std::pair<K, V>> mData;
	Array<String> mColumnNames{ "Title", "Description" };
	bool mIsEditable{ false };
};

template <typename V> class ItemVectorListOwnerModel final : public Model {
  public:
	static std::shared_ptr<ItemVectorListOwnerModel>
	create( const size_t& columnCount, const Array<Array<V>>& data ) {
		return std::make_shared<ItemVectorListOwnerModel<V>>( columnCount, data );
	}

	explicit ItemVectorListOwnerModel( const size_t& columnCount,
									   const Array<Array<V>>& data ) :
		mData( data ) {
		mColumnNames.resize( columnCount );
	}

	virtual ~ItemVectorListOwnerModel() {}

	virtual size_t rowCount( const ModelIndex& ) const { return mData.size(); }

	virtual size_t columnCount( const ModelIndex& ) const { return mColumnNames.size(); }

	virtual String columnName( const size_t& index ) const {
		eeASSERT( index < mColumnNames.size() );
		return mColumnNames[index];
	}

	virtual void setColumnName( const size_t& index, const String& name ) {
		eeASSERT( index < mColumnNames.size() );
		mColumnNames[index] = name;
	}

	virtual ModelIndex index( int row, int column, const ModelIndex& parent = ModelIndex() ) const {
		if ( row >= (int)rowCount( parent ) || column >= (int)columnCount( parent ) )
			return {};
		return Model::index( row, column, parent );
	}

	virtual Variant data( const ModelIndex& index, ModelRole role = ModelRole::Display ) const {
		eeASSERT( index.row() < (Int64)mData.size() );
		eeASSERT( index.column() < (Int64)mData[index.row()].size() );
		if ( index.row() >= (Int64)mData.size() ||
			 index.column() >= (Int64)mData[index.row()].size() )
			return {};

		if ( role == ModelRole::Display )
			return Variant( mData[index.row()][index.column()] );

		return {};
	}

	virtual bool isEditable( const ModelIndex& ) const { return mIsEditable; }

	void setIsEditable( bool isEditable ) { mIsEditable = isEditable; }

	// TODO: Fix this
	virtual void setData( const ModelIndex& index, const Variant& val ) {
		if ( mIsEditable && index.row() < (int)mData.size() &&
			 index.column() < (int)mData[index.row()].size() ) {
			mData[index.row()][index.column()] = val.toString();
		}
	}

  private:
	Array<Array<V>> mData;
	Array<String> mColumnNames;
	bool mIsEditable{ false };
};

}}} // namespace Zero::UI::Models

#endif // EE_UI_MODELS_ITEMLISTMODEL_HPP
