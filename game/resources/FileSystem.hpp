#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <utils/log.hpp>
#include <utils/forwards.hpp>


class FileSystem;


class FileSystemDestroyer
{
public:
	~FileSystemDestroyer();
	inline void init(FileSystem *instance) { ASSERT(!m_instance); m_instance = instance; }
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
	FileSystem(const FileSystem&) = delete;
	
	FileSystem& operator=(const FileSystem&) = delete;
	
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
	static FileSystemDestroyer m_destroyer;
	
	std::string m_resource_path;
	std::string m_assets_path;
};

inline FileSystem& filesystem() { return FileSystem::instance(); }

#endif //FILESYSTEM_H
