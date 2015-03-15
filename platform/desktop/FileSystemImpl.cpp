#include <resources/FileSystem.hpp>
#include <utils/log.hpp>


std::string FileSystem::resources_path()
{
	return m_resource_path;
}


utils::bytearray_sptr_t FileSystem::load_asset(const std::string& path)
{
	const std::string full_path = m_assets_path + path;
	return load_from_file(full_path);
}