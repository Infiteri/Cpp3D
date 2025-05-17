
#include "Geometry.h"

namespace Core
{
    BoxGeometry::BoxGeometry(float w, float h, float d) : Width(w), Height(h), Depth(d)
    {
        type = Box;
        Vertices.resize(24); // 6 faces × 4 vertices
        Indices.resize(36);  // 6 faces × 2 triangles × 3 indices

        float min_x = -Width;
        float max_x = Width;
        float min_y = -Height;
        float max_y = Height;
        float min_z = -Depth;
        float max_z = Depth;

        auto SetFace =
            [&](int faceIndex, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 normal)
        {
            float UV0 = 0.0f;
            float UV1 = 1.0f;

            int i = faceIndex * 4;
            Vertices[i + 0].Position = v0;
            Vertices[i + 1].Position = v1;
            Vertices[i + 2].Position = v2;
            Vertices[i + 3].Position = v3;

            Vertices[i + 0].UV.Set(UV0, UV0);
            Vertices[i + 1].UV.Set(UV1, UV0);
            Vertices[i + 2].UV.Set(UV1, UV1);
            Vertices[i + 3].UV.Set(UV0, UV1);

            Vertices[i + 0].Normal = normal;
            Vertices[i + 1].Normal = normal;
            Vertices[i + 2].Normal = normal;
            Vertices[i + 3].Normal = normal;
        };

        SetFace(0, {min_x, min_y, max_z}, {max_x, min_y, max_z}, {max_x, max_y, max_z},
                {min_x, max_y, max_z}, {0, 0, 1}); // Front

        SetFace(1, {max_x, min_y, min_z}, {min_x, min_y, min_z}, {min_x, max_y, min_z},
                {max_x, max_y, min_z}, {0, 0, -1}); // Back

        SetFace(2, {min_x, min_y, min_z}, {min_x, min_y, max_z}, {min_x, max_y, max_z},
                {min_x, max_y, min_z}, {-1, 0, 0}); // Left

        SetFace(3, {max_x, min_y, max_z}, {max_x, min_y, min_z}, {max_x, max_y, min_z},
                {max_x, max_y, max_z}, {1, 0, 0}); // Right

        SetFace(4, {min_x, min_y, min_z}, {max_x, min_y, min_z}, {max_x, min_y, max_z},
                {min_x, min_y, max_z}, {0, -1, 0}); // Bottom

        SetFace(5, {min_x, max_y, max_z}, {max_x, max_y, max_z}, {max_x, max_y, min_z},
                {min_x, max_y, min_z}, {0, 1, 0}); // Top

        // Build indices for each face
        for (u32 i = 0; i < 6; ++i)
        {
            u32 v_offset = i * 4;
            u32 i_offset = i * 6;
            Indices[i_offset + 0] = v_offset + 0;
            Indices[i_offset + 1] = v_offset + 1;
            Indices[i_offset + 2] = v_offset + 2;
            Indices[i_offset + 3] = v_offset + 0;
            Indices[i_offset + 4] = v_offset + 2;
            Indices[i_offset + 5] = v_offset + 3;
        }
    }
} // namespace Core
  // namespace Core
