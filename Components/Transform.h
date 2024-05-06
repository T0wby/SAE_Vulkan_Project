#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include <glm/glm/glm.hpp>

class CTransform : public IComponent
{
public:
	CTransform() = default;
	CTransform(const CTransform&) = default;
	CTransform(CTransform&&) = default;
	CTransform& operator= (const CTransform&) = default;
	CTransform& operator= (CTransform&&) = default;
	~CTransform() = default;

	// Inherited via IComponent
	virtual int Initialize(void) override;
	virtual int Initialize(const VkCommandBuffer& a_commandBuffer) override;
	virtual int Update(const double& a_dDeltaTime) override;
	virtual void Draw(void) override;
	virtual void Draw(const DrawInformation& a_drawInformation) override;
	virtual void Finalize(void) override;

	inline auto GetTransformMatrix(void) const -> const glm::mat4x4 { return m_transformMatrix; }
	inline auto GetInverseScaleMatrix(void) const -> const glm::mat3x3 { return CalcInverseScale(); }
	inline auto GetPosition(void) const -> const glm::vec3 { return m_position; }
	inline void AddPosition(glm::vec3 a_pos){ m_position += a_pos; }
	inline void SetPosition(glm::vec3 a_pos){ m_position = a_pos; }
	inline void AddRotation(glm::vec3 a_rotation){ m_rotation += a_rotation; }
	inline void SetRotation(glm::vec3 a_rotation){ m_rotation = a_rotation; }
	inline void AddScale(glm::vec3 a_scale){ m_scale += a_scale; }
	inline void SetScale(glm::vec3 a_scale){ m_scale = a_scale; }

private:
	glm::vec3 m_position{0.0f,0.0f,0.0f};
	glm::vec3 m_rotation{0.0f,0.0f,0.0f};
	glm::vec3 m_scale{ 1.0f,1.0f,1.0f };
	glm::mat4x4 m_transformMatrix{};
	glm::mat4x4 m_positionMatrix{};
	glm::mat4x4 m_rotationMatrix{};
	glm::mat4x4 m_scaleMatrix{};

	auto CalcInverseScale(void) const -> const glm::mat3x3;
};
#endif // !TRANSFORM_H
