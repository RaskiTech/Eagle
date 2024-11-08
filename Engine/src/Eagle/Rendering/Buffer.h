#pragma once
#include <string>
#include <vector>
#include "Eagle/Core/Core.h"
#include "Eagle/Core/Log.h"

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

		EAGLE_ENG_ASSERT(false, "The ShaderDataType was None or unknown");
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
			EAGLE_ENG_ASSERT(false, "The ShaderDataType was None or unknown");
			return 0;
		}
	};

class BufferLayout {
	public:
		BufferLayout() {};
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
		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
	};

	//////////////////////////////////// Index buffer /////////////////////////////////////

	// Currently Eagle only supports 32-bit index buffers
	class IndexBuffer {
	public:
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};
}