#pragma once

namespace Zero::Gpu
{

struct Buffer;
struct Program;
struct Texture;
struct Query;
struct BindGroup;

using BufferHandle = Buffer*;
using ProgramHandle = Program*;
using TextureHandle = Texture*;
using QueryHandle = Query*;
using BindGroupHandle = BindGroup*;
const BufferHandle INVALID_BUFFER = nullptr;
const ProgramHandle INVALID_PROGRAM = nullptr;
const TextureHandle INVALID_TEXTURE = nullptr;
const QueryHandle INVALID_QUERY = nullptr;
const BindGroupHandle INVALID_BIND_GROUP = nullptr;

DeclareBitField2(InitFlags,
                 // NONE,
                 DEBUG_OUTPUT,
                 VSYNC);

DeclareBitField2(FramebufferFlags,
                 // NONE,
                 SRGB,
                 READONLY_DEPTH_STENCIL);

/* 16 bits reserved for blending*/
/* 40 bits reserver for stencil*/
// enum class StateFlags : u64 {
//	NONE = 0,
//	WIREFRAME = 1 << 0,
//	DEPTH_FN_GREATER = 1 << 1,
//	DEPTH_FN_EQUAL = 1 << 2,
//	DEPTH_FUNCTION = DEPTH_FN_GREATER | DEPTH_FN_EQUAL,
//	CULL_FRONT = 1 << 3,
//	CULL_BACK = 1 << 4,
//	SCISSOR_TEST = 1 << 5,
//	DEPTH_WRITE = 1 << 6,
//
// };

DeclareBitField7(StateFlags,
                 // NONE,
                 WIREFRAME,
                 DEPTH_FN_GREATER,
                 DEPTH_FN_EQUAL,
                 // DEPTH_FUNCTION = DEPTH_FN_GREATER | DEPTH_FN_EQUAL,
                 CULL_FRONT,
                 CULL_BACK,
                 SCISSOR_TEST,
                 DEPTH_WRITE);

DeclareEnum2(QueryType, TIMESTAMP, STATS);

DeclareBitField2(MemoryBarrierType, SSBO, COMMAND);

DeclareEnum5(PrimitiveType,
             TRIANGLES,
             TRIANGLE_STRIP,
             LINES,
             POINTS,

             NONE);

DeclareEnum4(ShaderType, VERTEX, FRAGMENT, GEOMETRY, COMPUTE);

DeclareBitField3(ClearFlags, COLOR, DEPTH, STENCIL

                 //,ALL = COLOR | DEPTH | STENCIL,
);

DeclareEnum4(StencilFuncs, DISABLE, ALWAYS, EQUAL, NOT_EQUAL);

DeclareEnum8(StencilOps, KEEP, ZERO, REPLACE, INCR, INCR_WRAP, DECR, DECR_WRAP, INVERT);

DeclareEnum14(BlendFactors,
              ZERO,
              ONE,
              SRC_COLOR,
              ONE_MINUS_SRC_COLOR,
              SRC_ALPHA,
              ONE_MINUS_SRC_ALPHA,
              DST_COLOR,
              ONE_MINUS_DST_COLOR,
              DST_ALPHA,
              ONE_MINUS_DST_ALPHA,
              SRC1_COLOR,
              ONE_MINUS_SRC1_COLOR,
              SRC1_ALPHA,
              ONE_MINUS_SRC1_ALPHA);

DeclareEnum4(AttributeType, U8, FLOAT, I16, I8);

// keep order, this is serialized
DeclareEnum24(TextureFormat,
              R8,
              RG8,
              D32,
              D24S8,
              RGBA8,
              RGBA16,
              RGBA16F,
              RGBA32F,
              BGRA8,
              R16F,
              R16,
              R32F,
              RG32F,
              SRGB,
              SRGBA,
              BC1,
              BC2,
              BC3,
              BC4,
              BC5,
              R11G11B10F,
              RGB32F,
              RG16,
              RG16F);

DeclareBitField1(BindShaderBufferFlags,
                 // NONE, // 0
                 OUTPUT // 1 << 0
);

DeclareBitField12(TextureFlags,
                  // NONE = 0,
                  POINT_FILTER,
                  CLAMP_U,
                  CLAMP_V,
                  CLAMP_W,
                  ANISOTROPIC_FILTER,
                  NO_MIPS,
                  SRGB,
                  READBACK,
                  IS_3D,
                  IS_CUBE,
                  COMPUTE_WRITE,
                  RENDER_TARGET );

DeclareBitField5(BufferFlags,
                 // NONE = 0,
                 IMMUTABLE,
                 UNIFORM_BUFFER,
                 SHADER_BUFFER,
                 COMPUTE_WRITE,
                 MAPPABLE);

DeclareEnum2(DataType, U16, U32);

#pragma pack(1)
struct Attribute
{
  enum Flags
  {
    NORMALIZED = 1 << 0,
    AS_INT = 1 << 1,
    INSTANCED = 1 << 2
  };
  u8 components_count;
  u8 byte_offset;
  AttributeType::Enum type;
  u8 flags;

