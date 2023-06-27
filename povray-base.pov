#include "colors.inc"
#include "textures.inc"
#include "skies.inc"

camera {
    location <0, 128, 128>
    look_at <256, 0, 256>
    right x * image_width / image_height
}

#declare WIDTH = 512;
#declare HEIGHT = 512;
#declare LIGHT_Y = 256;

light_source {
    <0, LIGHT_Y, 0>
    color rgb <1, 1, 1>
}
light_source {
    <0, LIGHT_Y, HEIGHT>
    color rgb <1, 1, 1>
}    
light_source {
    <WIDTH, LIGHT_Y, 0>
    color rgb <1, 1, 1>
}  
light_source {
    <WIDTH, LIGHT_Y, HEIGHT>
    color rgb <1, 1, 1>
}
light_source {
    <WIDTH / 2, LIGHT_Y, HEIGHT / 2>
    color rgb <1, 1, 1>
}

light_source {
    <WIDTH / 2, LIGHT_Y, 0>
    color rgb <1, 1, 1>
}
light_source {
    <0, LIGHT_Y, HEIGHT / 2>
    color rgb <1, 1, 1>
}    
light_source {
    <512, LIGHT_Y, HEIGHT / 2>
    color rgb <1, 1, 1>
}  
light_source {
    <WIDTH / 2, LIGHT_Y, 512>
    color rgb <1, 1, 1>
}              

#include "generated.inc"

// If you don't like skyboxes, you can remove this statement
sky_sphere {
    S_Cloud5
}