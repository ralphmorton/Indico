
#include "IndicoTypeInfo.h"

namespace Indico
{
	TypeInfo::TypeInfo(const string& classname) : mClassName(classname) { }

	bool TypeInfo::operator == (const TypeInfo& other) const { return mClassName == other.mClassName; }
}
