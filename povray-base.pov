#include "colors.inc"
#include "textures.inc"
#include "skies.inc"

camera {
    location <4, 4, 4>
    look_at <0, 0, 0>
    right x * image_width / image_height
}

light_source {
    <10, 10, 10>
    color rgb 1
}              

object {
    box {
        <0, 0, 0>
        <1, 1, 1>
    }
    translate { <0, 0, 0> }
    scale { <1, 1, 1> }
    texture {
        pigment { rgb <1,0,0> }
    }
}

// If you don't like skyboxes, you can remove this statement
sky_sphere {
    S_Cloud5
}