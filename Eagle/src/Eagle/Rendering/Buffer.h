#pragma once

namespace Egl {
	//////////////////////////////////// Buffer Layout ////////////////////////////////////

	enum class ShaderDataType {
		None, Float, Float2, Float3, Float4, Matrix3, Matrix4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:   return 4;
			case ShaderDataType::Float2:  return 4 * 2;
			case ShaderDataType::Float3:  return 4 * 3;
			case ShaderDataType::Float4:  return 4 * 4;
			case ShaderDataType::Matrix3: return 4 * 3 * 3;
			case ShaderDataType::Matrix4: return 4 * 4 * 4;
			case ShaderDataType::Int:     return 4;
			case ShaderDataType::Int2:    return 4 * 2;
			case ShaderDataType::Int3:    return 4 * 3;
			case ShaderDataType::Int4:    return 4 * 4;
			case ShaderDataType::Bool:    return 1;
		}

		EAGLE_CORE_ASSERT(false, "The shaderDataType was None or unknown");
		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		uint32_t offset;
		uint32_t size;
		bool normalized;

		BufferElement() : name("Element"), type(ShaderDataType::None), offset(0), size(0), normalized(false) {}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {}

		uint32_t GetComponentCount() const {
			switch (type)
			{
				case Egl::ShaderDataType::None:    return 0;
				case Egl::ShaderDataType::Float:   return 1;
				case Egl::ShaderDataType::Float2:  return 2;
				case Egl::ShaderDataType::Float3:  return 3;
				case Egl::ShaderDataType::Float4:  return 4;
				case Egl::ShaderDataType::Matrix3: return 3 * 3;
				case Egl::ShaderDataType::Matrix4: return 4 * 4;
				case Egl::ShaderDataType::Int:     return 1;
				case Egl::ShaderDataType::Int2:    return 2;
				case Egl::ShaderDataType::Int3:    return 3;
				case Egl::ShaderDataType::Int4:    return 4;
				case Egl::ShaderDataType::Bool:    return 1;
			}
			EAGLE_CORE_ASSERT(false, "The shaderDataType was None or unknown");
			return 0;
		}
	};

class BufferLayout {
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: mElements(elements) { CalculateOffsetsAndStride(); }

		inline const std::vector<BufferElement>& GetElements() const { return mElements; }
		inline uint32_t GetStride() const { return mStride; }
		std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
		std::vector<BufferElement>::iterator end() { return mElements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

	private:
		void CalculateOffsetsAndStride() {
			uint32_t offset = 0;
			mStride = 0;
			for (auto& element : mElements) {
				element.offset = offset;
				offset += element.size;
			}
			mStride = offset;
		}

		std::vector<BufferElement> mElements;
		uint32_t mStride = 0;
	};

	//////////////////////////////////// Vertex buffer ////////////////////////////////////

	class VertexBuffer {
	public:
		inline static VertexBuffer* Create(float* vertices, uint32_t size) { return new VertexBuffer(vertices, size); }
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
		void SetLayout(const BufferLayout& layout) { mLayout = layout; }
		const BufferLayout& GetLayout() const { return mLayout; }

	private:
		BufferLayout mLayout;


#ifdef EAGLE_RENDERER_OPENGL
	private:
		uint32_t mRendererID;
#endif

	};

	//////////////////////////////////// Index buffer /////////////////////////////////////

	class IndexBuffer {
	public:
		inline static IndexBuffer* Create(uint32_t* indices, uint32_t count) { return new IndexBuffer(indices, count); }
		IndexBuffer(uint32_t* indices, uint32_t size);
		~IndexBuffer();

		inline uint32_t GetCount() const { return mCount; }
		void Bind() const;
		void Unbind() const;

	private:
		uint32_t mCount;


#ifdef EAGLE_RENDERER_OPENGL
	private:
		uint32_t mRendererID;
#endif

	};
}