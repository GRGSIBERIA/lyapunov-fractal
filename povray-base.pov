#include "colors.inc"
#include "textures.inc"
#include "skies.inc"

camera {
    location <5, 5, 5>
    look_at <0, 0, 0>
    right x * image_width / image_height
}

light_source {
    <10, 10, 10>
    color rgb <1, 1, 1>
}              

object {
	plane { y, -4 }
	texture {
		pigment {
			checker
	    color rgb 1,
	    color rgb 0
		}
	}
}

#include "generated.inc"

// If you don't like skyboxes, you can remove this statement
sky_sphere {
    S_Cloud5
}