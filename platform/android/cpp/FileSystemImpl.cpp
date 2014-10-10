#include <resources/FileSystem.hpp>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <utils/JString.hpp>
#include <utils/log.hpp>


std::string FileSystem::resources_path()
{
	const jclass jc_filesystem = jni::get_class("com/eosproject/resources/FileSystem");
	ASSERT(jc_filesystem);
	
	const jmethodID jm_getrespath = jni::env()->GetStaticMethodID(jc_filesystem, "getResourcesPath", "()Ljava/lang/String;");
	ASSERT(jm_getrespath);
	
	const jstring result = static_cast<jstring>(jni::env()->CallStaticObjectMethod(jc_filesystem, jm_getrespath));
	ASSERT(result);
	
	return jni::JString(result).str();
}


bool FileSystem::load_asset(const std::string& path, utils::bytearray& data)
{
	const jclass jc_activity = jni::get_class("com/eosproject/eos/EOSActivity");
	ASSERT(jc_activity);
	
	const jmethodID jm_getassetmgr = jni::env()->GetStaticMethodID(jc_activity, "getAssetManager", "()Landroid/content/res/AssetManager;");
	ASSERT(jm_getassetmgr);
	
	const jobject assetManager = static_cast<jobject>(jni::env()->CallStaticObjectMethod(jc_activity, jm_getassetmgr));
	ASSERT(assetManager);
	
	AAssetManager* mgr = AAssetManager_fromJava(jni::env(), assetManager);
	ASSERT(mgr);
	
	AAsset* asset = AAssetManager_open(mgr, path.c_str(), AASSET_MODE_STREAMING);
	if (!asset) return false;
	
	const long int filesize = AAsset_getLength(asset);
	utils::bytearray buff(filesize);
	if (AAsset_read(asset, buff.get(), buff.size()) < 0)
	{
		AAsset_close(asset);
		return false;
	}
	else
	{
		data = buff;
		AAsset_close(asset);
		return true;
	}
}
