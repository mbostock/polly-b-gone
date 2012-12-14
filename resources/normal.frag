varying vec3 normal;

void main() {
  vec3 n = normalize(normal) / 2.0 + vec3(0.5, 0.5, 0.5);
  gl_FragColor = vec4(n[0], n[1], n[2], 1.0);
}
