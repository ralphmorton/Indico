
#ifndef _INDICOAPP_H_
#define _INDICOAPP_H_

namespace Indico
{
	class App
	{
	public:
		virtual ~App() { }

		virtual void init() = 0;
		virtual bool update(unsigned long long nanos) = 0;
		virtual void shutdown() = 0;
	};
}

#endif
