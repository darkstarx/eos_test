#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <utils/log.hpp>
#include <utils/forwards.hpp>


class FileSystem
{
private:
	FileSystem();
	
	~FileSystem();
	
public:
	FileSystem(const FileSystem&) = delete;
	
	FileSystem& operator=(const FileSystem&) = delete;
	
	/** \brief Создать экземпляр доступа к файловой системе
	 */
	static void create();
	
	/** \brief Разрушить экземпляр доступа к файловой системе
	 */
	static void destroy();
	
	/** \brief Проверить, что экземпляр доступа к файловой системе создан
	 */
	static bool is_alive() { return m_instance != 0; }
	
	/** \brief Получить экземпляр доступа к файловой системе
	 */
	static FileSystem& instance();
	
	std::string resources_path();
	
	/** \brief Загрузить файл
	 * \param path Полный или относительный (расположению исполняемого файла приложения) путь к файлу.
	 * \return Указатель на набор данных. Если загрузить не удалось, то пустой указатель.
	 */
	utils::bytearray_sptr_t load_from_file(const std::string& path);
	
	/** \brief Сохранить данные в файл
	 * \param path Полный или относительный (расположению исполняемого файла приложения) путь к файлу.
	 * \param data Данные, которые следует сохранить в файл.
	 * \return Признак успешности операции.
	 */
	bool save_to_file(const std::string& path, utils::bytearray &data);
	
	/** \brief Загрузить ресурс
	 * \param path Относительный путь к файлу в ресурсах.
	 * \return Указатель на набор данных. Если загрузить не удалось, то пустой указатель.
	 */
	utils::bytearray_sptr_t load_resource(const std::string& path);
	
	/** \brief Загрузить ассет
	 * \param path Относительный путь к файлу в ассетах.
	 * \return Указатель на набор данных. Если загрузить не удалось, то пустой указатель.
	 */
	utils::bytearray_sptr_t load_asset(const std::string& path);
	
private:
	static FileSystem *m_instance;
	
	std::string m_resource_path;
	std::string m_assets_path;
};

inline FileSystem& filesystem() { return FileSystem::instance(); }

#endif //FILESYSTEM_H
