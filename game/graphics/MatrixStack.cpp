#include <graphics/MatrixStack.hpp>
#include <glm/gtx/transform.hpp>


namespace graphics
{
	
	MatrixStack* MatrixStack::m_instance = NULL;
	MatrixStackDestroyer MatrixStack::m_destroyer;
	
	/** @class MatrixStackDestroyer */
	MatrixStackDestroyer::~MatrixStackDestroyer()
	{
		delete m_instance;
	}
	
	
	
	/** @class MatrixStack */
	
	MatrixStack::MatrixStack()
	: m_matrix(1.0f)
	{
	}
	
	
	MatrixStack::~MatrixStack()
	{
	}
	
	
	MatrixStack& MatrixStack::instance()
	{
		if (!m_instance)
		{
			m_instance = new MatrixStack();
			m_destroyer.init(m_instance);
		}
		return *m_instance;
	}
	
	
	void MatrixStack::load_identity()
	{
		m_matrix = glm::mat4(1.0f);
	}
	
	
	void MatrixStack::push()
	{
		m_stack.push(m_matrix);
	}
	
	
	void MatrixStack::pop()
	{
		if (m_stack.empty()) return;
		m_matrix = m_stack.top();
		m_stack.pop();
	}
	
	
	void MatrixStack::translate(const position_t& pos)
	{
		translate(pos.x, pos.y, pos.z);
	}
	
	
	void MatrixStack::translate(GLfloat x, GLfloat y, GLfloat z)
	{
		m_matrix = glm::translate(m_matrix, glm::vec3(x, y, z));
	}
	
	
	void MatrixStack::rotate(const rotation_t& rotation)
	{
		if (rotation.angle_x) {
			m_matrix *= glm::rotate(glm::radians(rotation.angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (rotation.angle_y) {
			m_matrix *= glm::rotate(glm::radians(rotation.angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (rotation.angle_z) {
			m_matrix *= glm::rotate(glm::radians(rotation.angle_z), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
	
	
	void MatrixStack::rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
	{
		m_matrix *= glm::rotate(glm::radians(angle), glm::vec3(x, y, z));
	}
	
	
	void MatrixStack::scale(const scale_t scale)
	{
		if (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f) {
			m_matrix = glm::scale(m_matrix, glm::vec3(scale.x, scale.y, scale.z));
		}
	}
	
	
	void MatrixStack::ortho(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
	{
		m_matrix *= glm::ortho(left, right, bottom, top, zNear, zFar);
	}
	
	
	void MatrixStack::frustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
	{
		m_matrix *= glm::frustum(left, right, bottom, top, zNear, zFar);
	}
	
	
	void MatrixStack::perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
	{
		m_matrix *= glm::perspective(fovy, aspect, zNear, zFar);
	}
	
}