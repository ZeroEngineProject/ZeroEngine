#ifndef EE_UI_DOC_FOLDRANGESERVICE_HPP
#define EE_UI_DOC_FOLDRANGESERVICE_HPP

#include <eepp/config.hpp>
#include <eepp/system/mutex.hpp>
#include <eepp/ui/doc/textrange.hpp>
#include <optional>
#include <unordered_map>

using Zero::System;

namespace Zero { namespace UI { namespace Doc {

class TextDocument;

class FoldRangeProvider {
	public:
		virtual bool foldingRangeProvider() const = 0;

		virtual void requestFoldRange() = 0;
};

class ZeroShared FoldRangeServive {
  public:
	FoldRangeServive( TextDocument* doc );

	bool canFold() const;

	void findRegions();

	void clear();

	bool empty();

	std::optional<TextRange> find( Int64 docIdx );

	void addFoldRegions( Array<TextRange> regions );

	bool isFoldingRegionInLine( Int64 docIdx );

	void removeFoldedRegion( const TextRange& region );

	void shiftFoldingRegions( Int64 fromLine, Int64 numLines );

	void setFoldingRegions( Array<TextRange> regions );

	FoldRangeProvider* getProvider() const;

	bool hasProvider() const;

	void setProvider( FoldRangeProvider* provider );

	bool isEnabled() const;

	void setEnabled( bool enabled );

  protected:
	TextDocument* mDoc;
	std::unordered_map<Int64, TextRange> mFoldingRegions;
	FoldRangeProvider* mProvider{ nullptr };
	Mutex mMutex;
	bool mEnabled{ true };
};

}}} // namespace Zero::UI::Doc

#endif // EE_UI_DOC_FOLDRANGESERVICE_HPP
