varying vec4 the_color;
varying vec3 v;
varying vec3 N;

void main (void)  
{	


	float color2;
	float color1;
	vec3 L1 = normalize(gl_LightSource[1].position.xyz - v);
	float angle1 = max(dot(L1, N), 0.0);
	float factor = 0.4;
	vec3 L2 = normalize(gl_LightSource[2].position.xyz - v);
	float angle2tmp = max(dot(L2, N), 0.0);
	float angle2 = -1.0;
	if (angle2tmp > 0.0)
	{
		float spotEffect = dot(normalize(gl_LightSource[2].spotDirection), -L2);
		// cos(2.5degrees)
		if (spotEffect < 0.999)
		{
			angle2 = 1.0;
		}
	}
	if (gl_LightSource[1].diffuse == vec4(0.0, 0.0, 0.0, 0.0))
	{
		angle1 = -1.0;
	}
	if (gl_LightSource[2].diffuse == vec4(0.0, 0.0, 0.0, 0.0))
	{
		angle2 = -1.0;

	}
	if (angle1 > 0.0 && angle2 > 0.0)
    {  
		if (the_color[0]*(1.0+factor) > 1.0)
		{
			color1 = 1.0;

		}
		else
		{
			color1 = the_color[0]*(1.0+factor);
		}
		if (the_color[1]*(1.0+factor) > 1.0)
		{
			color2 = 1.0;
		}
		else
		{
			color2 = the_color[1]*(1.0+factor);
		}
		gl_FragColor = vec4(color1, color2, the_color[2]*(1.0-factor), the_color[3]*(1.0-factor));
	}
	if (angle1 > 0.0 && angle2 <= 0.0)
	{  
		if (the_color[0]*(1.0+factor) > 1.0)
		{
			gl_FragColor = vec4(1.0, the_color[1]*(1.0-factor), the_color[2]*(1.0-factor), the_color[3]*(1.0-factor));
		}
		else
		{
			gl_FragColor = vec4(the_color[0]*(1.0+factor), the_color[1]*(1.0-factor), the_color[2]*(1.0-factor), the_color[3]*(1.0-factor));
		}
	}
	if (angle1 <= 0.0 && angle2 > 0.0)
	{  
		if (the_color[1]*(1.0+factor) > 1.0)
		{
			gl_FragColor = vec4(the_color[0]*(1.0-factor), 1.0, the_color[2]*(1.0-factor), the_color[3]*(1.0-factor));
		}
		else
		{
			gl_FragColor = vec4(the_color[0]*(1.0-factor), the_color[1]*(1.0+factor), the_color[2]*(1.0-factor), the_color[3]*(1.0-factor));
		}
	}
	if (angle1 <= 0.0 && angle2 <= 0.0)
	{	
		gl_FragColor = the_color;
	}
}