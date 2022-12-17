#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "Camera.h"
#include "Ray.h"

class Renderer
{
public:

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Camera& m_Camera);

	std::shared_ptr<Walnut::Image> GetFinalImage() { return m_FinalImage; }

private:

	glm::vec4 TraceRay(const Ray& ray);

private:

	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};