  size_t Hash() const;
};
#pragma pack()

struct VertexDecl
{
  enum
  {
    MAX_ATTRIBUTES = 16
  };

  VertexDecl(PrimitiveType::Enum pt);
  void addAttribute(u8 byte_offset, u8 components_num, AttributeType::Enum type, u8 flags);
  u32 getStride() const;
  void computeHash();
  void setPrimitiveType(PrimitiveType::Enum type);

  u8 attributes_count = 0;
  u32 hash;
  Attribute attributes[MAX_ATTRIBUTES];
  PrimitiveType::Enum primitive_type = PrimitiveType::NONE;
};

size_t Hash(Attribute attributes[(u32)VertexDecl::MAX_ATTRIBUTES], u32 count);

struct TextureDesc
{
  TextureFormat::Enum format;
  u32 width;
  u32 height;
  u32 depth;
  u32 mips;
  bool is_cubemap;
};

struct MemoryStats
{
  u64 total_available_mem;
  u64 current_available_mem;
  u64 dedicated_vidmem;
  u64 render_target_mem;
  u64 buffer_mem;
  u64 texture_mem;
};

struct BindGroupEntryDesc
{
  enum Type
  {
    UNIFORM_BUFFER,
    TEXTURE
  };
  Type type;
  union
  {
    BufferHandle buffer;
    TextureHandle texture;
  };
  u32 bind_point;
  u32 offset;
  u32 size;
};

void preinit(bool load_renderdoc);
bool init(void* window_handle, BitField<InitFlags::Enum> flags);
void captureRenderDocFrame();
bool getMemoryStats(MemoryStats& stats);
u32 swapBuffers();
void waitFrame(u32 frame);
bool frameFinished(u32 frame);
ZeroShared bool isOriginBottomLeft();
void checkThread();
void shutdown();
int getSize(AttributeType::Enum type);
u32 getSize(TextureFormat::Enum format, u32 w, u32 h);
u32 getBytesPerPixel(TextureFormat::Enum format);

TextureHandle allocTextureHandle();
BufferHandle allocBufferHandle();
ProgramHandle allocProgramHandle();
BindGroupHandle allocBindGroupHandle();

void createBuffer(BufferHandle handle, BitField<BufferFlags::Enum> flags, size_t size, const void* data);
QueryHandle createQuery(QueryType::Enum type);

void createProgram(ProgramHandle prog,
                   BitField<StateFlags::Enum> state,
                   const VertexDecl& decl,
                   const char** srcs,
                   const ShaderType::Enum* types,
                   u32 num,
                   const char** prefixes,
                   u32 prefixes_count,
                   const char* name);
void createBuffer(BufferHandle buffer, BitField<BufferFlags::Enum> flags, size_t size, const void* data);
void createTexture(TextureHandle handle,
                   u32 w,
                   u32 h,
                   u32 depth,
                   TextureFormat::Enum format,
                   BitField<TextureFlags::Enum> flags,
                   const char* debug_name);
void createTextureView(TextureHandle view, TextureHandle texture, u32 layer);
void createBindGroup(BindGroupHandle group, Span<const BindGroupEntryDesc> descs);

void destroy(TextureHandle texture);
void destroy(BufferHandle buffer);
void destroy(ProgramHandle program);
void destroy(BindGroupHandle group);

void setCurrentWindow(void* window_handle);
void setFramebuffer(const TextureHandle* attachments, u32 num, TextureHandle ds, FramebufferFlags::Enum flags);
void setFramebufferCube(TextureHandle cube, u32 face, u32 mip);
void viewport(u32 x, u32 y, u32 w, u32 h);
void scissor(u32 x, u32 y, u32 w, u32 h);
void clear(BitField<ClearFlags::Enum> flags, const float* color, float depth);

void startCapture();
void stopCapture();
void pushDebugGroup(const char* msg);
void popDebugGroup();

void useProgram(ProgramHandle program);

void bind(BindGroupHandle group);
void bindIndexBuffer(BufferHandle buffer);
void bindVertexBuffer(u32 binding_idx, BufferHandle buffer, u32 buffer_offset, u32 stride);
void bindTextures(const TextureHandle* handles, u32 offset, u32 count);
void bindUniformBuffer(u32 ub_index, BufferHandle buffer, size_t offset, size_t size);
void bindIndirectBuffer(BufferHandle buffer);
void bindShaderBuffer(BufferHandle buffer, u32 binding_idx, BitField<BindShaderBufferFlags::Enum> flags);
void bindImageTexture(TextureHandle texture, u32 unit);

void drawArrays(u32 offset, u32 count);
void drawIndirect(DataType::Enum index_type, u32 indirect_buffer_offset);
void drawIndexed(u32 offset, u32 count, DataType::Enum type);
void drawArraysInstanced(u32 indices_count, u32 instances_count);
void drawIndexedInstanced(u32 indices_count, u32 instances_count, DataType::Enum index_type);
void dispatch(u32 num_groups_x, u32 num_groups_y, u32 num_groups_z);

void memoryBarrier(BitField<MemoryBarrierType::Enum> type, BufferHandle);

void copy(TextureHandle dst, TextureHandle src, u32 dst_x, u32 dst_y);
void copy(BufferHandle dst, BufferHandle src, u32 dst_offset, u32 src_offset, u32 size);

void readTexture(TextureHandle texture, u32 mip, Span<u8> buf);
void generateMipmaps(TextureHandle texture);
void setDebugName(TextureHandle texture, const char* debug_name);

void update(TextureHandle texture,
            u32 mip,
            u32 x,
            u32 y,
            u32 z,
            u32 w,
            u32 h,
            TextureFormat::Enum format,
            const void* buf,
            u32 size);
void update(BufferHandle buffer, const void* data, size_t size);

void* map(BufferHandle buffer, size_t size);
void unmap(BufferHandle buffer);
void queryTimestamp(QueryHandle query);

void beginQuery(QueryHandle query);
void endQuery(QueryHandle query);
u64 getQueryResult(QueryHandle query);
u64 getQueryFrequency();
bool isQueryReady(QueryHandle query);

void destroy(BufferHandle buffer);
void destroy(QueryHandle query);

void pushDebugGroup(const char* msg);
void popDebugGroup();

inline BitField<StateFlags::Enum> getBlendStateBits(BlendFactors::Enum src_rgb,
                                                    BlendFactors::Enum dst_rgb,
                                                    BlendFactors::Enum src_a,
                                                    BlendFactors::Enum dst_a)
{
  return BitField<StateFlags::Enum>((((u64)src_rgb & 15) << 7) | (((u64)dst_rgb & 15) << 11) |
                                    (((u64)src_a & 15) << 15) | (((u64)dst_a & 15) << 19));
}

inline BitField<StateFlags::Enum> getStencilStateBits(u8 write_mask,
                                                      StencilFuncs::Enum func,
                                                      u8 ref,
                                                      u8 mask,
                                                      StencilOps::Enum sfail,
                                                      StencilOps::Enum dpfail,
                                                      StencilOps::Enum dppass)
{
  return BitField<StateFlags::Enum>(((u64)write_mask << 23) | ((u64)func << 31) | ((u64)ref << 35) | ((u64)mask << 43) |
                                    ((u64)sfail << 51) | ((u64)dpfail << 55) | ((u64)dppass << 59));
}
} // namespace Zero::Gpu