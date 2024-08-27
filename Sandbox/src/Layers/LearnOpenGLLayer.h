#pragma once
#include <Usagi.h>
#include "platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <glm/glm.hpp>

class LearnOpenGL : public Usagi::Layer {
public:

	LearnOpenGL()
		:m_Camera(glm::perspectiveFov(glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 10000.0f)),
		m_Light(glm::vec3(100.0f,100.0f,100.0f), glm::vec3(1.0f,1.0f,1.0f))
	{
		// data vertices and indices;
		float squareVertices[14 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		};

		unsigned int squareIndices[6] = { 0,1,2,2,3,0 };

		// create VA, VA is a member
		m_VA_rectangle = Usagi::VertexArray::create();

		// create VB VBLayout IB only active in the scope 
		Usagi::Ref<Usagi::VertexBuffer> vertexBuffer = Usagi::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Usagi::BufferLayout squareLayout = {
			{Usagi::ShaderDataType::Float3, "a_Position"},
			{Usagi::ShaderDataType::Float3, "a_Normal"},
			{Usagi::ShaderDataType::Float3, "a_Tangent"},
			{Usagi::ShaderDataType::Float3, "a_Binormal"},
			{Usagi::ShaderDataType::Float2, "a_TexCoords"}
		};
		vertexBuffer->SetLayout(squareLayout);
		Usagi::Ref<Usagi::IndexBuffer> indexBuffer = Usagi::IndexBuffer::Create(squareIndices, 6);

		// add VB IB for VA
		m_VA_rectangle->AddVertexBuffer(vertexBuffer);
		m_VA_rectangle->SetIndexBuffer(indexBuffer);

		// loadShader
		m_ShaderLibrary.Load("assets/shaders/first3D.glsl");
		m_ShaderLibrary.Load("assets/shaders/LearnOpenGLShaders/lightShader.glsl");
		m_ShaderLibrary.Load("assets/shaders/LearnOpenGLShaders/PhongShader.glsl");

		// loadTexture
		m_Texture_usagi3 = Usagi::Texture2D::Create("assets/textures/usagi3.png");
		m_Texture_Cerberus_A = Usagi::Texture2D::Create("assets/textures/cerberus/cerberus_A.png");

		// bind static uniform for a shader
		auto first3DShader = m_ShaderLibrary.Get("first3D");
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(first3DShader)->Bind();
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(first3DShader)->UploadUniformInt("u_Texture", 0);

		auto lightShader = m_ShaderLibrary.Get("lightShader");
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(lightShader)->Bind();
		std::dynamic_pointer_cast<Usagi::OpenGLShader>(lightShader)->UploadUniformFloat3("u_lightColor", m_Light.getColor());


		m_SphereMesh.reset(new Usagi::Mesh("assets/models/Sphere.fbx"));
		m_CerberusMesh.reset(new Usagi::Mesh("assets/models/Cerberus.fbx"));
		m_CubeMesh.reset(new Usagi::Mesh("assets/models/Cube.fbx"));
	}

	void OnUpdate(Usagi::Timestep ts) override {

		// USG_INFO("FPS:{0})", 1000/ts.GetMilliseconds());

		m_Camera.Update();

		Usagi::RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 1 });
		Usagi::RenderCommand::Clear();

		auto first3DShader = m_ShaderLibrary.Get("first3D");

		Usagi::Renderer::BeginScene(m_Camera);

		glm::mat4 transformMatrix_Sphere = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 100.0f)) *
									 glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
									 glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));

		glm::mat4 transformMatrix_Cerberus = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)) *
										glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
										glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

		glm::mat4 transformMatrix_rectangle = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 50.0f)) *
										glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
										glm::scale(glm::mat4(1.0f), glm::vec3(50.0f));

		glm::mat4 transformMatrix_Light = glm::translate(glm::mat4(1.0f), m_Light.getPosition()) *
										glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
										glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));

		auto PhongShader = std::dynamic_pointer_cast<Usagi::OpenGLShader>(m_ShaderLibrary.Get("PhongShader")); 
		PhongShader->Bind();
		PhongShader->UploadUniformFloat3("u_lightColor", m_Light.getColor());
		PhongShader->UploadUniformFloat3("u_lightPosition", m_Light.getPosition());
		PhongShader->UploadUniformFloat3("u_viewPosition", m_Camera.GetPosition());
		PhongShader->UploadUniformMat3("u_NormalMatrix", glm::mat3(glm::transpose(glm::inverse(transformMatrix_Cerberus))));

		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% BEGIN SCENE %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		{
			m_Texture_Cerberus_A->Bind();
			
			// Sphere
			// Usagi::Renderer::Submit(m_ShaderLibrary.Get("PhongShader"), m_SphereMesh->getVertexArray(), transformMatrix_Sphere);
			
			// Cerberus
			// Usagi::Renderer::Submit(m_ShaderLibrary.Get("PhongShader"), m_CerberusMesh->getVertexArray(), transformMatrix_Cerberus);
			
			// Rectangle
			//Usagi::Renderer::Submit(first3DShader, m_VA_rectangle, transformMatrix_rectangle);
			
			// Cube
			Usagi::Renderer::Submit(m_ShaderLibrary.Get("PhongShader"), m_CubeMesh->getVertexArray(), glm::mat4(1.0f));

			// Light
			Usagi::Renderer::Submit(m_ShaderLibrary.Get("lightShader"), m_SphereMesh->getVertexArray(), transformMatrix_Light);
		}
		// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  END SCENE  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		Usagi::Renderer::EndScene();
	}


private:

	Usagi::ProjectionCamera m_Camera;
	Usagi::ShaderLibrary m_ShaderLibrary;
	Usagi::Light m_Light;

	Usagi::Scope<Usagi::Mesh> m_SphereMesh;
	Usagi::Scope<Usagi::Mesh> m_CerberusMesh;
	Usagi::Scope<Usagi::Mesh> m_CubeMesh;

	Usagi::Ref<Usagi::VertexArray> m_VA_rectangle;

	Usagi::Ref<Usagi::Texture2D> m_Texture_usagi3;
	Usagi::Ref<Usagi::Texture2D> m_Texture_Cerberus_A;
};