#version 330 core

in vec3 Normal_cameraspace;
in vec3 eyeDirection;
in vec3 lightDirection;

// Ouput data
out vec3 color;

uniform vec3 spotlightDirection;

void main()
{
	vec3 LightColor = vec3(1,1,1);
	vec3 spec = vec3(0.0);
	float shininess = 1000.0;

	vec3 MaterialDiffuseColor = vec3(1.0, 1.0, 1.0);
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	//Spotlights
	float intensity = 0.0;
	float l_spotCutOff = 0.999;

	vec3 ld = normalize(lightDirection);
	vec3 sd = normalize(spotlightDirection); //direccion del spotlight

	// inside the cone?
	if (dot(sd,ld) > l_spotCutOff) 
	{
		vec3 n = normalize(Normal_cameraspace);
		intensity = max(dot(n,ld), 0.0);

		if (intensity > 0.0) 
		{
			vec3 eye = normalize(eyeDirection);
			vec3 h = normalize(ld + eye);
			float intSpec = max(dot(h,n), 0.0);
			spec = MaterialSpecularColor * pow(intSpec, shininess);
		}
    	color = max(intensity * MaterialDiffuseColor + spec, MaterialAmbientColor);
	}


}