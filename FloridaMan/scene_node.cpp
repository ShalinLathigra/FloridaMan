#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>
#include "utilities.h"

#include "scene_node.h"

namespace game {

	SceneNode::SceneNode(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture, const Resource *envmap) {

		// Set name of scene node
		name_ = name;

		m_pParentNode = nullptr;
		// Set geometry
		if (geometry->GetType() == PointSet) {
			mode_ = GL_POINTS;
		}
		else if (geometry->GetType() == Mesh) {
			mode_ = GL_TRIANGLES;
		}
		else {
			throw(std::invalid_argument(std::string("Invalid type of geometry")));
		}

		array_buffer_ = geometry->GetArrayBuffer();
		element_array_buffer_ = geometry->GetElementArrayBuffer();
		size_ = geometry->GetSize();

		// Set material (shader program)
		if (material->GetType() != Material) {
			throw(std::invalid_argument(std::string("Invalid type of material")));
		}

		material_ = material->GetResource();

		// Set texture
		if (texture) {
			texture_ = texture->GetResource();
		}
		else {
			texture_ = 0;
		}

		// Set environment map texture
		if (envmap) {
			envmap_ = envmap->GetResource();
		}
		else {
			envmap_ = 0;
		}

		// Other attributes
		scale_ = glm::vec3(1.0, 1.0, 1.0);
		blending_ = false;
		m_isVisible = true;
		skybox_ = false;

		updated_ = false;
	}

	SceneNode::SceneNode(const std::string name)
	{
		name_ = name;
		m_isVisible = false;
		m_pParentNode = nullptr;
		m_childNodes = std::vector<SceneNode*>();
	}
	SceneNode::SceneNode()
	{
		name_ = std::string("Undefined Node");
		m_isVisible = false;
		m_pParentNode = nullptr;
		m_childNodes = std::vector<SceneNode*>();
	}

	SceneNode *SceneNode::GetParent()
	{
		return m_pParentNode;
	}

	std::vector<SceneNode*> SceneNode::GetChildren()
	{
		return m_childNodes;
	}

	void SceneNode::SetParent(SceneNode *pParent)
	{
		m_pParentNode = pParent;
	}

	void SceneNode::AddChild(SceneNode *pChildNode)
	{
		pChildNode->SetParent(this);
		m_childNodes.push_back(pChildNode);
	}

	SceneNode *SceneNode::FindChild(std::string nodeName)
	{
		//std::cout << name_ << " " << nodeName << std::endl;
		if (name_.compare(nodeName) == 0)
		{
			return this;
		}
		for (size_t i = 0; i < m_childNodes.size(); i++)
		{
			SceneNode *node = m_childNodes[i]->FindChild(nodeName);
			if (node) {
				return node;
			}
		}
		return nullptr;
	}

	bool SceneNode::checkIfDestroy() {
		return set_toDestroy;
	}

	SceneNode* SceneNode::RemoveChildAt(int index) {
		if (m_childNodes[index]->m_childNodes.empty()) {
			SceneNode *node = *(m_childNodes.begin() + index);
			m_childNodes.erase(m_childNodes.begin() + index);
			return node;
		}
	}

	SceneNode::~SceneNode() {
	}


	const std::string SceneNode::GetName(void) const {

		return name_;
	}


	glm::vec3 SceneNode::GetPosition(void) const {

		return position_;
	}

