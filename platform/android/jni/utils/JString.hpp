#ifndef ANDROID_UTILS_JSTRING_HPP
#define ANDROID_UTILS_JSTRING_HPP

#include <jni.h>
#include <string>


namespace utils
{
	
	std::string std_string(JNIEnv *env, jstring jstr)
	{
		const char *cstr = env->GetStringUTFChars(jstr, NULL);
		std::string str(cstr, env->GetStringUTFLength(jstr));
		env->ReleaseStringUTFChars(jstr, cstr);
		return str;
	}
	
}

#endif //ANDROID_UTILS_JSTRING_HPP
