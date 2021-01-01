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

	struct RendererData {
		static const uint32_t maxQuads = 50000;
		static const uint32_t maxVertices = 4 * maxQuads;
		static const uint32_t maxIndices = 6 * maxQuads;
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

	static RendererData sData;
	static RendererStats sStats;

	RendererStats& Renderer::GetStats() { return sStats; }

	void Renderer::Init() {
		EAGLE_PROFILE_FUNCTION();

		RenderCommand::Init();
		sData.quadVA = VertexArray::Create();

		sData.quadVB = VertexBuffer::Create(sData.maxVertices * sizeof(QuadVertex));
		sData.quadVB->SetLayout({
			{ ShaderDataType::Float3, "Position" },
			{ ShaderDataType::Float4, "Color" },
			{ ShaderDataType::Float2, "TextureCoordinates" },
			{ ShaderDataType::Float, "TilingFactor" },
			{ ShaderDataType::Float, "TextureID" }
		});
		sData.quadVA->AddVertexBuffer(sData.quadVB);

		sData.quadVertexBufferBase = new QuadVertex[sData.maxVertices];

		uint32_t* quadIndices = new uint32_t[sData.maxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < sData.maxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, sData.maxIndices);
		sData.quadVA->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		sData.whiteTexture = Texture::Create(1, 1);
		uint32_t data = 0xffffffff;
		sData.whiteTexture->SetData(&data, sizeof(uint32_t));

		int32_t samplers[sData.maxTextureSlots];
		for (int32_t i = 0; i < sData.maxTextureSlots; i++)
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
		sData.quadShader = Shader::Create(vertexSource, fragmentSource);
		sData.quadShader->Bind();
		sData.quadShader->SetIntArray("uTextures", samplers, sData.maxTextureSlots);

		// Set the white texture to slot 0
		sData.textureSlots[0] = sData.whiteTexture;

		sData.sampleVertices[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		sData.sampleVertices[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		sData.sampleVertices[2] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		sData.sampleVertices[3] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
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
		sData.quadShader->Bind();
		sData.quadShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());

		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
		sData.quadIndexCount = 0; 
		sData.textureSlotIndex = 1;
	}

	
	void Renderer::StartNewBatch() {
		EndScene();
		sData.quadIndexCount = 0;
		sData.quadVertexBufferPtr = sData.quadVertexBufferBase;
		sData.textureSlotIndex = 1;
	}
	void Renderer::SetQuadVertexBufferDataAndIncrement(QuadVertex*& ptr, const glm::vec3& position, 
		const glm::vec4& color, const glm::vec2& texCoord, float tiligFactor, float textureID) 
	{
		ptr->position = position;
		ptr->color = color;
		ptr->texCoord = texCoord;
		ptr->tilingFactor = tiligFactor;
		ptr->textureID = textureID;
		ptr++;
	}
	void Renderer::DrawColorQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		if (sData.quadIndexCount >= RendererData::maxIndices)
			StartNewBatch();

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		for (int i = 0; i < 4; i++)
			SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[i], color, { 0, 0 }, 1, 0);

		sData.quadIndexCount += 6;

		sStats.quadCount++;
	}
	void Renderer::DrawTextureQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		if (sData.quadIndexCount >= RendererData::maxIndices)
			StartNewBatch();

		// Is this texture bound in some slot already
		float textureIndex = 0;
		for (uint32_t i = 1; i < sData.textureSlotIndex; i++) {
			if (*sData.textureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			// Not bound
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture; // Should be replaced when switching to using assets
			sData.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[0], color, { 0, 0 }, tilingFactor, textureIndex);
		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[1], color, { 1, 0 }, tilingFactor, textureIndex);
		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[2], color, { 1, 1 }, tilingFactor, textureIndex);
		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[3], color, { 0, 1 }, tilingFactor, textureIndex);

		sData.quadIndexCount += 6;

		sStats.quadCount++;
	}
	void Renderer::DrawRotatedColorQuad(const glm::vec3& position, float radiants, const glm::vec2& size, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();
		
		if (sData.quadIndexCount >= RendererData::maxIndices)
			StartNewBatch();

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::rotate(glm::mat4(1), radiants, { 0, 0, 1 })
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		for (int i = 0; i < 4; i++)
			SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[i], color, { 0, 0 }, 1, 0);
		
		sData.quadIndexCount += 6;

		sStats.quadCount++;
	}
	void Renderer::DrawRotatedTextureQuad(const glm::vec3& position, float radiants, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		if (sData.quadIndexCount >= RendererData::maxIndices)
			StartNewBatch();

		// Is this texture bind in some slot already
		float textureIndex = 0;
		for (uint32_t i = 1; i < sData.textureSlotIndex; i++) {
			if (*sData.textureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			// Not bound
			textureIndex = (float)sData.textureSlotIndex;
			sData.textureSlots[sData.textureSlotIndex] = texture; // Should be replaced when switching to using assets
			sData.textureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::rotate(glm::mat4(1), radiants, { 0, 0, 1 })
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[0], color, { 0, 0 }, tilingFactor, textureIndex);
		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[1], color, { 1, 0 }, tilingFactor, textureIndex);
		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[2], color, { 1, 1 }, tilingFactor, textureIndex);
		SetQuadVertexBufferDataAndIncrement(sData.quadVertexBufferPtr, transform * sData.sampleVertices[3], color, { 0, 1 }, tilingFactor, textureIndex);

		sData.quadIndexCount += 6;

		sStats.quadCount++;
	}


	void Renderer::EndScene() {
		EAGLE_PROFILE_FUNCTION();

		uint32_t size = (uint8_t*)sData.quadVertexBufferPtr - (uint8_t*)sData.quadVertexBufferBase;
		sData.quadVB->SetData(sData.quadVertexBufferBase, size);
		Flush();
	}

	void Renderer::Flush() {
		EAGLE_PROFILE_FUNCTION();
		for (uint32_t i = 0; i < sData.textureSlotIndex; i++)
			sData.textureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(sData.quadVA, sData.quadIndexCount);
		sStats.drawCallCount++;
	}
}