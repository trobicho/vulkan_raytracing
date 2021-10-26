#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

layout(origin_upper_left) in vec4 gl_FragCoord;

#define MAX_ITER	1000

int		mandelbrot(vec2 z)
{
	float	zrs;
	float	zis;
	float	zir;
	vec2	c = z;

	for (int i = 0; i < MAX_ITER; i++)
	{
		zrs = c.x * c.x;
		zis = c.y * c.y;
		zir = c.x * c.y;
		if (zrs + zis > 4.0)
			return (i);
		c.x = zrs - zis + z.x;
		c.y = zir + zir + z.y;
	}
	return (MAX_ITER);
}

void	main()
{
	vec3	color;
	vec2	uv = vec2(gl_FragCoord.xy);
	float	coloradd = 0.203005;

	//vec2 min = vec2(0.278587, -0.012560);
	//vec2 max = vec2(0.285413, -0.007440);
	vec2 min = vec2(-2.5, -2.0);
	vec2 max = vec2(1.5, 2.0);
	vec2 z = uv * (max - min) + min;
	vec2 c = mix(vec2(min.x, min.y), vec2(max.x, max.y), gl_FragCoord.xy
		/ vec2(800, 800));

	int col = mandelbrot(c);
	if (col == MAX_ITER)
		color = vec3(0.0, 0.0, 0.0);
	else
	{
		col %= 30;
		if (col >= 15)
			col = 30 - col;
		color = vec3(1.0 / col);
	}
	outColor = vec4(color, 1.0);
}
