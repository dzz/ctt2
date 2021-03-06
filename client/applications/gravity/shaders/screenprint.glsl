
uniform sampler2D buffer;

uniform float time;
uniform vec2 cam;
uniform float scale;
uniform vec4 col1;
uniform vec4 col2;
uniform vec4 col3;
uniform float steps;
uniform float atan_factor;
uniform float circle_factor;
uniform float cosmunge_factor;
uniform float wobble_factor;
uniform float crazy;
uniform float r;


in vec2 uv;

void main(void) {

    vec2 scam = cam*0.001*crazy;
    vec3 smpl = texture(buffer,(0.1*uv*scale) - scam)*col3;
    vec3 smpl2 = texture(buffer,(0.3*uv*scale) - (cos(time)*scam));
    smpl*=sin(time*circle_factor);
    vec2 uv_t;

    uv_t.x = uv.x*smpl.r+time;
    uv_t.y = uv.y*smpl.g;

    gl_FragColor = (((col1*(texture(buffer,(uv* scale) - scam )) * (0.5+smpl.b)) +
                   (col2*(1-texture(buffer,(uv))))) + 
                            (col3*(smpl2.r,smpl.g,smpl2.b,1.0)))/2;
}
