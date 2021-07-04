uniform vec3 initial_pos;
uniform vec3 initial_vel;
uniform vec3 horizon_unit;
uniform vec3 vertical_unit;
uniform int max_depth;

uniform int top_level_bvh_index;

uniform int number_of_lights;

uniform int sample_per_iteration;

uniform vec3 random_vector;

uniform sampler2D input_color;
uniform usampler2D init_random_seed[16];
uniform writeonly image2D output_color;

