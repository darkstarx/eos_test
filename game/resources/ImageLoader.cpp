#include <resources/ImageLoader.hpp>
#include <resources/Image.hpp>
#include <resources/FileSystem.hpp>

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include <resources/stb_image.h>


resources::ImageSPtr ImageLoader::load_from_res(const std::string& path)
{
	resources::ImageSPtr ret;
	utils::bytearray encoded;
	if (!filesystem().load_resource(path, encoded)) {
		LOG(ERR) << "Не удалось загрузить изображение из ресурса `" << path << "`";
		return resources::ImageSPtr();
	}
	return decode(encoded, "ресурс `" + path + "`");
}


resources::ImageSPtr ImageLoader::load_from_asset(const std::string& path)
{
	utils::bytearray encoded;
	if (!filesystem().load_asset(path, encoded)) {
		LOG(ERR) << "Не удалось загрузить изображение из ассета `" << path << "`";
		return resources::ImageSPtr();
	}
	return decode(encoded, "ассет `" + path + "`");
}


resources::ImageSPtr ImageLoader::decode(const utils::bytearray& encoded, const std::string &desc)
{
	resources::ImageSPtr ret;
	unsigned char *data = 0;
	int width = 0;
	int height = 0;
	int components = 0;
	data = stbi_load_from_memory(encoded.get(), encoded.size(), &width, &height, &components, 0);
	if (data) {
		utils::bytearray decoded(data, width * height * components * sizeof(stbi_uc));
		ret.reset(new resources::Image(decoded, width, height, components));
		DLOG(INFO) << "Успешно декодирован " << desc << ": " << width << "x" << height << "x" << components;
	} else {
		LOG(ERR) << "Не удалось декодировать " << desc;
	}
	stbi_image_free(data);
	return ret;
}
