#version 460 core

in vec3 vertexColor;

out vec4 FragColor;

void main()
{
    // Set the fragment color
    FragColor = vec4(vertexColor, 1.0f); // RGBA color
}
