
#include "IndicoLog.h"

namespace Indico
{
	Log::Log(LogLevel minLevel) : mMinLevel(minLevel) { }
	Log::~Log() { }

	void Log::add(LogDevice* device) { mDevices.add(device); }

	void Log::remove(LogDevice* device)
	{
		for (unsigned int i = 0; i < mDevices.size(); i++)
		{
			if (mDevices[i] == device)
			{
				mDevices.remove(i);
				return;
			}
		}
	}

	void Log::log(LogLevel level, const string& cat, const string& msg)
	{
		if (level < mMinLevel) return;

		for (unsigned int i = 0; i < mDevices.size(); i++)
			mDevices[i]->log(level, cat, msg);
	}

	LogLevel& Log::minLevel() { return mMinLevel; }
	LogLevel Log::minLevel() const { return mMinLevel; }

	void Log::operator() (LogLevel level, const string& cat, const string& msg)
	{
		log(level, cat, msg);
	}
}
