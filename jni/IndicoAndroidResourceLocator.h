
#ifndef _INDICOANDROIDRESOURCELOCATOR_H_
#define _INDICOANDROIDRESOURCELOCATOR_H_

#include "IndicoException.h"
#include "IndicoResource.h"
#include "IndicoResourceLocator.h"
#include "IndicoResourceStream.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"

#include <android/asset_manager.h>

namespace Indico
{
	class AndroidResourceStream : public ResourceStream
	{
	private:
		AAsset* mAsset;
		string mPath;
		int mLength, mRemaining;

	public:
		AndroidResourceStream(AAsset* asset, const string& path);
		~AndroidResourceStream();

		int length() const;

		int read(void* dest, unsigned int length);

		bool eos() const;

		const string& getResourceID() const;
	};

	class AndroidResourceLocator : public ResourceLocator
	{
	private:
		AAssetManager* mManager;

	public:
		AndroidResourceLocator(AAssetManager* manager);

		bool exists(const string& name);
		shared_ptr<ResourceStream> locate(const string& name);
	};
}

#endif
