#version 460

out vec4 color;
uniform float time;

const float MAX_ITER = 128.0;

float mandelbrot(vec2 uv) {
	vec2 c = 4 * uv - vec2(0.7, 0.0);
	c = c / pow(time, 4.0) - vec2(0.65, 0.45);
	vec2 z = vec2(0.0);
	float iter = 0.0;
	for (float i; i < MAX_ITER; i++) {
		z = vec2(z.x * z.x - z.y * z.y,
				 2.0 * z.x * z.y) + c;
	    if (dot(z, z) >	4.0) return iter / MAX_ITER;
		iter++;
	}
	return 0.0;
}

vec3 hash13(float m) {
	float x = fract(sin(m) * 5565.746);
	float y = fract(sin(m + x) * 2219.486);
	float z = fract(sin(x + y) * 4576.3);
	return vec3(x, y, z);
}

void main() {
	vec2 resolution = vec2(1080, 1080);
	vec2 uv = (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.y;
	vec3 col = vec3(0.0);

	float m = mandelbrot(uv);
	col += m;
	//col += hash13(m);
	col = pow(col, vec3(0.25));
	color = vec4(col, 0.0);
}