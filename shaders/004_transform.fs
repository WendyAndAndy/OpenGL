#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;     //for mix
uniform float mixValue;

void main()
{
	//FragColor = texture(texture1, TexCoord);
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.f);
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixValue) * vec4(ourColor, 1.f);
}