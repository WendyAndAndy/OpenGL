#version 330 core
in vec3 ourColor; //in b.vs (from vertex attribute)
out vec4 fragColor;

in vec3 outColor; //using vertex shader position as color

void main()
{
    fragColor = vec4(ourColor, 1);

    //fragColor = vec4(outColor, 1);
}