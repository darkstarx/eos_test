#include <utils/JNI.hpp>
#include <mutex>
#include <map>


#define PATH_SEPARATOR ';' /* define it to be ':' on Solaris */
#define USER_CLASSPATH "." /* where Prog.class is */


namespace jni
{
	
	namespace detail
	{
		static thread_local jni::JEnv* singleton = NULL;
		typedef std::map<std::string, jclass> class_storage_t;
		static class_storage_t class_storage;
		static std::mutex class_storage_m;
	}
	
	
	inline void set_javavm(JavaVM *javavm)
	{
		JEnv::javavm = javavm;
	}
	
	
	JavaVM * JEnv::javavm = NULL;
	
	
	JEnv::JEnv()
	: m_env(NULL)
	, m_obj(NULL)
	, m_prev(detail::singleton)
	, m_attached(false)
	{
		ASSERT(javavm);
		javavm->GetEnv((void **)&m_env, JNI_VERSION_1_4);
		if (!m_env) {
			javavm->AttachCurrentThread(&m_env, NULL);
			CHECK(m_env) << "AttachCurrentThread() failed";
			m_attached = true;
		}
		detail::singleton = this;
	}
	
	
	JEnv::JEnv(JNIEnv* env, jobject obj)
	: m_env(env)
	, m_obj(obj)
	, m_prev(detail::singleton)
	, m_attached(false)
	{
		detail::singleton = this;
	}
	
	
	JEnv::~JEnv()
	{
		if (m_attached) javavm->DetachCurrentThread();
		detail::singleton = m_prev;
	}
	
	
	JEnv& JEnv::instance()
	{
		CHECK(detail::singleton) << "Java environment absent";
		return *detail::singleton;
	}
	
	
	
	void store_class(const std::string& classname, jclass jClass)
	{
		using namespace detail;
		std::unique_lock<std::mutex> class_storage_wrl(class_storage_m, std::try_to_lock);
		ASSERT(!class_storage.count(classname)) << ": classname = " << classname;
		class_storage[classname] = static_cast<jclass>(env()->NewGlobalRef(jClass));
	}
	
	
	jclass get_class(const std::string& classname)
	{
		using namespace detail;
		std::unique_lock<std::mutex> class_storage_rdl(class_storage_m, std::try_to_lock);
		ASSERT(class_storage.count(classname)) << ": classname = " << classname;
		return class_storage[classname];
	}
	
	
	void clear_class_storage()
	{
		using namespace detail;
		std::unique_lock<std::mutex> class_storage_wrl(class_storage_m, std::try_to_lock);
		for (class_storage_t::iterator it(class_storage.begin()), end(class_storage.end()); it != end; ++it) {
			env()->DeleteGlobalRef(it->second);
		}
		class_storage.clear();
	}
	
}


extern "C"
{
	
	JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
	{
		JNIEnv *env;
		jni::set_javavm(vm);
		if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
			LOG(ERR) << "Failed to get Java environment";
			return -1;
		}
		DLOG(INFO) << "Java environment initialized";
		return JNI_VERSION_1_4;
	}
	
	
	JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
	{
		jni::clear_class_storage();
		DLOG(INFO) << "Java environment deinitialized";
	}
	
}
