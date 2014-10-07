#include <jni.h>
#include <Application.hpp>


extern "C"
{
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnCreate(JNIEnv *env, jobject obj)
	{
		app()->create();
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnDestroy(JNIEnv *env, jobject obj)
	{
		app()->destroy();
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnStart(JNIEnv *env, jobject obj)
	{
		app()->start();
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnResume(JNIEnv *env, jobject obj)
	{
		app()->resume();
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnPause(JNIEnv *env, jobject obj)
	{
		app()->pause();
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnStop(JNIEnv *env, jobject obj)
	{
		app()->stop();
	}
	
}
