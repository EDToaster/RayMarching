#version 440


#define MOST_MARCHES 150
#define THRESH 0.00001

#define DE de_sphere
#define DE_UNIT de_sphere_unit
#define FOCAL -1

in vec3 vs_pos;

uniform vec3 cam_pos;
uniform vec3 cam_dir;

out vec4 fragColor;

// current pos, center, and radius
float de_sphere_unit(vec3 p) {
  // p.xy = mod((p.xy), 1.0) - vec2(0.5);
  // return length(p) - 0.3;

  // return length(mod(p, 4.0f) - vec3(2)) - 1;

  return (length(p - vec3(2)) - .7);
}

float de_tetra(vec3 z) {
  float r;
  int n = 0;
  while ( n < 4 ) {
    if (z.x + z.y < 0) z.xy = -z.yx; // fold # 1
    if (z.x + z.z < 0) z.xz = -z.zx; // fold # 2
    if (z.y + z.z < 0) z.zy = -z.yz; // fold # 3

    z = z * 2 - vec3(1) * (2 - 1);

    n++;
  }

  return (length(z)) * pow(2, -float(n));
}


float de_sphere(inout vec3 p) {
  p = mod(p, 4);

  return de_sphere_unit(p);
}


void ray_march(inout vec3 origin, vec3 dir, inout float s, inout float m, inout float d) {
  float total_dist = 0.0;

  s = 0;

  // closest it has gotten
  d = -1;

  for ( ; s < MOST_MARCHES; s++) {
    // origin = vec3(mod(origin.x, 4.0), origin.y, mod(origin.z, 4.0));

    float dist_to_next = DE(origin);
    if (d == -1 || dist_to_next < d) d = dist_to_next;

    origin += dist_to_next * dir;

    if (dist_to_next < THRESH) break;
  }

  m = MOST_MARCHES;
  // closest

  // return 1.0 - float(steps) / float (MOST_MARCHES);
}

void main() {

  // vec3 spec_dir = vec3(-1, 1, 1);


  vec3 center_of_screen = cam_pos;
  vec3 dir = normalize(cam_dir);

  vec3 focal_point = center_of_screen + dir * FOCAL;
  vec3 origin = vs_pos.xyz + center_of_screen;
  vec3 ray_dir = normalize(origin - focal_point);


  float s = 0;
  float m = 0;
  float d = 0;
  ray_march(origin, ray_dir, s, m, d);

  vec3 col = vec3(0);

  vec3 e = vec3(THRESH, 0, 0);

  vec3 normal = normalize(vec3(
    DE_UNIT(origin + e.xyy) - DE_UNIT(origin - e.xyy),
    DE_UNIT(origin + e.yxy) - DE_UNIT(origin - e.yxy),
    DE_UNIT(origin + e.yyx) - DE_UNIT(origin - e.yyx)
  ));



  if (s < m) {
    // hit
    col += (1.0 - float(s) / float(m)) * vec3(0.9, 0.6, 0.2);
    col += vec3(
      pow(
        clamp(dot(normal, normalize(vec3(-1, 1, -1))), 0, .7), 3
      )
    );
  } else {
    // missed, do glow
    // col += vec3(0.0, 0.5, 0.5);
    col += clamp(vec3(0.0, 0.5, 0.7) * d, 0.0, 1.0);
  }

  fragColor = vec4(col, 1.0);
  // fragColor = vec4(1.0, 1.0, 1.0, 0.5);

}