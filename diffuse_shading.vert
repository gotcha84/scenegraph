varying vec4 the_color;
varying vec3 v;
varying vec3 N;


void main(void)  
{     
	bool only1;
   // eye coordinate of vertex
   v = vec3(gl_ModelViewMatrix * gl_Vertex);
   
   // current color of pixel
   the_color = gl_Color;

   // normal vector
   N = normalize(gl_NormalMatrix * gl_Normal);

	vec4 shift0 = vec4(0, 0, 0, 0);
	vec4 shift1 = vec4(0, 0, 0, 0);
	vec4 shift2 = vec4(0, 0, 0, 0);

	float dist0;
	float dist1;
	float dist2;

	// CHANGE FOR MORE DRASTIC RESULTS
	float factor0tmp = 0.15;
	float factor1tmp = 0.15;
	float factor2tmp = 0.15;

	float factor0;
	float factor1;
	float factor2;

	vec3 L0tmp = normalize(gl_LightSource[0].position.xyz);
	vec3 L0 = -L0tmp;

	// MAYBE
	//float angle0 = max(dot(L0, N), 0.0);
	float angle0 = max(dot(L0tmp, N), 0.0);

	vec3 L1 = normalize(gl_LightSource[1].position.xyz - v);
	float angle1 = max(dot(L1, N), 0.0);
	vec3 L2 = normalize(gl_LightSource[2].position.xyz - v);
	float angle2tmp = max(dot(L2, N), 0.0);
	float angle2 = -1.0;

	dist1 = length(gl_LightSource[1].position.xyz - v);
	dist2 = length(gl_LightSource[2].position.xyz - v);

	factor0 = 1.0+factor0tmp;
	factor1 = 1.0+(factor1tmp/(dist1*dist1));
	factor2 = 1.0+(factor2tmp/(dist2*dist2));

	if (angle2tmp > 0.0)
	{
		float spotEffect = dot(normalize(gl_LightSource[2].spotDirection), -L2);
		// cos(2.5degrees)
		if (spotEffect < 0.999)
		{
			angle2 = 1.0;
		}
	}
	if (gl_LightSource[0].diffuse == vec4(0.0, 0.0, 0.0, 0.0))
	{
		angle0 = -1.0;
	}
	if (gl_LightSource[1].diffuse == vec4(0.0, 0.0, 0.0, 0.0))
	{
		angle1 = -1.0;
	}
	if (gl_LightSource[2].diffuse == vec4(0.0, 0.0, 0.0, 0.0))
	{
		angle2 = -1.0;
	}
	if (angle0 > 0.0)
	{
		// MIGHT BE OPPOSITE
		// repulse
		shift0 = vec4(-L0tmp[0]*factor0, -L0tmp[1]*factor0, -L0tmp[2]*factor0, 0.0);
	}

	if (angle1 > 0.0)
	{
		shift1 = vec4(-L1[0]*factor1, -L1[1]*factor1, -L1[2]*factor1, 0.0);
	}
	if (angle2 > 0.0)
	{
		shift2 = vec4(L2[0]*factor2, L2[1]*factor2, L2[2]*factor2, 0.0);
	}
	vec4 Vertex2 = gl_Vertex+shift0+shift1+shift2;
	gl_Position = gl_ModelViewProjectionMatrix * Vertex2;

	//gl_Position = ftransform();
}
