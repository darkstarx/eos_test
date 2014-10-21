#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include <graphics/types.hpp>
#include <glm/glm.hpp>
#include <utils/log.hpp>
#include <stack>


namespace graphics
{
	
	class MatrixStack;
	
	
	class MatrixStackDestroyer
	{
	public:
		~MatrixStackDestroyer();
		inline void init(MatrixStack *instance) { ASSERT(!m_instance); m_instance = instance; }
	private:
		MatrixStack *m_instance;
	};
	
	
	class MatrixStack
	{
	private:
		friend class MatrixStackDestroyer;
		
		MatrixStack();
		
		~MatrixStack();
		
	public:
		MatrixStack(const MatrixStack&) = delete;
		
		const MatrixStack& operator =(const MatrixStack&) = delete;
		
		static MatrixStack& instance();
		
		void load_identity();
		
		void push();
		
		void pop();
		
		void translate(const position_t &pos);
		
		void translate(GLfloat x, GLfloat y, GLfloat z);
		
		void rotate(const rotation_t &rotation);
		
		void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
		
		void scale(const scale_t scale);

		void ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
		
		void frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
		
		void perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
		
		inline const glm::mat4& get() const { return m_matrix; }
		
		inline const GLfloat* array() const { return &m_matrix[0][0]; }
		
		inline void set(const glm::mat4 &value) { m_matrix = value; }
		
	private:
		static MatrixStack* m_instance;
		static MatrixStackDestroyer m_destroyer;
		
		glm::mat4 m_matrix;
		std::stack<glm::mat4> m_stack;
	};
	
	
	inline MatrixStack& matrix() { return MatrixStack::instance(); }
}

#endif // MATRIXSTACK_H
