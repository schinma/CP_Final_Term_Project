#include "MyGlWindow.h"


static float DEFAULT_VIEW_POINT[3] = { -10, 10, 10 };
static float DEFAULT_VIEW_CENTER[3] = { 0, 10, 0 };
static float DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };

float rotAngle = 0;

MyGlWindow::MyGlWindow(float w, float h)
{
	m_width = w;
	m_height = h;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	setupBuffer();
}

MyGlWindow::~MyGlWindow()
{

}

void MyGlWindow::resize(int width, int height) {
	m_height = height;
	m_width = width;
}

void MyGlWindow::setupBuffer()
{
	dragon = new Mesh("./models/dragon.obj");
	wrap_shader = new Shader("./shaders/wrap_lighting.vert", "./shaders/wrap_lighting.frag");


	glm::vec3 Ka(0.1, 0.1, 0.1);
	glm::vec3 Kd(0.4, 0.4, 0.4);
	glm::vec3 Ks(0.9, 0.9, 0.9);
	GLfloat shininess = 180.0f;

	glm::vec3 lightIntensity(0.8, 0.8, 0.8);

	wrap_shader->use();
	wrap_shader->setVec3("Ka", Ka);
	wrap_shader->setVec3("Kd", Kd);
	wrap_shader->setVec3("Ks", Ks);
	wrap_shader->setFloat("shininess", shininess);
	wrap_shader->setVec3("Light.Intensity", lightIntensity);
	wrap_shader->disable();
}

void MyGlWindow::draw(Parameter param)
{
	// Set the uniforms for the shader 
	glm::vec3 eye;
	glm::vec3 look;
	glm::vec3 up = m_viewer->getUpVector();

	eye = m_viewer->getViewPoint();
	look = m_viewer->getViewCenter();

	glm::mat4 view = lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.0f, m_width / m_height, 0.1f, 500.0f);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 modelView;
	glm::mat4 inverseModelView;
	glm::mat3 normalMatrix;
	glm::mat4 mvp;

	model = glm::scale(model, glm::vec3(10, 10, 10));

	modelView = view * model;
	inverseModelView = glm::inverse(modelView);
	normalMatrix = glm::mat3(glm::transpose(inverseModelView));
	mvp = projection * view * model;

	wrap_shader->use();
	wrap_shader->setMat4("ModelViewMatrix", modelView);
	wrap_shader->setMat3("NormalMatrix", normalMatrix);
	wrap_shader->setMat4("mvp", mvp);

	glm::vec4 lightPos(param.light_x, param.light_y, param.light_z, 1);
	glm::vec4 viewPos = view * lightPos;

	wrap_shader->setVec4("Light.Position", viewPos);
	wrap_shader->disable();

	dragon->draw(wrap_shader);



}
