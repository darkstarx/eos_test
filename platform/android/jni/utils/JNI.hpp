#ifndef JNI_JNI_HPP
#define JNI_JNI_HPP
#include <utils/log.hpp>
#include <jni.h>
#include <memory>


namespace jni
{
	
	/** @brief Обертка над синглтоном java-окружения
	 */
	struct JEnv
	{
		/** 
		 * Позволяет инициализировать окружение Java имеющимся значением.
		 * Необходимо вызывать там, где контекст Java недоступен, например, после создание нитей.
		 */
		JEnv();
		
		/**
		 * Позволяет инициализировать окружение Java передаваемым значением.
		 * Подходит для создание объекта окружения в вызовах методов из Java.
		 */
		JEnv(JNIEnv *env, jobject obj);
		
		~JEnv();
		
		JEnv(const JEnv&) = delete;
		JEnv& operator =(const JEnv&) = delete;
		
		inline JNIEnv* env() const { return m_env; }
		inline jobject obj() const { return m_obj; }
		
		static bool present();
		static JEnv& instance();
		
	private:
		JNIEnv *m_env;			///< Java-окружение, специфичное потоку, которому принадлежит данный JEnv
		jobject m_obj;			///< Объект Activity
		JEnv *m_prev;			///< Предыдущий экземпляр JEnv синглтона, принадлежащего текущему потоку
		bool m_attached;		///< Признак того, что это окружение было инициализировано в не нативном потоке
		
		static JavaVM *javavm;	///< Java-машина
		
		friend void set_javavm(JavaVM*);
	};
	
	
	inline JNIEnv* env() { return JEnv::instance().env(); }
	inline jobject obj() { return JEnv::instance().obj(); }
	
	/**
	 * Class storage нужен по следующей причине:
	 * У нас есть места, где надо получать доступ к нашим java классам (не андроидовским) из потоков, созданных в сишном коде,
	 * а не в нативном (java). Например, когда мы вызываем bitmap::convert_to_png из background_queue. В этом случае, если
	 * попытаться вызвать FindClass для нашего класса, то она выдаст NULL:
	 * http://developer.android.com/intl/ru/training/articles/perf-jni.html#faq_FindClass
	 *
	 * Замечания по использованию:
	 * - Не кэшировать результат выполнения функции get_class в static переменных, она возвращает global reference, которая
	 *   будет разрушена при разрушении SQActivity.
	 */
	void store_class(const std::string& classname, jclass jClass);
	jclass get_class(const std::string& classname);
	void clear_class_storage();
	
}

#endif //JNI_JNI_HPP
