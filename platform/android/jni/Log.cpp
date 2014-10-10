#include <jni.h>
#include <utils/log.hpp>
#include <utils/JString.hpp>
#include <android_log.hpp>


extern "C"
{
	
	JNIEXPORT void JNICALL Java_com_socialquantum_framework_utils_LOG_nativeLog(JNIEnv *env, jobject obj,
		jint level, jstring _file, jint line, jstring _text)
	{
		jni::JString jfile(_file);
		jni::JString jtext(_text);
		utils::Log(static_cast<utils::log_level_t>(level), jfile.str().c_str(), line).stream() << jtext.str();
	}
	
	JNIEXPORT jboolean JNICALL Java_com_socialquantum_framework_utils_DLOG_nativeLogDebug(JNIEnv *env, jobject obj)
	{
		#ifndef NDEBUG
		return true;
		#else //NDEBUG
		return false;
		#endif//NDEBUG
	}
	
	JNIEXPORT void JNICALL Java_com_socialquantum_framework_utils_LOG_nativeSetLogDirectoryPath(JNIEnv *env, jclass, jstring path)
	{
		jni::JString jpath(path);
		utils::android_set_log_directory_path(jpath.str());
	}
	
	JNIEXPORT void JNICALL Java_com_socialquantum_framework_utils_LOG_nativeCreateLogFile(JNIEnv *env, jclass)
	{
		utils::android_create_log_file();
	}
	
	JNIEXPORT void JNICALL Java_com_socialquantum_framework_utils_LOG_nativeFlushLogFile(JNIEnv *env, jclass)
	{
		utils::android_flush_log_file();
	}
	
}
