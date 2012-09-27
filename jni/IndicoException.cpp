
#include "IndicoException.h"

namespace Indico
{
	Exception::Exception(const string& message) : mMessage(message) { }
	Exception::~Exception() { }
}
