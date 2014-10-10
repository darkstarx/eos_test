#include <resources/FileSystem.hpp>
#include <utils/log.hpp>


std::string FileSystem::resources_path()
{
	return m_resource_path;
}


bool FileSystem::load_asset(const std::string& path, utils::bytearray& data)
{
	const std::string full_path = m_assets_path + path;
	return load_from_file(full_path, data);
}