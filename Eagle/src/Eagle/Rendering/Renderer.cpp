#include <EaglePCH.h>
#include "Renderer.h"
#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

// UI Library example usage
	// x and y drivers will derive from baseClass called UIPosition and width and height drivers from UIArea
	// Button might represent some parameters or a component like button
	// Drviers like Left() and Right() are just to tell what there goes, not actual driveers

	//DrawUI(Button).xDriver().yDriver(UITopAligment(10.0f)).widthDriver(UIStretch(0.5f)).heightDriver(UIConstSize(100.0f));
	//DrawUI(Button).Drivers(TopAligment(1.0f), BottomScale(1.0f), Left(1.0f), Right(1.0f))

	// Or probably just

	// DrawUI(Button).Drivers(UICenterAligment, UITopAligment(10.0f), UIStretch(0.5f), UIConstSize(100.0f))
	// DrawUI(Button).Drivers(Top(), Bottom(200f), Left(), Right())

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

		QuadData* quadDataBase = nullptr;
		QuadData* quadDataPtr = nullptr;
		QuadVertex* sortedQuadsBase = nullptr;


		glm::vec4 sampleVertices[4] = {
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f, -0.5f, 0.0f, 1.0f},
			{ 0.5f,  0.5f, 0.0f, 1.0f},
			{-0.5f,  0.5f, 0.0f, 1.0f}
		};
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

		sData.sortedQuadsBase = new QuadVertex[sData.maxVertices];
		sData.quadDataBase = new QuadData[sData.maxQuads];
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
			layout(location = 1) out uint entityID;
			
			in vec2 vTexCoord;
			in vec4 vColor;
			in float vTilingFactor;
			in float vTextureID;

			uniform sampler2D uTextures[32];

			void main() { 
				// Aparently this doesn't work for some devices
				//color = texture(uTextures[int(vTextureID)], vTexCoord * vTilingFactor) * vColor;
				
				vec4 texColor = vColor;
				switch (int(vTextureID)) {
					case 0:  texColor *= texture(uTextures[0],  vTexCoord * vTilingFactor); break;
					case 1:  texColor *= texture(uTextures[1],  vTexCoord * vTilingFactor); break;
					case 2:  texColor *= texture(uTextures[2],  vTexCoord * vTilingFactor); break;
					case 3:  texColor *= texture(uTextures[3],  vTexCoord * vTilingFactor); break;
					case 4:  texColor *= texture(uTextures[4],  vTexCoord * vTilingFactor); break;
					case 5:  texColor *= texture(uTextures[5],  vTexCoord * vTilingFactor); break;
					case 6:  texColor *= texture(uTextures[6],  vTexCoord * vTilingFactor); break;
					case 7:  texColor *= texture(uTextures[7],  vTexCoord * vTilingFactor); break;
					case 8:  texColor *= texture(uTextures[8],  vTexCoord * vTilingFactor); break;
					case 9:  texColor *= texture(uTextures[9],  vTexCoord * vTilingFactor); break;
					case 10: texColor *= texture(uTextures[10], vTexCoord * vTilingFactor); break;
					case 11: texColor *= texture(uTextures[11], vTexCoord * vTilingFactor); break;
					case 12: texColor *= texture(uTextures[12], vTexCoord * vTilingFactor); break;
					case 13: texColor *= texture(uTextures[13], vTexCoord * vTilingFactor); break;
					case 14: texColor *= texture(uTextures[14], vTexCoord * vTilingFactor); break;
					case 15: texColor *= texture(uTextures[15], vTexCoord * vTilingFactor); break;
					case 16: texColor *= texture(uTextures[16], vTexCoord * vTilingFactor); break;
					case 17: texColor *= texture(uTextures[17], vTexCoord * vTilingFactor); break;
					case 18: texColor *= texture(uTextures[18], vTexCoord * vTilingFactor); break;
					case 19: texColor *= texture(uTextures[19], vTexCoord * vTilingFactor); break;
					case 20: texColor *= texture(uTextures[20], vTexCoord * vTilingFactor); break;
					case 21: texColor *= texture(uTextures[21], vTexCoord * vTilingFactor); break;
					case 22: texColor *= texture(uTextures[22], vTexCoord * vTilingFactor); break;
					case 23: texColor *= texture(uTextures[23], vTexCoord * vTilingFactor); break;
					case 24: texColor *= texture(uTextures[24], vTexCoord * vTilingFactor); break;
					case 25: texColor *= texture(uTextures[25], vTexCoord * vTilingFactor); break;
					case 26: texColor *= texture(uTextures[26], vTexCoord * vTilingFactor); break;
					case 27: texColor *= texture(uTextures[27], vTexCoord * vTilingFactor); break;
					case 28: texColor *= texture(uTextures[28], vTexCoord * vTilingFactor); break;
					case 29: texColor *= texture(uTextures[29], vTexCoord * vTilingFactor); break;
					case 30: texColor *= texture(uTextures[30], vTexCoord * vTilingFactor); break;
					case 31: texColor *= texture(uTextures[31], vTexCoord * vTilingFactor); break;
				}
				color = texColor;
				entityID = 10;
			}
		)";
		#pragma endregion
		sData.quadShader = Shader::Create(vertexSource, fragmentSource);
		sData.quadShader->Bind();
		sData.quadShader->SetIntArray("uTextures", samplers, sData.maxTextureSlots);

		// Set the white texture to slot 0
		sData.textureSlots[0] = sData.whiteTexture;
	}
	void Renderer::Shutdown() {
		EAGLE_PROFILE_FUNCTION();
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		EAGLE_PROFILE_FUNCTION();
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(const Camera& camera, const glm::mat4& transform) {
		EAGLE_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		sData.quadShader->Bind();
		sData.quadShader->SetMat4("uViewProjection", viewProj);

		sData.quadDataPtr = sData.quadDataBase;
		sData.quadIndexCount = 0; 
		sData.textureSlotIndex = 1;
	}

	
	void Renderer::StartNewBatch() {
		EndScene();
		sData.quadDataPtr = sData.quadDataBase;
		sData.quadIndexCount = 0;
		sData.textureSlotIndex = 1;
	}

	void Renderer::DrawColorQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		DrawColorQuad(depth, transform, color);
	}
	void Renderer::DrawRotatedColorQuad(uint16_t depth, const glm::vec3& position, float radiants, const glm::vec2& size, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::rotate(glm::mat4(1), radiants, { 0, 0, 1 })
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		DrawColorQuad(depth, transform, color);
	}

	void Renderer::DrawTextureQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });
		glm::vec2 texCoords[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };

		DrawTextureQuad(depth, transform, texture, texCoords, tilingFactor, color);
	}
	void Renderer::DrawRotatedTextureQuad(uint16_t depth, const glm::vec3& position, float radiants, const glm::vec2& size, const Ref<Texture>& texture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::rotate(glm::mat4(1), radiants, { 0, 0, 1 })
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });
		glm::vec2 texCoords[4] = { {0, 0}, {1, 0}, {1, 1}, {0, 1} };

		DrawTextureQuad(depth, transform, texture, texCoords, tilingFactor, color);
	}

	void Renderer::DrawTextureQuad(uint16_t depth, const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();
		EAGLE_ENG_ASSERT(subTexture != nullptr, "Texture cannot be null!");

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		DrawTextureQuad(depth, transform, subTexture->GetTexture(), subTexture->GetTextureCoords(), tilingFactor, color);
	}
	void Renderer::DrawRotatedTextureQuad(uint16_t depth, const glm::vec3& position, float radiants, const glm::vec2& size, const Ref<SubTexture>& subTexture, float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();
		EAGLE_ENG_ASSERT(subTexture != nullptr, "Texture cannot be null!");

		glm::mat4 transform = glm::translate(glm::mat4(1), position)
			* glm::rotate(glm::mat4(1), radiants, { 0, 0, 1 })
			* glm::scale(glm::mat4(1), { size.x, size.y, 1 });

		DrawTextureQuad(depth, transform, subTexture->GetTexture(), subTexture->GetTextureCoords(), tilingFactor, color);
	}

	void Renderer::DrawTextureQuad(uint16_t depth, const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec2 texCoords[4], float tilingFactor, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();
		EAGLE_ENG_ASSERT(texture != nullptr, "Texture cannot be null!");

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

		for (int i = 0; i < 4; i++) {
			QuadVertex& vertex = sData.quadDataPtr->quadVertices[i];
			vertex.position = transform * sData.sampleVertices[i];
			vertex.color = color;
			vertex.texCoord = texCoords[i];
			vertex.tilingFactor = tilingFactor;
			vertex.textureID = textureIndex;
		}
		sData.quadDataPtr->placeIndex = sData.quadIndexCount / 6;
		sData.quadDataPtr->depth = depth;
		sData.quadDataPtr++;

		sData.quadIndexCount += 6;
		sStats.quadCount++;
	}
	void Renderer::DrawColorQuad(uint16_t depth, const glm::mat4& transform, const glm::vec4& color) {
		EAGLE_PROFILE_FUNCTION();

		if (sData.quadIndexCount >= RendererData::maxIndices)
			StartNewBatch();

		for (int i = 0; i < 4; i++) {
			QuadVertex& vertex = sData.quadDataPtr->quadVertices[i];
			vertex.position = transform * sData.sampleVertices[i]; // Access violation here? Maybe didn't begin scene?
			vertex.color = color;
			vertex.texCoord = { 0, 0 }; // This could probably be removed
			vertex.tilingFactor = 0;   	// This could probably be removed
			vertex.textureID = 0;
		}
		sData.quadDataPtr->placeIndex = sData.quadIndexCount / 6;
		sData.quadDataPtr->depth = depth;
		sData.quadDataPtr++;

		sData.quadIndexCount += 6;
		sStats.quadCount++;
	}

	void Renderer::EndScene() {
		EAGLE_PROFILE_FUNCTION();
		//// Sort the vertexbuffer ////

		const uint32_t quadAmount = (uint32_t)(sData.quadDataPtr - sData.quadDataBase);
		const uint32_t vertexAmount = quadAmount * 4;
		const uint32_t verticesSize = vertexAmount * sizeof(QuadVertex);

		std::sort(sData.quadDataBase, sData.quadDataPtr, [](QuadData s1, QuadData s2) {
			return s1.depth < s2.depth;
		});


		// placeIndex tells what index comes here
		QuadVertex* sortedVertexPtr = sData.sortedQuadsBase;
		for (uint32_t i = 0; i < quadAmount; i++) {
			for (int j = 0; j < 4; j++) {
				*sortedVertexPtr = sData.quadDataBase[i].quadVertices[j];
				sortedVertexPtr++;
			}
		}

		//// Do other stuff ////
		sData.quadVB->SetData(sData.sortedQuadsBase, verticesSize);
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