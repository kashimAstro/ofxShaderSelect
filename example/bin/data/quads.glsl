-- VERTEX

in vec4 Position;
out vec3 vPosition;

void main()
{
    vPosition = Position.xyz;
}

-- FRAGMENT

out vec4 FragColor;
in vec3 gFacetNormal;
in vec3 gTriDistance;
in vec4 gPatchDistance;

uniform vec3 LightPosition;
uniform vec3 DiffuseMaterial;
uniform vec3 AmbientMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;

const vec3 InnerLineColor = vec3(1, 1, 1);
const bool DrawLines = false;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void main()
{
    vec3 N = normalize(gFacetNormal);
    vec3 L = LightPosition;
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(L + E);

    float df = abs(dot(N, L));
    float sf = abs(dot(N, H));
    sf = pow(sf, Shininess);
    vec3 color = AmbientMaterial + df * DiffuseMaterial + sf * SpecularMaterial;

    if (DrawLines) {
        float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
        float d2 = min(min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z), gPatchDistance.w);
        d1 = 1 - amplify(d1, 50, -0.5);
        d2 = amplify(d2, 50, -0.5);
        color = d2 * color + d1 * d2 * InnerLineColor;
    }

    FragColor = vec4(color, 1.0);
}

-- GEOMETRY

uniform mat4 Modelview;
uniform mat3 NormalMatrix;
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
in vec3 tePosition[3];
in vec4 tePatchDistance[3];
out vec3 gFacetNormal;
out vec4 gPatchDistance;
out vec3 gTriDistance;

void main()
{
    vec3 A = tePosition[2] - tePosition[0];
    vec3 B = tePosition[1] - tePosition[0];
    gFacetNormal = NormalMatrix * normalize(cross(A, B));

    gPatchDistance = tePatchDistance[0];
    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[1];
    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    gPatchDistance = tePatchDistance[2];
    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}

-- TESSCONTROL

layout(vertices = 16) out;
in vec3 vPosition[];
out vec3 tcPosition[];
uniform float TessLevelInner;
uniform float TessLevelOuter;

#define ID gl_InvocationID

void main()
{
    tcPosition[ID] = vPosition[ID];

    if (ID == 0) {
        gl_TessLevelInner[0] = TessLevelInner;
        gl_TessLevelInner[1] = TessLevelInner;
        gl_TessLevelOuter[0] = TessLevelOuter;
        gl_TessLevelOuter[1] = TessLevelOuter;
        gl_TessLevelOuter[2] = TessLevelOuter;
        gl_TessLevelOuter[3] = TessLevelOuter;
    }
}

-- TESSEVAL

layout(quads) in;
in vec3 tcPosition[];
out vec3 tePosition;
out vec4 tePatchDistance;
uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 B;
uniform mat4 BT;

void main()
{
    float u = gl_TessCoord.x, v = gl_TessCoord.y;

    mat4 Px = mat4(
        tcPosition[0].x, tcPosition[1].x, tcPosition[2].x, tcPosition[3].x,
        tcPosition[4].x, tcPosition[5].x, tcPosition[6].x, tcPosition[7].x,
        tcPosition[8].x, tcPosition[9].x, tcPosition[10].x, tcPosition[11].x,
        tcPosition[12].x, tcPosition[13].x, tcPosition[14].x, tcPosition[15].x );

    mat4 Py = mat4(
        tcPosition[0].y, tcPosition[1].y, tcPosition[2].y, tcPosition[3].y,
        tcPosition[4].y, tcPosition[5].y, tcPosition[6].y, tcPosition[7].y,
        tcPosition[8].y, tcPosition[9].y, tcPosition[10].y, tcPosition[11].y,
        tcPosition[12].y, tcPosition[13].y, tcPosition[14].y, tcPosition[15].y );

    mat4 Pz = mat4(
        tcPosition[0].z, tcPosition[1].z, tcPosition[2].z, tcPosition[3].z,
        tcPosition[4].z, tcPosition[5].z, tcPosition[6].z, tcPosition[7].z,
        tcPosition[8].z, tcPosition[9].z, tcPosition[10].z, tcPosition[11].z,
        tcPosition[12].z, tcPosition[13].z, tcPosition[14].z, tcPosition[15].z );

    mat4 cx = B * Px * BT;
    mat4 cy = B * Py * BT;
    mat4 cz = B * Pz * BT;

    vec4 U = vec4(u*u*u, u*u, u, 1);
    vec4 V = vec4(v*v*v, v*v, v, 1);

    float x = dot(cx * V, U);
    float y = dot(cy * V, U);
    float z = dot(cz * V, U);
    tePosition =  vec3(x, y, z);

    tePatchDistance = vec4(u, v, 1-u, 1-v);
    gl_Position = Projection * Modelview * vec4(x, y, z, 1);
}

-- FLATTESSEVAL

layout(quads) in;
in vec3 tcPosition[];
out vec3 tePosition;
out vec4 tePatchDistance;
uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 B, BT;

void main()
{
    float u = gl_TessCoord.x, v = gl_TessCoord.y;
    vec3 a = mix(tcPosition[0], tcPosition[3], u);
    vec3 b = mix(tcPosition[12], tcPosition[15], u);
    tePosition = mix(a, b, v);
    tePatchDistance = vec4(u, v, 1-u, 1-v);
    gl_Position = Projection * Modelview * vec4(tePosition, 1);
}

