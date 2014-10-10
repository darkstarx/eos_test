#ifndef JNI_JSTRING_HPP
#define JNI_JSTRING_HPP
#include "JNI.hpp"


namespace jni
{

	class JString
	{
	public:
		JString(const JString&) = delete;
		JString& operator =(const JString&) = delete;
		
		explicit JString(jstring jstr, bool owner = true)
		: m_owned(owner)
		, m_jstr(jstr)
		{}
		
		explicit JString(const char *cstr)
		: m_owned(true)
		, m_jstr(jni::env()->NewStringUTF(cstr))
		{}
		
		explicit JString(const std::string &str)
		: m_owned(true)
		, m_jstr(jni::env()->NewStringUTF(str.c_str()))
		{}
		
		~JString()
		{
			if (m_owned) env()->DeleteLocalRef(m_jstr);
		}
		
		inline operator jstring() const { return j_str(); }
		
		inline jstring j_str() const { return m_jstr; }
		
		inline jsize size() const { return env()->GetStringLength(m_jstr); }
		
		inline operator std::string() { return str(); }
		
		std::string str()
		{
			const char *cstr = env()->GetStringUTFChars(m_jstr, NULL);
			ASSERT(cstr);
			std::string str(cstr, env()->GetStringUTFLength(m_jstr));
			env()->ReleaseStringUTFChars(m_jstr, cstr);
			return str;
		}
		
	private:
		bool m_owned;
		jstring m_jstr;
	};
	
}

#endif //JNI_JSTRING_HPP
