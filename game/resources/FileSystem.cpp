#include <resources/FileSystem.hpp>
#include <utils/bytearray.hpp>
#include <memory/Cache.hpp>
#include <errno.h>

#ifndef RESOURCES_PATH
#ifndef ANDROID
	// � �������� ������������ �������� ������������ ����������� ��� ������� ����������
#ifndef _WIN32
	#error "���������� ���������� ������������ �������� RESOURCES_PATH"
#endif
#endif //ANDROID
#define RESOURCES_PATH "resources/"
#endif

#ifndef ASSETS_PATH
#ifndef ANDROID
#ifndef _WIN32
// � �������� ������������ ������� �� ���������, ��� ��� ������������ ������-��������
	#error "���������� ���������� ������������ �������� ASSETS_PATH"
#endif
#endif //ANDROID
#define ASSETS_PATH "assets/"
#endif


FileSystem *FileSystem::m_instance = NULL;
FileSystemDestroyer FileSystem::m_destroyer;


/** \class FileSystemDestroyer */

FileSystemDestroyer::~FileSystemDestroyer()
{
	delete m_instance;
}



/** \class FileSystem */

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


utils::bytearray_sptr_t FileSystem::load_from_file(const std::string &path)
{
	// ���� �����������
	const std::string cache_key("f:" + path);
	// �������� ����� ������ � ����
	utils::bytearray_sptr_t ret = cache().get_obj<utils::bytearray_sptr_t>(cache_key);
	// ���� �����, ���������� ��
	if (ret) return ret;
	// ������ � ���� �� �������, �������� ��������� �� �� �����
	FILE *file;
#ifdef _WIN32
	fopen_s(&file, path.c_str(), "rb");
#else
	file = fopen(path.c_str(), "rb");
#endif
	if (!file) {
		DLOG(ERR) << "�� ������� ��������� ����, ��� ������: " << errno;
		return false;
	}

	fseek(file, 0, SEEK_END);
	const long int filesize = ftell(file);
	utils::bytearray buff(filesize);
	fseek(file, 0, SEEK_SET);
	if (fread(buff.get(), buff.size(), 1, file) == 1)
	{
		fclose(file);
		// �������� ����������� ������
		ret.reset(new utils::bytearray(buff));
		cache().cache_obj(cache_key, ret);
		// ���������� ����������� ������
		return ret;
	}
	else
	{
		fclose(file);
		// ������ �� ������� ���������, ���������� ������ ���������
		return utils::bytearray_sptr_t();
	}
}


bool FileSystem::save_to_file(const std::string &path, utils::bytearray &data)
{
	FILE *file;
#ifdef _WIN32
	if (!fopen_s(&file, path.c_str(), "wb")) {
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


utils::bytearray_sptr_t FileSystem::load_resource(const std::string &path)
{
	const std::string full_path = resources_path() + path;
	return load_from_file(full_path);
}