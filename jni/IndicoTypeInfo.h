
#ifndef _INDICOTYPEINFO_H_
#define _INDICOTYPEINFO_H_

#include "IndicoString.h"

namespace Indico
{
	class TypeInfo
	{
	private:
		string mClassName;

	public:
		TypeInfo(const string& classname);

		bool operator == (const TypeInfo& other) const;

		inline const string& className() const { return mClassName; }
	};
}

#define TYPED_DECL()								\
private:											\
	static Indico::TypeInfo msType;					\
public:												\
	static const Indico::TypeInfo& type_s();		\
	virtual const Indico::TypeInfo& type() const;

#define TYPED_DEF(classname) 												\
Indico::TypeInfo classname::msType(#classname);								\
const Indico::TypeInfo& classname::type_s() { return classname::msType;  }	\
const Indico::TypeInfo& classname::type() const { return classname::msType; }

#endif
