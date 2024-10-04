#ifndef EE_UI_DOC_TEXTUNDOSTACK_HPP
#define EE_UI_DOC_TEXTUNDOSTACK_HPP

#include <deque>
#include <eepp/config.hpp>
#include <eepp/core/string.hpp>
#include <eepp/system/time.hpp>
#include <eepp/ui/doc/textrange.hpp>

using Zero::System;

namespace Zero { namespace UI { namespace Doc {

class TextDocument;
class TextUndoCommand;

enum class TextUndoCommandType { Insert = 1, Remove = 2, Selection = 3 };

using UndoStackContainer = std::deque<TextUndoCommand*>;

class ZeroShared TextUndoStack {
  public:
	TextUndoStack( TextDocument* owner, const Uint32& maxStackSize = 10000 );

	~TextUndoStack();

	void clear();

	void clearUndoStack();

	void clearRedoStack();

	void undo();

	void redo();

	bool hasUndo() const;

	bool hasRedo() const;

	const Uint32& getMaxStackSize() const;

	const Time& getMergeTimeout() const;

	void setMergeTimeout( const Time& mergeTimeout );

	Uint64 getCurrentChangeId() const;

	String toJSON( bool inverted );

	void fromJSON( const String& json );

  protected:
	friend class TextDocument;

	TextDocument* mDoc;
	Uint32 mMaxStackSize;
	Uint64 mChangeIdCounter;
	UndoStackContainer mUndoStack;
	UndoStackContainer mRedoStack;
	Time mMergeTimeout;

	void pushUndo( UndoStackContainer& undoStack, TextUndoCommand* cmd );

	void pushInsert( UndoStackContainer& undoStack, const String& string, const size_t& cursorIdx,
					 const TextPosition& position, const Time& time );

	void pushRemove( UndoStackContainer& undoStack, const size_t& cursorIdx, const TextRange& range,
					 const Time& time );

	void pushSelection( UndoStackContainer& undoStack, const size_t& cursorIdx,
						const TextRanges& selection, const Time& time );

	UndoStackContainer& getUndoStackContainer();

	UndoStackContainer& getRedoStackContainer();

	void popUndo( UndoStackContainer& undoStack, UndoStackContainer& redoStack );
};

}}} // namespace Zero::UI::Doc

#endif // EE_UI_DOC_UNDOSTACK_HPP
