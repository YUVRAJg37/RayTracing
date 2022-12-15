#include "Renderer.h"
#include "Walnut/Random.h"

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

void Renderer::Render()
{
	for(uint32_t y=0; y<m_FinalImage->GetHeight(); y++)
	{
		for(uint32_t x=0; x<m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;
			m_ImageData[x + y*m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t red = (uint8_t)(coord.x * 255.0f);
	uint8_t green = (uint8_t)(coord.y * 255.0f);

	glm::vec3 raydirection = { coord.x, coord.y, -1.0f };
	glm::vec3 rayOrigin{ 0.0f, 0.0f, 2.0f };
	float radius = 0.5f;

	float a = glm::dot(raydirection, raydirection);
	float b = 2*glm::dot(raydirection, rayOrigin);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float d = b * b - 4.0f * a * c;

	if(d>=0)
		return 0xffff00ff;
	return 0xff000000;
}
