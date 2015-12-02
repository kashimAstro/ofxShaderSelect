-- VERTEX
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

in vec4 vPosition;
in vec3 vNormal;

smooth out vec3 vEyePos_TCS;
smooth out vec3 vNorm_TCS;

void main()
{
    vNorm_TCS = normalize(normalMatrix * vNormal);
    vEyePos_TCS = vec3(mvMatrix * vPosition);
}

-- FRAGMENT
uniform vec3 vLightPosition;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

in vec3 vEyePos_FS;
in vec3 vNorm_FS;
in vec3 gTriDistance;
in vec3 gPatchDistance;
in vec3 gFacetNormal;
out vec4 fragColor;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2 * d * d);
    return d;
}

void main()
{
    vec3 N = normalize(gFacetNormal);
    vec3 L = vLightPosition;
    float df = abs(dot(N, L));
    vec4 color = ambientColor + df * diffuseColor;

    fragColor = vec4(0.,0.,0.,1.);
    float d1 = min(min(gTriDistance.x, gTriDistance.y), gTriDistance.z);
    float d2 = min(min(gPatchDistance.x, gPatchDistance.y), gPatchDistance.z);
    color = amplify(d1, 10, -0.5) * amplify(d2, 100, 0.5) * color;

    fragColor+= color;
};

-- GEOMETRY
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat3 normalMatrix;

in vec3 vEyePos_GS[];
in vec3 vNorm_GS[];
in vec3 tePatchDistance[3];
in vec3 tePosition[3];

out vec3 vEyePos_FS;
out vec3 vNorm_FS;
out vec3 gTriDistance;
out vec3 gPatchDistance;
out vec3 gFacetNormal;

void main()
{
    vec3 A = tePosition[2] - tePosition[0];
    vec3 B = tePosition[1] - tePosition[0];
    gFacetNormal = normalMatrix * normalize(cross(A, B));

    vNorm_FS = vNorm_GS[0];
    vEyePos_FS = vEyePos_GS[0];
    gPatchDistance = tePatchDistance[0];

    gTriDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    vNorm_FS = vNorm_GS[1];
    vEyePos_FS = vEyePos_GS[1];
    gPatchDistance = tePatchDistance[1];

    gTriDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    vNorm_FS = vNorm_GS[2];
    vEyePos_FS = vEyePos_GS[2];
    gPatchDistance = tePatchDistance[2];

    gTriDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}

-- TESSCONTROL
layout (vertices = 3) out;

uniform vec3 eyePos;
uniform float TessLevelInner;
uniform float TessLevelOuter;
in vec3 vEyePos_TCS[];
in vec3 vNorm_TCS[];
out vec3 vEyePos_TES[];
out vec3 vNorm_TES[];
out vec3 tcPosition[];

float GetTessLevel(float dist0, float dist1){
    float avgDist = (dist0 + dist1) / 2.0;
    if (avgDist <= 10.0)
        return 5.0;
    else if (avgDist <= 15.0)
        return 3.0;
    else
        return 1.0;
}

void main()
{
    tcPosition[gl_InvocationID] = vEyePos_TCS[gl_InvocationID];

    vEyePos_TES[gl_InvocationID] = vEyePos_TCS[gl_InvocationID];
    vNorm_TES[gl_InvocationID] = vNorm_TCS[gl_InvocationID];
    float dist0 = distance(eyePos, vEyePos_TES[0]);
    float dist1 = distance(eyePos, vEyePos_TES[1]);
    float dist2 = distance(eyePos, vEyePos_TES[2]);
    gl_TessLevelInner[0] = TessLevelInner;
    gl_TessLevelOuter[0] = TessLevelOuter;//GetTessLevel(dist1, dist2)
    gl_TessLevelOuter[1] = TessLevelOuter;//GetTessLevel(dist2, dist0)
    gl_TessLevelOuter[2] = TessLevelOuter;//GetTessLevel(dist0, dist1)

}

-- TESSEVAL
layout(triangles, equal_spacing, ccw) in;

uniform mat4 pMatrix;

in vec3 vEyePos_TES[];
in vec3 vNorm_TES[];
in vec3 tcPosition[];

out vec3 tePosition;
out vec3 vEyePos_GS;
out vec3 vNorm_GS;
out vec3 tePatchDistance;

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main()
{
    vNorm_GS = interpolate3D(vNorm_TES[0], vNorm_TES[1], vNorm_TES[2]);
    vEyePos_GS = interpolate3D(vEyePos_TES[0], vEyePos_TES[1], vEyePos_TES[2]);
    tePatchDistance = gl_TessCoord;

    vec3 p0 = gl_TessCoord.x * vEyePos_TES[0];
    vec3 p1 = gl_TessCoord.y * vEyePos_TES[1];
    vec3 p2 = gl_TessCoord.z * vEyePos_TES[2];
    tePosition = normalize(p0 + p1 + p2);

    gl_Position = pMatrix * vec4(vEyePos_GS, 1.0);
}
