
#include "IndicoAndroidResourceLocator.h"

namespace Indico
{
	AndroidResourceStream::AndroidResourceStream(AAsset* asset, const string& path) : mAsset(asset), mPath(path), mLength(AAsset_getLength(asset)), mRemaining(mLength)
	{

	}

	AndroidResourceStream::~AndroidResourceStream() { AAsset_close(mAsset); }

	int AndroidResourceStream::length() const { return mLength; }

	int AndroidResourceStream::read(void* dest, unsigned int length)
	{
		if (mRemaining <= 0)
			return 0;

		int r = AAsset_read(mAsset, dest, length);
		mRemaining = AAsset_getRemainingLength(mAsset);
		return r;
	}

	const string& AndroidResourceStream::getResourceID() const { return mPath; }

	bool AndroidResourceStream::eos() const { return mRemaining <= 0; }

	AndroidResourceLocator::AndroidResourceLocator(AAssetManager* manager) : mManager(manager) { }

	bool AndroidResourceLocator::exists(const string& name)
	{
		AAsset* a = AAssetManager_open(mManager, name.c_str(), AASSET_MODE_UNKNOWN);

		bool exists = a!=0;
		if (exists)
			AAsset_close(a);

		return exists;
	}

	shared_ptr<ResourceStream> AndroidResourceLocator::locate(const string& name)
	{
		AAsset* a = AAssetManager_open(mManager, name.c_str(), AASSET_MODE_UNKNOWN);

		if (!a)
			throw Exception("Asset '" + name + "' could not be opened");

		return shared_ptr<ResourceStream>((ResourceStream*)new AndroidResourceStream(a, name));
	}
}



