#ifndef EE_UI_MODELS_FILESYSTEMMODEL_HPP
#define EE_UI_MODELS_FILESYSTEMMODEL_HPP

#include <array>
#include <atomic>
#include <memory>

#include <eepp/system/fileinfo.hpp>
#include <eepp/system/translator.hpp>
#include <eepp/ui/models/model.hpp>
#include <eepp/ui/uiicon.hpp>

namespace EE { namespace UI { namespace Models {

enum FileSystemEventType {
	/// Sent when a file is created or renamed
	Add = 1,
	/// Sent when a file is deleted or renamed
	Delete = 2,
	/// Sent when a file is modified
	Modified = 3,
	/// Sent when a file is moved
	Moved = 4
};

struct FileEvent {
	FileSystemEventType type;
	String directory;
	String filename;
	String oldFilename;

	FileEvent( const FileSystemEventType& action, const String& directory,
			   const String& filename, const String& oldFilename = "" ) :
		type( action ), directory( directory ), filename( filename ), oldFilename( oldFilename ) {}
};

class ZeroShared FileSystemModel : public Model {
  public:
	enum class Mode { DirectoriesOnly, FilesAndDirectories };

	struct DisplayConfig {
		DisplayConfig() {}
		DisplayConfig(
			bool sortByName, bool foldersFirst, bool ignoreHidden,
			const std::vector<String>& acceptedExtensions = {},
			std::function<bool( const String& filepath )> fileIsVisibleFn = nullptr ) :
			sortByName( sortByName ),
			foldersFirst( foldersFirst ),
			ignoreHidden( ignoreHidden ),
			acceptedExtensions( acceptedExtensions ),
			fileIsVisibleFn( fileIsVisibleFn ) {}
		bool sortByName{ true };
		bool foldersFirst{ true };
		bool ignoreHidden{ false };
		std::vector<String> acceptedExtensions;
		std::function<bool( const String& filepath )> fileIsVisibleFn;
		bool operator==( const DisplayConfig& other ) {
			return sortByName == other.sortByName && foldersFirst == other.foldersFirst &&
				   ignoreHidden == other.ignoreHidden &&
				   acceptedExtensions == other.acceptedExtensions;
		}
		bool operator!=( const DisplayConfig& other ) { return !( *this == other ); }
	};

	enum Column {
		Icon = 0,
		Name,
		Size,
		Owner,
		Group,
		Permissions,
		ModificationTime,
		Inode,
		Path,
		SymlinkTarget,
		Count,
	};

	struct ZeroShared Node {
	  public:
		Node( const String& rootPath, const FileSystemModel& model );

		Node( FileInfo&& info, Node* parent );

		const String& getName() const { return mName; }

		const String& getDisplayName() const { return mDisplayName; }

		Node* getParent() const { return mParent; }

		const FileInfo& info() const { return mInfo; }

		const String& fullPath() const;

		const String& getMimeType() const { return mMimeType; }

		size_t childCount() const { return mChildren.size(); }

		const Node& getChild( const size_t& index );

		void invalidate();

		bool inParentTree( Node* parent ) const;

		Node* findChildName( const String& name, const FileSystemModel& model,
							 bool forceRefresh = false );

		Int64 findChildRowFromInternalData( void* internalData, const FileSystemModel& model,
											bool forceRefresh = false );

		Int64 findChildRowFromName( const String& name, const FileSystemModel& model,
									bool forceRefresh = false );

		void refresh( const FileSystemModel& model );

		~Node();

		FileSystemModel::Node* childWithPathExists( const String& path );

		const Uint32& getHash() { return mHash; }

	  private:
		friend class FileSystemModel;

		Node() {}

		Node* createChild( const String& childName, const FileSystemModel& model );

		void rename( const FileInfo& file );

