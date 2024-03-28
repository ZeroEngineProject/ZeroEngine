#include "Precompiled.hpp"

namespace Zero::Gpu
{
u32 VertexDecl::getStride() const
{
  u32 stride = 0;
  for (u32 i = 0; i < attributes_count; ++i)
  {
    stride += attributes[i].components_count * getSize(attributes[i].type);
  }
  return stride;
}

void VertexDecl::computeHash()
{
  hash = Hash(attributes, attributes_count) ^ static_cast<u8>(primitive_type);
}

void VertexDecl::setPrimitiveType(PrimitiveType::Enum type)
{
  primitive_type = type;
  hash = Hash(attributes, attributes_count) ^ static_cast<u8>(primitive_type);
}

VertexDecl::VertexDecl(PrimitiveType::Enum pt)
{
  primitive_type = pt;
  hash = Hash(attributes, attributes_count) ^ static_cast<u8>(primitive_type);
}

void VertexDecl::addAttribute(u8 byte_offset, u8 components_num, AttributeType::Enum type, u8 flags)
{
  if (attributes_count >= lengthOf(attributes))
  {
    Error("Invalid attributes_count");
    return;
  }

  Attribute& attr = attributes[attributes_count];
  attr.components_count = components_num;
  attr.flags = flags;
  attr.type = type;
  attr.byte_offset = byte_offset;
  ++attributes_count;
  hash = Hash(attributes, attributes_count) ^ static_cast<u8>(primitive_type);
}

int getSize(AttributeType::Enum type)
{
  switch (type)
  {
  case AttributeType::FLOAT:
    return 4;
  case AttributeType::I8:
    return 1;
  case AttributeType::U8:
    return 1;
  case AttributeType::I16:
    return 2;
  }
  Error("Invalid AttributeType");
  return 0;
}

u32 getBytesPerPixel(TextureFormat::Enum format)
{
  switch (format)
  {
  case TextureFormat::R8:
    return 1;

  case TextureFormat::R16F:
  case TextureFormat::R16:
    return 2;
  case TextureFormat::SRGB:
    return 3;
  case TextureFormat::R11G11B10F:
  case TextureFormat::R32F:
  case TextureFormat::SRGBA:
  case TextureFormat::RGBA8:
    return 4;
  case TextureFormat::RGBA16:
  case TextureFormat::RGBA16F:
    return 8;
  case TextureFormat::RGBA32F:
    return 16;
  default:
    Error("Invalid TextureFormat");
    return 0;
  }
}
size_t Attribute::Hash() const
{
  size_t hash = 0;

  hash ^= (size_t)components_count;
  hash ^= (size_t)byte_offset;
  hash ^= (size_t)type;
  hash ^= (size_t)flags;

  return hash;
}
size_t Hash(Attribute attributes[(u32)VertexDecl::MAX_ATTRIBUTES], u32 count)
{
  size_t hash = 0;
  for (u32 i = 0; i < count; i++)
  {
    hash ^= attributes[i].Hash();
  }
  return hash;
}
} // namespace Zero::Gpu