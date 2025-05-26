#version 410 core
out vec4 FragColor;

in vec2 texcoord;

uniform sampler2D screenTexture;

void main()
{
     float bit = texture(screenTexture, texcoord).r;
     FragColor = vec4(bit,bit,bit,1.0f);
}
