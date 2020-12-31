#include <EaglePCH.h>
#include "Renderer.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

void UILibraryExampleUse() {
	// x and y drivers will derive from baseClass called UIPosition and width and height drivers from UIArea
	// Button might represent some parameters or a component like button
	// Drviers like Left() and Right() are just to tell what there goes, not actual driveers

	//DrawUI(Button).xDriver().yDriver(TopAligment(10.0f)).widthDriver(Stretch(0.5f)).heightDriver(ConstValue(100.0f));
	//DrawUI(Button).Drivers(TopAligment(1.0f), BottomScale(1.0f), Left(1.0f), Right(1.0f))
}

namespace Egl {

	struct QuadVertex {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float tilingFactor;
		float textureID;
	};

	struct RendererData {
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = 4 * maxQuads;
		const uint32_t maxIndices = 6 * maxQuads;
		static const uint32_t maxTextureSlots = 32;

		std::array<Ref<Texture>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1; // Slot 0 is a white texture for colors

		Ref<VertexArray> quadVA;
		Ref<VertexBuffer> quadVB;
		Ref<Shader> quadShader;
		Ref<Texture> whiteTexture;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		glm::vec4 sampleVertices[4];
	};
	static RendererData sRendererData;

	void Renderer::Init() {
		EAGLE_PROFILE_FUNCTION();

		RenderCommand::Init();
		sRendererData.quadVA = VertexArray::Create();

		sRendererData.quadVB = VertexBuffer::Create(sRendererData.maxVertices * sizeof(QuadVertex));
		sRendererData.quadVB->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Float2, "TextureCoordinates" },
			{ ShaderDataType::Float, "TilingFactor" },
			{ ShaderDataType::Float, "TextureID" }
		});
		sRendererData.quadVA->AddVertexBuffer(sRendererData.quadVB);

		sRendererData.quadVertexBufferBase = new QuadVertex[sRendererData.maxVertices];

		uint32_t* quadIndices = new uint32_t[sRendererData.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < sRendererData.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, sRendererData.maxIndices);
		sRendererData.quadVA->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		sRendererData.whiteTexture = Texture::Create(1, 1);
		uint32_t data = 0xffffffff;
		sRendererData.whiteTexture->SetData(&data, sizeof(uint32_t));

		int32_t samplers[sRendererData.maxTextureSlots];
		for (int32_t i = 0; i < sRendererData.maxTextureSlots; i++)
			samplers[i] = i;

		// TODO: TilingFactor
		#pragma region Shader Sourcecode
		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			layout(location = 1) in vec4 aColor;
			layout(location = 2) in vec2 aTexCoord;
			layout(location = 3) in float aTilingFactor;
			layout(location = 4) in float aTextureID;

			uniform mat4 uViewProjection;

			out vec2 vTexCoord;
			out vec4 vColor;
			out float vTilingFactor;
			out float vTextureID;

			void main() {
				vTilingFactor = aTilingFactor;
				vColor = aColor;
				vTexCoord = aTexCoord;
				vTextureID = aTextureID;
				gl_Position = uViewProjection * vec4(position, 1.0);
			}
		)";
		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			
			in vec2 vTexCoord;
			in vec4 vColor;
			in float vTilingFactor;
			in float vTextureID;

			uniform sampler2D uTextures[32];

			void main() { 
				color = texture(uTextures[int(vTextureID)], vTexCoord * vTilingFactor) * vColor;
			}
		)";
		#pragma endregion
		sRendererData.quadShader = Shader::Create(vertexSource, fragmentSource);
		sRendererData.quadShader->Bind();
		sRendererData.quadShader->SetIntArray("uTextures", samplers, sRendererData.maxTextureSlots);

		// Set the white texture to slot 0
		sRendererData.textureSlots[0] = sRendererData.whiteTexture;

		sRendererData.sampleVertices[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		sRendererData.sampleVertices[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		sRendererData.sampleVertices[2] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		sRendererData.sampleVertices[3] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
	}
	void Renderer::Shutdown() {
		EAGLE_PROFILE_FUNCTION();
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		EAGLE_PROFILE_FUNCTION();
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera) {
		EAGLE_PROFILE_FUNCTION();
		sRendererData.quadShader->Bind();
		sRendererData.quadShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());

		sRendererData.quadVertexBufferPtr = sRendererData.quadVertexBufferBase;
		sRendererData.quadIndexCount = 0; 
		sRendererData.textureSlotIndex = 1;
	}

	static inline void SetQuadVertexBufferDataAndIncrement(QuadVertex* ptr, const glm::vec3& position, const glm::vec4 color, const glm::vec2 texCoord) {
		ptr->position = position;
		ptr->color = color;
		ptr->texCoord = texCoord;
		ptr++;
	}
	void Renderer::DrawColorQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[0];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[1];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[2];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[3];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadIndexCount += 6;
	}
	void Renderer::DrawTextureQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		// Is this texture bind in some slot already
		float textureIndex = 0;
		for (uint32_t i = 1; i < sRendererData.textureSlotIndex; i++) {
			if (*sRendererData.textureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			// Not bound
			textureIndex = (float)sRendererData.textureSlotIndex;
			sRendererData.textureSlots[sRendererData.textureSlotIndex] = texture; // Should be replaced when switching to using assets
			sRendererData.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[0];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[1];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[2];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[3];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadIndexCount += 6;
	}
	void Renderer::DrawRotatedColorQuad(const glm::vec3& position, float radiants, const glm::vec2& size, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();
		
		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::rotate(glm::mat4(1), radiants, { 0, 0, 1 })
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[0];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[1];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[2];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[3];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = 1;
		sRendererData.quadVertexBufferPtr->textureID = 0;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadIndexCount += 6;
	}
	void Renderer::DrawRotatedTextureQuad(const glm::vec3& position, float radiants, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		// Is this texture bind in some slot already
		float textureIndex = 0;
		for (uint32_t i = 1; i < sRendererData.textureSlotIndex; i++) {
			if (*sRendererData.textureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			// Not bound
			textureIndex = (float)sRendererData.textureSlotIndex;
			sRendererData.textureSlots[sRendererData.textureSlotIndex] = texture; // Should be replaced when switching to using assets
			sRendererData.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::rotate(glm::mat4(1), radiants, { 0, 0, 1 })
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[0];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[1];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 0 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[2];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 1, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadVertexBufferPtr->position = transform * sRendererData.sampleVertices[3];
		sRendererData.quadVertexBufferPtr->color = color;
		sRendererData.quadVertexBufferPtr->texCoord = { 0, 1 };
		sRendererData.quadVertexBufferPtr->tilingFactor = tilingFactor;
		sRendererData.quadVertexBufferPtr->textureID = textureIndex;
		sRendererData.quadVertexBufferPtr++;

		sRendererData.quadIndexCount += 6;
	}


	void Renderer::EndScene() {
		EAGLE_PROFILE_FUNCTION();

		uint32_t size = (uint8_t*)sRendererData.quadVertexBufferPtr - (uint8_t*)sRendererData.quadVertexBufferBase;
		sRendererData.quadVB->SetData(sRendererData.quadVertexBufferBase, size);
		Flush();
	}

	void Renderer::Flush() {
		EAGLE_PROFILE_FUNCTION();
		for (uint32_t i = 0; i < sRendererData.textureSlotIndex; i++)
			sRendererData.textureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(sRendererData.quadVA, sRendererData.quadIndexCount);

	}
}