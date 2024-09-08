#pragma once

#include "Math.h"

namespace engine {
	struct VertexBuffer
	{
		unsigned int id;
	};

	struct IndexBuffer
	{
		unsigned int id;
		int indexCount;
	};

	struct Shader 
	{
		unsigned int id;
	};

	class GraphicsAPI {
	public:
		// Initialization
		static void initialize();
		static void shutdown();

		// Render targets
		static void setViewport(Vec2i resolution);
		static void clear(Vec4 color);
		static void present();

		// TODO: Pipeline state (like: depth/stencil states, blend modes, rasterizer settings)
		// TODO: Texture management

		// Buffer management
		static void createVertexBuffer(VertexBuffer& buffer);
		static void bindVertexBuffer(const VertexBuffer& buffer);
		static void deleteVertexBuffer(VertexBuffer& buffer);

		static void createIndexBuffer(IndexBuffer& buffer);
		static void bindIndexBuffer(const IndexBuffer& buffer);
		static void deleteIndexBuffer(IndexBuffer& buffer);

		// Shader management
		static void createShader(Shader& shader);
		static void bindShader(const Shader& shader);
		static void deleteShader(Shader& shader);

		// TODO: Cover other shader types then fragment & vertex?
		// TODO: Uniform management

		// Draw calls
		static void drawIndexed(const IndexBuffer& indexBuffer);

	private:
		GraphicsAPI() = default;
		~GraphicsAPI() = default;
	};
}