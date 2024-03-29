#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 edgePosition0;
layout (location = 2) in vec3 edgePosition1;
layout (location = 3) in vec3 edgePosition2;
layout (location = 4) in vec3 edgePosition3;
layout (location = 5) in float tJunctionFlag;

uniform mat4 viewProjectionMatrix;
uniform vec3 planetCenter;
uniform vec3 edgePositions[4];
uniform int perm[512];
uniform int perlinNoiseCount;
uniform int gridSize;

vec3 positionGlobal;
out VS_OUT 
{
    float height;
	vec3 position;
	vec3 color;
} vs_out;

float fade(float t) 
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) 
{
	return a + t * (b - a);
}

float grad(int hash, float x, float y, float z) 
{
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	float u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noise(float x, float y, float z) 
{
	int X = int(floor(x)) & 255;
	int Y = int(floor(y)) & 255;
	int Z = int(floor(z)) & 255;

	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	int A = perm[X] + Y;
	int AA = perm[A] + Z;
	int AB = perm[A + 1] + Z;
	int B = perm[X + 1] + Y;
	int BA = perm[B] + Z;
	int BB = perm[B + 1] + Z;

	float res = lerp(w, lerp(v, lerp(u, grad(perm[AA], x, y, z), grad(perm[BA], x - 1, y, z)), lerp(u, grad(perm[AB], x, y - 1, z), grad(perm[BB], x - 1, y - 1, z))), lerp(v, lerp(u, grad(perm[AA + 1], x, y, z - 1), grad(perm[BA + 1], x - 1, y, z - 1)), lerp(u, grad(perm[AB + 1], x, y - 1, z - 1), grad(perm[BB + 1], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0;
}

vec3 Interpolate3D(vec3 p1, vec3 p2, vec3 p3, vec3 p4)
{
	vec3 pm1 = mix(p1,p2, positionGlobal.x);
	vec3 pm2 = mix(p4,p3, positionGlobal.x);
	return mix(pm1, pm2, positionGlobal.y);
}

int Modulo(int number, int modulo)
{
	return number - (number / modulo) * modulo;
}

void ManageTJunctions()
{
	uint flag = floatBitsToUint(tJunctionFlag);
	positionGlobal = position;
	uint bit0 = flag & 0x1;
	uint bit1 = flag & 0x2;
	uint bit2 = flag & 0x4;
	uint bit3 = flag & 0x8;
	
	if(bit0 == 1 && Modulo(gl_VertexID + gridSize , gridSize * 2) == 0)
	{
		positionGlobal.x -= 1.0f / (gridSize-1);
	}
	if(bit1 == 2 && gl_VertexID < gridSize && Modulo(gl_VertexID,2) == 1)
	{
		positionGlobal.y -= 1.0f / (gridSize-1);
	}
	if(bit2 == 4 && Modulo(gl_VertexID, gridSize * 2) == gridSize * 2 -1)
	{
		positionGlobal.x -= 1.0f / (gridSize-1);
	}
	if(bit3 == 8 && gl_VertexID > gridSize * gridSize - gridSize && Modulo(gl_VertexID, 2) == 1)
	{
		positionGlobal.y -= 1.0f / (gridSize-1);
	}
}

void main()
{
	int nInstance = gl_InstanceID; 
	ManageTJunctions();
	vec4 pos = vec4(Interpolate3D(edgePosition0, edgePosition1, edgePosition2, edgePosition3),1);
	pos = vec4(normalize(pos.xyz) + planetCenter,1);
	vs_out.position = pos.xyz;
	float perlin = 0;
	for(float i = 1; i< perlinNoiseCount+1; ++i)
	{
		float pNoise = noise(pos.x * i * i, pos.y * i * i, pos.z * i *i);
		perlin += (pNoise - 0.5f) / i * 0.05f;
	}
	perlin = max(0.01f,perlin);
	pos = vec4(pos.xyz * perlin + pos.xyz,1);

	//vs_out.height = perlin;
	
	vec4 vertexPositionVS = viewProjectionMatrix * pos;
	
	gl_Position = vertexPositionVS;
}
