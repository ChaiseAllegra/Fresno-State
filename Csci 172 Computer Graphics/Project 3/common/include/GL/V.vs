
uniform float Scale;
void main(void)
{
    vec4 a = gl_Vertex;
    a.x = a.x * 1.5;
    a.y = a.y * 1.5;
    gl_Position = gl_ModelViewProjectionMatrix * a;
}
