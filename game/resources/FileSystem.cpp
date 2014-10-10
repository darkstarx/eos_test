#include <resources/FileSystem.hpp>
#include <utils/log.hpp>

#ifndef RESOURCES_PATH
#ifndef ANDROID
	// ¬ андроиде расположение ресурсов определ€етс€ динамически при запуске приложени€
	#error "Ќеобходимо определить расположение ресурсов RESOURCES_PATH"
#endif //ANDROID
#define RESOURCES_PATH "/resources"
#endif

#ifndef ASSETS_PATH
#ifndef ANDROID
	// ¬ андроиде расположение ассетов не требуетс€, так как используетс€ ассетс-менеджер
	#error "Ќеобходимо определить расположение ресурсов ASSETS_PATH"
#endif //ANDROID
#define ASSETS_PATH "/assets"
#endif


FileSystem *FileSystem::m_instance = NULL;
FileSystemDestroyer FileSystem::m_destroyer;


/** @class FileSystemDestroyer */

FileSystemDestroyer::~FileSystemDestroyer()
{
	delete m_instance;
}



/** @class FileSystem */

FileSystem::FileSystem()
: m_resource_path(RESOURCES_PATH)
, m_assets_path(ASSETS_PATH)
{
	LOG(INFO) << "FileSystem created";
}


FileSystem::~FileSystem()
{
	LOG(INFO) << "FileSystem destroyed";
}


FileSystem& FileSystem::instance()
{
	if (!m_instance)
	{
		m_instance = new FileSystem();
		m_destroyer.init(m_instance);
	}
	return *m_instance;
}


bool FileSystem::load_from_file(const std::string& path, utils::bytearray& data)
{
	FILE *file;
#ifdef _WIN32
	if (fopen_s(&file, path.c_str(), "rb")) {
		return false;
	}
#else
	file = fopen(path.c_str(), "rb");
	if (!file) {
		return false;
	}
#endif
	
	fseek(file, 0, SEEK_END);
	const long int filesize = ftell(file);
	utils::bytearray buff(filesize);
	fseek(file, 0, SEEK_SET);
	if (fread(buff.get(), buff.size(), 1, file) == 1)
	{
		data = buff;
		fclose(file);
		return true;
	}
	else
	{
		fclose(file);
		return false;
	}
}


bool FileSystem::save_to_file(const std::string& path, utils::bytearray& data)
{
	FILE *file;
#ifdef _WIN32
	if (fopen_s(&file, path.c_str(), "wb")) {
		return false;
	}
#else
	file = fopen(path.c_str(), "wb");
	if (!file) {
		return false;
	}
#endif
	
	if (!file) return false;
	if (fwrite(data.get(), data.size(), 1, file) == 1)
	{
		fclose(file);
		return true;
	}
	else
	{
		fclose(file);
		return false;
	}
}


bool FileSystem::load_resource(const std::string& path, utils::bytearray& data)
{
	const std::string full_path = resources_path() + path;
	return load_from_file(full_path, data);
}