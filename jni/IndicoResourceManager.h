
#ifndef _INDICORESOURCEMANAGER_H_
#define _INDICORESOURCEMANAGER_H_

#include "IndicoException.h"
#include "IndicoHashMap.h"
#include "IndicoResource.h"
#include "IndicoResourceLoader.h"
#include "IndicoResourceLocator.h"
#include "IndicoResourceStream.h"
#include "IndicoSingleton.h"
#include "IndicoSharedPointer.h"
#include "IndicoString.h"
#include "IndicoTypeInfo.h"
#include "IndicoVector.h"

namespace Indico
{
	class ResourceManager
	{
	private:
		Vector<shared_ptr<ResourceLocator> > mLocators;
		Vector<shared_ptr<ResourceLoader> > mLoaders;
		HashMap<string, shared_ptr<Resource> > mResources;

	private:
		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager& operator = (const ResourceManager& other) = delete;

	public:
		ResourceManager();
		~ResourceManager();

		bool exists(const string& name);
		bool isType(const string& name, const TypeInfo& type);

		Resource* get(const string& name, const TypeInfo& type);
		template <class T> T* get(const string& name)
		{
			return (T*)get(name, T::type_s());
		}

		void load(const string& name, const string& type);
		void unload(const string& name);
		void clear();

		void addLoader(shared_ptr<ResourceLoader> loader);
		void removeLoader(shared_ptr<ResourceLoader> loader);

		void addLocator(shared_ptr<ResourceLocator> locator);
		void removeLocator(shared_ptr<ResourceLocator> locator);
	};
}

#endif
