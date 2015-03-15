#include <resources/FileSystem.hpp>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <utils/JString.hpp>
#include <utils/log.hpp>
#include <utils/bytearray.hpp>
#include <memory/Cache.hpp>


std::string FileSystem::resources_path()
{
	jni::JEnv jenv;
	
	const jclass jc_filesystem = jni::get_class("com/eosproject/resources/FileSystem");
	ASSERT(jc_filesystem);
	
	const jmethodID jm_getrespath = jni::env()->GetStaticMethodID(jc_filesystem, "getResourcesPath", "()Ljava/lang/String;");
	ASSERT(jm_getrespath);
	
	const jstring result = static_cast<jstring>(jni::env()->CallStaticObjectMethod(jc_filesystem, jm_getrespath));
	ASSERT(result);
	
	return jni::JString(result).str();
}


utils::bytearray_sptr_t FileSystem::load_asset(const std::string& path)
{
	// Ключ кэширования
	const std::string cache_key("a:" + path);
	
	// Пытаемся найти данные в кэше
	utils::bytearray_sptr_t ret = cache().get_obj<utils::bytearray_sptr_t>(cache_key);
	
	// Если нашли, возвращаем их
	if (ret) return ret;
	
	// Данные в кэше не найдены, пытаемся загрузить их из файла
	jni::JEnv jenv;
	
	const jclass jc_activity = jni::get_class("com/eosproject/eos/EOSActivity");
	ASSERT(jc_activity);
	
	const jmethodID jm_getassetmgr = jni::env()->GetStaticMethodID(jc_activity, "getAssetManager", "()Landroid/content/res/AssetManager;");
	ASSERT(jm_getassetmgr);
	
	const jobject assetManager = static_cast<jobject>(jni::env()->CallStaticObjectMethod(jc_activity, jm_getassetmgr));
	ASSERT(assetManager);
	
	AAssetManager* mgr = AAssetManager_fromJava(jni::env(), assetManager);
	ASSERT(mgr);
	
	AAsset* asset = AAssetManager_open(mgr, path.c_str(), AASSET_MODE_STREAMING);
	if (!asset) {
		LOG(ERR) << "Не удалось открыть файл " << path;
		return utils::bytearray_sptr_t();
	}
	
	const long int filesize = AAsset_getLength(asset);
	utils::bytearray buff(filesize);
	if (AAsset_read(asset, buff.get(), buff.size()) < 0)
	{
		AAsset_close(asset);
		LOG(ERR) << "Не удалось прочитать файл " << path;
		return utils::bytearray_sptr_t();
	}
	else
	{
		AAsset_close(asset);
		// Кэшируем загруженные данные
		ret.reset(new utils::bytearray(buff));
		cache().cache_obj(cache_key, ret);
		// Возвращаем загруженные данные
		return ret;
	}
}
