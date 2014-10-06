#ifndef APPLICATION_H
#define APPLICATION_H


class Application
{
public:
	static void create();
	
	static void destroy();
	
	inline static Application* instance() { return m_instance; }
	
private:
	static Application* m_instance;
	
	Application();
	
	~Application();
};

#endif // APPLICATION_H
