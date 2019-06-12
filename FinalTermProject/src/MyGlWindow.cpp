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
	shadow_map = new Shader("./shaders/shadow_map.vert", "./shaders/shadow_map.frag");
	debug_shader = new Shader("./shaders/debug_depth.vert", "./shaders/debug_depth.frag");

	glm::vec3 Ka(0.1, 0.1, 0.1);
	glm::vec3 Kd(0.4, 0.6, 0.4);
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

	// Setup FBO for depth map
	glGenFramebuffers(1, &fbo);
	// create depth texture

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	wrap_shader->use();
	wrap_shader->setInt("depthMap", 0);
	wrap_shader->disable();

	debug_shader->use();
	debug_shader->setInt("depthMap", 0);
	debug_shader->disable();

}

void MyGlWindow::draw(Parameter param)
{

	glm::vec3 eye;
	glm::vec3 look;
	glm::vec3 up = m_viewer->getUpVector();

	eye = m_viewer->getViewPoint();
	look = m_viewer->getViewCenter();

	glm::mat4 view = lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(45.0f, m_width / m_height, 0.1f, 500.0f);

	glm::vec3 lightPos(param.light_x, param.light_y, param.light_z);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10, 10, 10));

	// 1 render depth of scene to texture from light's point of view
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = -10.0f, far_plane = 10.0f;
	
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(10, 10, 10) , glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	
	shadow_map->use();
	shadow_map->setMat4("lightMVP", lightSpaceMatrix );

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_DEPTH_BUFFER_BIT);
		dragon->draw(shadow_map);
		shadow_map->disable();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2 render scene as normal using the generated depth map  
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	wrap_shader->use();

	glm::mat4 modelView;
	glm::mat4 inverseModelView;
	glm::mat3 normalMatrix;
	glm::mat4 mvp;

	modelView = view * model;
	inverseModelView = glm::inverse(modelView);
	normalMatrix = glm::mat3(glm::transpose(inverseModelView));
	mvp = projection * view * model;

	wrap_shader->use();
	wrap_shader->setMat4("ModelViewMatrix", modelView);
	wrap_shader->setMat3("NormalMatrix", normalMatrix);
	wrap_shader->setMat4("mvp", mvp);

	glm::vec4 viewPos = view * glm::vec4(lightPos, 1);

	wrap_shader->setVec4("Light.Position", viewPos);
	wrap_shader->disable();
	wrap_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	
	dragon->draw(wrap_shader);

	wrap_shader->disable();

	// render Depth map to quad for visual debugging

	if (param.depth) {
		debug_shader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();
	}
}

void MyGlWindow::renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}