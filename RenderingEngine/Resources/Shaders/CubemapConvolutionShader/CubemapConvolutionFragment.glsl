#version 400 core

in vec3 FragPosition;

out vec4 FragColor;

uniform samplerCube Cubemap;

const float PI = 3.14159265359;

void main()
{
	// the sample direction equals the hemisphere's orientation 
	vec3 Normal = normalize(FragPosition);

	vec3 Irradiance = vec3(0.0);

	// Convolution code
	vec3 Up = vec3(0.0, 1.0, 0.0);
	vec3 Right = cross(Up, Normal);
	Up = cross(Normal, Right);

	float SampleDelta = 0.025;
	float NumOfSamples = 0.0;
	for (float Phi = 0.0; Phi < 2.0 * PI; Phi += SampleDelta)
	{
		for (float Theta = 0.0; Theta < 0.5 * PI; Theta += SampleDelta)
		{
			// spherical to cartesian (in tangent space)
			vec3 TangentSample = vec3(sin(Theta) * cos(Phi), sin(Theta) * sin(Phi), cos(Theta));
			// tangent space to world
			vec3 SampleVec = TangentSample.x * Right + TangentSample.y * Up + TangentSample.z * Normal;

			Irradiance += texture(Cubemap, SampleVec).rgb * cos(Theta) * sin(Theta);
			NumOfSamples++;
		}
	}
	Irradiance = PI * Irradiance * (1.0 / float(NumOfSamples));
	FragColor = vec4(Irradiance, 1.0);
}