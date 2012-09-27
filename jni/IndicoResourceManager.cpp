
#include "IndicoResourceManager.h"

namespace Indico
{
	template<> ResourceManager* Singleton<ResourceManager>::mSingleton = 0;

	ResourceManager::ResourceManager() : mResources(0,50)
	{

	}

	ResourceManager::~ResourceManager()
	{

	}

	bool ResourceManager::exists(const string& name)
	{
		if (mResources.has(name))
			return true;

		for (unsigned int i = 0; i < mLocators.size(); i++)
		{
			if (mLocators[i]->exists(name))
				return true;
		}

		return false;
	}

	bool ResourceManager::isType(const string &name, const TypeInfo& type)
	{
		if (mResources.has(name))
			return mResources[name]->type() == type;

		return false;
	}

	Resource* ResourceManager::get(const string& name, const TypeInfo& type)
	{
		if (mResources.has(name))
		{
			shared_ptr<Resource> res = mResources[name];

			if (!(res->type() == type))
				throw Exception("Wrong resource type, attempting to load resource of type '" + res->type().className() + "' as '" + type.className() + "'");

			return res.get();
		}

		shared_ptr<ResourceLoader> loader = shared_ptr<ResourceLoader>(0);
		for (unsigned int i = 0; i < mLoaders.size(); i++)
		{
			if (mLoaders[i]->loadType() == type)
			{
				loader = mLoaders[i];
				break;
			}
		}

		if (!loader.get())
			throw Exception("No loader for type '" + type.className() + "' is registered");

		for (unsigned int i = 0; i < mLocators.size(); i++)
		{
			if (mLocators[i]->exists(name))
			{
				shared_ptr<ResourceStream> stream = mLocators[i]->locate(name);

				if (!stream.get())
					throw Exception("Failed to create resource stream for resource '" + name + "'");

				shared_ptr<Resource> res = loader->load(*stream.get());

				if (!res.get())
					throw Exception("Failed to load resource from '" + name + "'");

				mResources[name] = res;

				return res.get();
			}
		}

		throw Exception("Resource '" + name + "' could not be found");
		return 0;
	}

	void ResourceManager::load(const string& name, const string& type)
	{
		get(name, type);
	}

	void ResourceManager::unload(const string& name)
	{
		if (mResources.has(name))
			mResources.remove(name);
	}

	void ResourceManager::clear()
	{
		mResources.clear();
	}

	void ResourceManager::addLoader(shared_ptr<ResourceLoader> loader)
	{
		if (!loader.get())
			throw Exception("Cannot add null loader");

		for (unsigned int i = 0; i < mLoaders.size(); i++)
		{
			if (mLoaders[i]->loadType() == loader->loadType())
				throw Exception("Cannot add another loader for resource type '" + loader->loadType().className() + "'");
		}

		mLoaders.add(loader);
	}

	void ResourceManager::removeLoader(shared_ptr<ResourceLoader> loader)
	{
		for (unsigned int i = 0; i < mLoaders.size(); i++)
		{
			if (mLoaders[i].get() == loader.get())
			{
				mLoaders.remove(i);
				return;
			}
		}
	}

	void ResourceManager::addLocator(shared_ptr<ResourceLocator> locator)
	{
		if (!locator.get())
			throw Exception("Cannot add null locator");

		mLocators.add(locator);
	}

	void ResourceManager::removeLocator(shared_ptr<ResourceLocator> locator)
	{
		for (unsigned int i = 0; i < mLocators.size(); i++)
		{
			if (mLocators[i].get() == locator.get())
			{
				mLocators.remove(i);
				return;
			}
		}
	}
}
