#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <utils/bytearray.hpp>


class FileSystem;


class FileSystemDestroyer
{
public:
	~FileSystemDestroyer();
	inline void init(FileSystem *instance) { m_instance = instance; }
private:
	FileSystem *m_instance;
};


class FileSystem
{
private:
	friend class FileSystemDestroyer;
	
	FileSystem();
	
	~FileSystem();
	
public:
	FileSystem(const FileSystem& root) = delete;
	
	FileSystem& operator=(const FileSystem&) = delete;
	
	static FileSystem& instance();
	
	std::string resources_path();
	
	/** @brief Загрузить файл
	 * @param path Полный или относительный (расположению исполняемого файла приложения) путь к файлу.
	 * @param data Ссылка на набор данных, который должен быть загружен из файла.
	 */
	bool load_from_file(const std::string& path, utils::bytearray& data);\
	
	/** @brief Сохранить данные в файл
	 * @param path Полный или относительный (расположению исполняемого файла приложения) путь к файлу.
	 * @param data Ссылка на набор данных, который должен быть сохранен в файл.
	 */
	bool save_to_file(const std::string& path, utils::bytearray& data);
	
	/** @brief Загрузить ресурс
	 * @param path Относительный путь к файлу в ресурсах.
	 * @param data Ссылка на набор данных, который должен быть загружен из ассета.
	 */
	bool load_resource(const std::string& path, utils::bytearray& data);
	
	/** @brief Загрузить ассет
	 * @param path Относительный путь к файлу в ассетах.
	 * @param data Ссылка на набор данных, который должен быть загружен из ассета.
	 */
	bool load_asset(const std::string& path, utils::bytearray& data);
	
private:
	static FileSystem *m_instance;
	static FileSystemDestroyer m_destroyer;
	
	std::string m_resource_path;
	std::string m_assets_path;
};

inline FileSystem& resources() { return FileSystem::instance(); }

#endif //FILESYSTEM_H