		friend class FileSystemModel;
		String mName;
		String mDisplayName;
		String mMimeType;
		Node* mParent{ nullptr };
		FileInfo mInfo;
		std::vector<Node*> mChildren;
		bool mHasTraversed{ false };
		bool mInfoDirty{ true };
		Uint32 mHash{ 0 };

		ModelIndex index( const FileSystemModel& model, int column ) const;

		void cleanChildren();

		void traverseIfNeeded( const FileSystemModel& );

		void refreshIfNeeded( const FileSystemModel& );

		bool fetchData( const String& fullPath );

		void updateMimeType();
	};

	static std::shared_ptr<FileSystemModel>
	New( const String& rootPath, const Mode& mode = Mode::FilesAndDirectories,
		 const DisplayConfig& displayConfig = DisplayConfig(), Translator* translator = nullptr );

	const Mode& getMode() const { return mMode; }

	const String& getRootPath() const;

	void setRootPath( const String& rootPath );

	Node* getNodeFromPath( String path, bool folderNode = false, bool invalidateTree = true );

	String_view getNodeRelativePath( const Node* ) const;

	void reload();

	void refresh();

	void update();

	const Node& node( const ModelIndex& index ) const;
	virtual size_t treeColumn() const { return Column::Name; }
	virtual size_t rowCount( const ModelIndex& = ModelIndex() ) const;
	virtual size_t columnCount( const ModelIndex& = ModelIndex() ) const;
	virtual String columnName( const size_t& column ) const;
	virtual Variant data( const ModelIndex&, ModelRole role = ModelRole::Display ) const;
	virtual ModelIndex parentIndex( const ModelIndex& ) const;
	virtual ModelIndex index( int row, int column = 0,
							  const ModelIndex& parent = ModelIndex() ) const;

	virtual UIIcon* iconFor( const Node& node, const ModelIndex& index ) const;

	void setMode( const Mode& mode );

	const DisplayConfig& getDisplayConfig() const;

	void setDisplayConfig( const DisplayConfig& displayConfig );

	const ModelIndex& getPreviouslySelectedIndex() const;

	void setPreviouslySelectedIndex( const ModelIndex& previouslySelectedIndex );

	bool handleFileEvent( const FileEvent& event );

	virtual bool classModelRoleEnabled() { return true; }

	~FileSystemModel();

  protected:
	std::atomic<bool> mInitOK;
	String mRootPath;
	String mRealRootPath;
	std::unique_ptr<Node> mRoot{ nullptr };
	Mode mMode{ Mode::FilesAndDirectories };
	DisplayConfig mDisplayConfig;
	std::array<String, Column::Count> mColumnNames;

	ModelIndex mPreviouslySelectedIndex{};

	Node& nodeRef( const ModelIndex& index ) const;

	FileSystemModel( const String& rootPath, const Mode& mode,
					 const DisplayConfig& displayConfig, Translator* translator );

	size_t getFileIndex( Node* parent, const FileInfo& file );

	bool handleFileEventLocked( const FileEvent& event );

	void setupColumnNames( Translator* translator );
};

class ZeroShared DiskDrivesModel : public Model {
  public:
	enum Column {
		Icon = 0,
		Name,
		Count,
	};

	static std::shared_ptr<DiskDrivesModel> create( const std::vector<String>& data );

	static std::shared_ptr<DiskDrivesModel> create();

	virtual ~DiskDrivesModel() {}

	virtual size_t rowCount( const ModelIndex& ) const { return mData.size(); }

	virtual size_t columnCount( const ModelIndex& ) const { return 2; }

	virtual String columnName( const size_t& index ) const {
		return index == 0 ? "Icon" : "Name";
	}

	UIIcon* diskIcon() const;

	virtual Variant data( const ModelIndex& index, ModelRole role = ModelRole::Display ) const;

  private:
	explicit DiskDrivesModel( const std::vector<String>& data ) : mData( data ) {}

	std::vector<String> mData;
};

}}} // namespace EE::UI::Models

#endif // EE_UI_MODELS_FILESYSTEMMODEL_HPP
