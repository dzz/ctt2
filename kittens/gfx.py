import shaders
import hwgfx

BLENDMODE_OVER              = 0
BLENDMODE_BRUSH_COMPOSITE   = 1
BLENDMODE_DAB_RENDERING     = 2
BLENDMODE_BLACKHOLE         = 600

class label(object):
    def __init__(self, text, color, fontsize ):
        self._label = hwgfx.label_generate()
        self.color = color;
        hwgfx.label_set_text(self._label,text)

    def __del__(self):
        hwgfx.label_drop(self._label)

    def set_text(text):
        hwgfx.label_set_text(self._label,text)

    def draw(self,x,y ):
        hwgfx.label_render(self._label,x,y,1,1,1);

def solid_rect(r,color):
    shader = shaders.get( [ "hwgfx/rect", 
                            "hwgfx/rect_solid" ] )

    shader.bind         ( [ "color", color ] )
    hwgfx.rect_draw( [0],r[1],r[2],r[3] )


def blend_enter(mode):
    hwgfx.blend_enter(mode)

def blend_exit():
    hwgfx.blend_exit();
