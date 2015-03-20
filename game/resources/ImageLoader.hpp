#ifndef RESOURCES_IMAGELOADER_HPP
#define RESOURCES_IMAGELOADER_HPP

#include <resources/forwards.hpp>
#include <utils/forwards.hpp>
#include <string>


/** \brief Статический класс для загрузки изображений
 * Позволяет загрузить изображение из файла или из памяти, выполнив декодирование.
 */
class ImageLoader
{
public:
	ImageLoader() = delete;
	
	~ImageLoader() = delete;
	
	ImageLoader(const ImageLoader& copy) = delete;
	
	ImageLoader& operator=(const ImageLoader&) = delete;
	
	/** \brief Загрузить изображение из файла ресурсов
	 */
	static resources::ImageSPtr load_from_res(const std::string &path);
	
	/** \brief Загрузить изображение из ассета
	 */
	static resources::ImageSPtr load_from_asset(const std::string &path);
	
private:
	/** \brief Выполнить декодирование изображения
	 */
	static resources::ImageSPtr decode(const utils::bytearray_sptr_t &encoded, const std::string &desc);
};

#endif // RESOURCES_IMAGELOADER_HPP
