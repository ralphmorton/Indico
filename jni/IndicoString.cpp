
#include "IndicoString.h"
#include "IndicoException.h"

#include <string.h>
#include <stdlib.h>
#include <cstdio>

namespace Indico
{
	const string string::empty = string("");

	string::string()
	{
		mData = new char[1];
		mData[0] = '\0';
	}

	string::string(const char* str)
	{
		size_t len = strlen(str);
		mData = new char[len+1];
		mData[len] = '\0';
		memcpy(mData, str, len);
	}

	string::string(const string& str)
	{
		size_t len = strlen(str.mData);
		mData = new char[len+1];
		mData[len] = '\0';
		memcpy(mData, str.mData, len);
	}

	string::~string()
	{
		delete [] mData;
	}

	const char* string::c_str() const
	{
		return mData;
	}

	unsigned int string::length() const
	{
		return (unsigned int)strlen(mData);
	}

	bool string::contains(const string& sub) const
	{
		return find(sub) >= 0;
	}

	int string::find(const string& sub) const
	{
		unsigned int len = length();
		unsigned int slen = sub.length();

		if (slen > len) return -1;

		unsigned int start = 0;
		while (start <= (len-slen))
		{
			unsigned int ix = 0;

			for (ix; ix < slen; ix++)
			{
				if ((*this)[ix+start] != sub[ix])
				{
					start++;
					break;
				}
			}

			if (ix==slen) return start;
		}

		return -1;
	}

	string string::substr(unsigned int start) const
	{
		unsigned int len = length();

		if (start > len)
			throw Exception("String index out of bounds");

		return substr(start, len - start);
	}

	string string::substr(unsigned int start, unsigned int len) const
	{
		unsigned int mlen = strlen(mData);

		if (start+len > mlen)
			throw Exception("String index out of bounds");

		char* data = new char[len + 1];
		data[len] = '\0';
		memcpy(data, mData+start, len);

		string s;
		delete [] s.mData;
		s.mData = data;

		return s;
	}

	string string::trim() const
	{
		unsigned int len = length();

		if (len == 0)
			return *this;

		unsigned int start = 0;
		unsigned int end = length()-1;

		while ((mData[start] == ' ' || mData[start] == '\t') && start < len - 1)
			start++;

		while ((mData[end] == ' ' || mData[end] == '\t') && end > 0)
			end--;

		end++;

		unsigned int nlen = (end-start);
		char* data = new char[nlen+1];
		data[nlen] = '\0';
		memcpy(data, mData+start, nlen);

		string s;
		delete [] s.mData;
		s.mData = data;

		return s;
	}

	string string::trimNewlines() const
	{
		unsigned int len = length();

		if (len == 0)
			return *this;

		unsigned int start = 0;
		unsigned int end = length()-1;

		while ((mData[start] == '\r' || mData[start] == '\n') && start < len - 1)
			start++;

		while ((mData[end] == '\r' || mData[end] == '\n') && end > 0)
			end--;

		end++;

		unsigned int nlen = (end-start);
		char* data = new char[nlen+1];
		data[nlen] = '\0';
		memcpy(data, mData+start, nlen);

		string s;
		delete [] s.mData;
		s.mData = data;

		return s;
	}

	string string::toLower() const
	{
		int len = (int)length();

		string r;
		delete [] r.mData;
		r.mData = new char[len+1];
		r.mData[len] = '\0';

		int offset = 'Z'-'z';

		for (int i = -1; ++i < len;)
		{
			if (mData[i] >= 'A' && mData[i] <= 'Z')
				r.mData[i] = mData[i]-offset;
			else
				r.mData[i] = mData[i];
		}

		return r;
	}

	int string::toi() const
	{
		return atoi(mData);
	}

	float string::tof() const
	{
		return atof(mData);
	}

	string string::parse(int val)
	{
		char buf[11];
		memset(buf, 0, 11);

		sprintf(buf, "%i", val);

		return string(buf);
	}

	string string::parse(float val)
	{
		char buf[11];
		memset(buf, 0, 11);

		sprintf(buf, "%f", val);

		return string(buf);
	}

	char string::operator [] (unsigned int index) const
	{
		unsigned int len = strlen(mData);

		if (index >= len)
			throw Exception("String index out of bounds");

		return mData[index];
	}

	bool string::operator == (const string& other) const
	{
		return strcmp(mData, other.mData) == 0;
	}

	bool string::operator != (const string& other) const
	{
		return strcmp(mData, other.mData) != 0;
	}

	string& string::operator = (const string& other)
	{
		delete [] mData;
		unsigned int len = other.length();

		char* data = new char[len + 1];
		data[len] = '\0';
		memcpy(data, other.mData, len);

		mData = data;

		return *this;
	}

	string string::operator + (const string& other) const
	{
		size_t len = strlen(mData);
		size_t olen = strlen(other.mData);

		char* data = new char[len + olen + 1];
		data[len + olen] = '\0';
		memcpy(data, mData, len);
		memcpy(data+len, other.mData, olen);

		string s;
		delete [] s.mData;
		s.mData = data;

		return s;
	}

	string& string::operator += (const string& other)
	{
		size_t len = strlen(mData);
		size_t olen = strlen(other.mData);

		char* data = new char[len + olen + 1];
		data[len + olen] = '\0';
		memcpy(data, mData, len);
		memcpy(data+len, other.mData, olen);

		delete [] mData;
		mData = data;

		return *this;
	}

	unsigned int string::hash(unsigned int seed) const //Murmur2
	{
		const unsigned int m = 0x5bd1e995;
		const int r = 24;

		unsigned int len = length();
		unsigned int h = seed ^ len;

		const unsigned char* data = (const unsigned char*)mData;

		while(len >= 4)
		{
			unsigned int k = *(unsigned int*)data;

			k *= m;
			k ^= k >> r;
			k *= m;

			h *= m;
			h ^= k;

			data += 4;
			len -= 4;
		}

		switch(len)
		{
			case 3: h ^= data[2] << 16;
			case 2: h ^= data[1] << 8;
			case 1: h ^= data[0];
			h *= m;
		};

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}
}
