
#ifndef _INDICOSTRING_H_
#define _INDICOSTRING_H_

namespace Indico
{
	class string
	{
	public:
		static const string empty;

	private:
		char* mData;

	public:
		string();
		string(const char* str);
		string(const string& str);
		~string();

		const char* c_str() const;
		unsigned int length() const;

		bool contains(const string& sub) const;
		int find(const string& sub) const;
		string substr(unsigned int start) const;
		string substr(unsigned int start, unsigned int len) const;
		string trim() const;
		string trimNewlines() const;
		string toLower() const;
		int toi() const;
		float tof() const;

		static string parse(int val);
		static string parse(float val);

		char operator [] (unsigned int index) const;

		bool operator == (const string& other) const;
		bool operator != (const string& other) const;

		string& operator = (const string& other);
		string operator + (const string& other) const;
		string& operator += (const string& other);

		unsigned int hash(unsigned int seed) const;
	};

	static string operator + (const char* str1, const string& str2)
	{
		return string(str1) + str2;
	}
}

#endif
