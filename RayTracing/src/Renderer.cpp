#include "Renderer.h"
#include "Walnut/Random.h"

namespace utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t red = (uint8_t)(color.r * 255.0f);
		uint8_t green = (uint8_t)(color.g * 255.0f);
		uint8_t blue = (uint8_t)(color.b * 255.0f);
		uint8_t alpha = (uint8_t)(color.a * 255.0f);

		uint32_t result = (alpha << 24) | (blue << 16) | (green << 8) | red;

		return result;
	}
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if(m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(const Camera& m_Camera)
{
	Ray ray;
	ray.Origin = m_Camera.GetPosition();

	for(uint32_t y=0; y<m_FinalImage->GetHeight(); y++)
	{
		for(uint32_t x=0; x<m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = m_Camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = utils::ConvertToRGBA(color);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
	glm::vec3 lightDirection = glm::normalize(glm::vec3(-1, -1 ,-1));
	float radius = 0.5f;

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2*glm::dot(ray.Direction, ray.Origin);
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;

	if(discriminant<0.0f)
		return glm::vec4(0,0,0,1);

	float closestHit = (- b - glm::sqrt(discriminant)) / (2.0f * a);
	glm::vec3 hitPoint = ray.Origin + ray.Direction * closestHit;
	glm::vec3 normal = glm::normalize(hitPoint);
	float dot = glm::max(glm::dot(normal, -lightDirection), 0.0f);

	glm::vec3 sphereColor(1, 0, 1);
	sphereColor *= dot;

	return glm::vec4(sphereColor, 1.0f);
}
