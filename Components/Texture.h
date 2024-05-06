#ifndef MESH_H
#define MESH_H
#include <memory>
#include "Component.h"
#include "../Utility/CoreStructs.h"
#include "../Core/System/Device.h"
#include "../Core/System/Descriptors.h"

class CTexture : public IComponent
{
public:
	inline CTexture(const std::shared_ptr<CDevice>& a_pDevice, const std::string& a_texFilePath)
		: m_pDevice(a_pDevice), m_texFilePath(a_texFilePath){}
	CTexture(const CTexture&) = delete;
	CTexture(CTexture&&) = default;
	CTexture& operator= (const CTexture&) = delete;
	CTexture& operator= (CTexture&&) = default;
	~CTexture();
	
	// Inherited via IComponent
	int Initialize(void) override;
	int Initialize(const VkCommandBuffer& a_commandBuffer) override;
	int Update(const double& a_dDeltaTime) override;
	void Draw(void) override;
	void Draw(const DrawInformation& a_drawInformation) override;
	void Finalize(void) override;


private:
	std::shared_ptr<CDevice> m_pDevice{nullptr};
	VkImage m_textureImage{};
	VkDeviceMemory m_textureImageMemory{};
	VkImageView m_textureImageView{};
	VkSampler m_textureSampler{};
	std::string m_texFilePath{};

	
	std::unique_ptr<CDescriptorSetLayout> m_pDescriptorSetLayout{nullptr};
	VkDescriptorSet m_descriptorSet{nullptr};

	void CreateTextureImage(const std::string& a_texFilePath);
	void CopyBufferToImage(const VkBuffer& a_buffer,const VkImage& a_image,const uint32_t& a_width,const uint32_t& a_height);
	void CreateTextureImageView(void);
	void CreateTextureSampler(void);
	void CreateImage(uint32_t a_width, uint32_t a_height, VkFormat a_format, VkImageTiling a_tiling, VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties, VkImage& a_image, VkDeviceMemory& a_imageMemory);
	VkImageView CreateImageView(VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspectFlags);
	void TransitionImageLayout(VkImage a_image, VkFormat a_format, VkImageLayout a_oldLayout, VkImageLayout a_newLayout);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	bool HasStencilComponent(VkFormat a_format);
};
#endif