	bool SceneNode::CheckCollision(SceneNode *pNode)
	{
		//Set up the local variables to be used for calculating the collision
		glm::vec3 c1Pos = position_;
		glm::vec3 pNodePos = pNode->GetPosition();

		glm::vec3 c1_xAxis, pNode_xAxis, c1_yAxis, pNode_yAxis, c1_zAxis, pNode_zAxis;
		c1_xAxis = utilities::RotateVecByQuat(glm::vec3(1.0f, 0.0f, 0.0f), GetOrientation());
		c1_yAxis = utilities::RotateVecByQuat(glm::vec3(0.0f, 1.0f, 0.0f), GetOrientation());
		c1_zAxis = utilities::RotateVecByQuat(glm::vec3(0.0f, 0.0f, 1.0f), GetOrientation());
		pNode_xAxis = utilities::RotateVecByQuat(glm::vec3(1.0f, 0.0f, 0.0f), pNode->GetOrientation());
		pNode_yAxis = utilities::RotateVecByQuat(glm::vec3(0.0f, 1.0f, 0.0f), pNode->GetOrientation());
		pNode_zAxis = utilities::RotateVecByQuat(glm::vec3(0.0f, 0.0f, 1.0f), pNode->GetOrientation());

		float c1x = scale_.x / 2.0f;
		float c1y = scale_.y / 2.0f;
		float c1z = scale_.z / 2.0f;
		float pNodex = pNode->GetScale().x / 2.0f;
		float pNodey = pNode->GetScale().y / 2.0f;
		float pNodez = pNode->GetScale().z / 2.0f;

		glm::vec3 T = pNodePos - c1Pos;
		bool collisionDetected = false;
		//Case 1
		glm::vec3 L = c1_xAxis;
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 2
		L = c1_yAxis;
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 3
		L = c1_zAxis;
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 4
		L = pNode_xAxis;
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 5
		L = pNode_yAxis;
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 6
		L = pNode_zAxis;
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 7
		L = glm::cross(c1_xAxis, pNode_xAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 8
		L = glm::cross(c1_xAxis, pNode_yAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 9
		L = glm::cross(c1_xAxis, pNode_zAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 10
		L = glm::cross(c1_yAxis, pNode_xAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 11
		L = glm::cross(c1_yAxis, pNode_yAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 12
		L = glm::cross(c1_yAxis, pNode_zAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 13
		L = glm::cross(c1_zAxis, pNode_xAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 14
		L = glm::cross(c1_zAxis, pNode_yAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		//Case 15
		L = glm::cross(c1_zAxis, pNode_zAxis);
		collisionDetected = collisionDetected || (glm::abs(glm::dot(T, L)) > glm::abs(glm::dot(c1x * c1_xAxis, L)) + glm::abs(glm::dot(c1y * c1_yAxis, L)) + glm::abs(glm::dot(c1z * c1_zAxis, L)) + glm::abs(glm::dot(pNodex * pNode_xAxis, L)) + glm::abs(glm::dot(pNodey*pNode_yAxis, L)) + glm::abs(glm::dot(pNodez * pNode_zAxis, L)));
		return !collisionDetected;

	}

	glm::quat SceneNode::GetOrientation(void) const {

		return orientation_;
	}


	glm::vec3 SceneNode::GetScale(void) const {

		return scale_;
	}


	bool SceneNode::GetBlending(void) const {

		return blending_;
	}


	void SceneNode::SetPosition(glm::vec3 position) {

		position_ = position;
	}


	void SceneNode::SetOrientation(glm::quat orientation) {

		orientation_ = orientation;
	}


	void SceneNode::SetScale(glm::vec3 scale) {

		scale_ = scale;
	}


	void SceneNode::Translate(glm::vec3 trans) {

		position_ += trans;
	}


	void SceneNode::Rotate(glm::quat rot) {

		orientation_ *= rot;
		orientation_ = glm::normalize(orientation_);
	}


	void SceneNode::Scale(glm::vec3 scale) {

		scale_ *= scale;
	}


	void SceneNode::SetBlending(bool blending) {

		blending_ = blending;
	}
	void SceneNode::SetSkybox(bool skybox)
	{
		skybox_ = skybox;
	}


	GLenum SceneNode::GetMode(void) const {

		return mode_;
	}


	GLuint SceneNode::GetArrayBuffer(void) const {

		return array_buffer_;
	}


	GLuint SceneNode::GetElementArrayBuffer(void) const {

		return element_array_buffer_;
	}


	GLsizei SceneNode::GetSize(void) const {

		return size_;
	}


	GLuint SceneNode::GetMaterial(void) const {

		return material_;
	}


	void SceneNode::Draw(Camera *camera) {

		if (m_isVisible)
		{
			// Select blending or not
			if (blending_) {
				// Disable z-buffer
				glDisable(GL_DEPTH_TEST);

				// Enable blending
				glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Simpler form
				glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
			}
			else {
				// Enable z-buffer
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
			}

			// Select proper material (shader program)
			glUseProgram(material_);

			// Set geometry to draw
			glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

			// Set globals for camera
			camera->SetupShader(material_);

			// Set world matrix and other shader input variables
			SetupShader(material_, camera);

			// Draw geometry
			if (mode_ == GL_POINTS) {
				glDrawArrays(mode_, 0, size_);
			}
			else {
				glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
			}
		}

		//Draw all child nodes
		for (size_t i = 0; i < m_childNodes.size(); i++)
		{
			m_childNodes[i]->Draw(camera);
		}

	}

	glm::mat4 SceneNode::GetParentTransform()
	{
		if (m_pParentNode)
		{
			glm::mat4 rotation = glm::mat4_cast(m_pParentNode->GetOrientation());
			glm::mat4 translation = glm::translate(glm::mat4(1.0), m_pParentNode->GetPosition());
			return m_pParentNode->GetParentTransform() * translation * rotation;// * m_pParentNode->GetParentTransform();
		}
		return glm::mat4(1.0);
	}

	void SceneNode::Update(float deltaTime) {
		// Do nothing for this generic type of scene node, then update all Child nodes.
		for (size_t i = 0; i < m_childNodes.size(); i++)
		{
			m_childNodes[i]->Update(deltaTime);
		}
	}


	void SceneNode::SetupShader(GLuint program, Camera *camera) {

		// Set attributes for shaders
		GLint vertex_att = glGetAttribLocation(program, "vertex");
		glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(vertex_att);

		GLint normal_att = glGetAttribLocation(program, "normal");
		glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(normal_att);

		GLint color_att = glGetAttribLocation(program, "color");
		glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(color_att);

		GLint tex_att = glGetAttribLocation(program, "uv");
		glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(tex_att);

		// World transformation
		glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
		glm::mat4 rotation = glm::mat4_cast(orientation_);
		glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);
		if (skybox_)
			translation = glm::translate(glm::mat4(1.0), glm::vec3(position_.x, position_.y * 0.5f, position_.z));

		glm::mat4 transf = GetParentTransform() * translation * rotation * scaling;

		GLint world_mat = glGetUniformLocation(program, "world_mat");
		glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

		// Normal matrix
		glm::mat4 normal_matrix = glm::transpose(glm::inverse(transf));
		GLint normal_mat = glGetUniformLocation(program, "normal_mat");
		glUniformMatrix4fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));

		// Normal matrix with view added
		glm::mat4 normal_view_matrix = glm::transpose(glm::inverse(camera->GetCurrentViewMatrix() * transf));
		GLint normal_view_mat = glGetUniformLocation(program, "normal_view_mat");
		glUniformMatrix4fv(normal_view_mat, 1, GL_FALSE, glm::value_ptr(normal_view_matrix));

		// Texture
		if (texture_) {
			GLint tex = glGetUniformLocation(program, "texture_map");
			glUniform1i(tex, 0); // Assign the first texture to the map
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_); // First texture we bind
			// Define texture interpolation
 			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// Environment map
		if (envmap_) {
			//GLint tex = glGetUniformLocation(program, "env_map");
			//glUniform1i(tex, 1); // Assign the first texture to the map
			//glActiveTexture(GL_TEXTURE1); 
			//glBindTexture(GL_TEXTURE_CUBE_MAP, envmap_); // First texture we bind
			//// Define texture interpolation
			//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		// Timer
		GLint timer_var = glGetUniformLocation(program, "timer");
		double current_time = glfwGetTime();
		glUniform1f(timer_var, (float)current_time);
		// Type
		GLint type_var = glGetUniformLocation(program, "type");
		glUniform1i(type_var, type_);
	}
	void SceneNode::SetGame(Game* game)
	{
		game_ = game;
	}

	void SceneNode::SetUpdated(bool u)
	{
		updated_ = u;
	}
	bool SceneNode::GetUpdated(void)
	{
		return updated_;
	}

} // namespace game;
