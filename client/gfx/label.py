import hwgfx

class label:
    def __init__(self, text, fontsize ):
        self._label = hwgfx.label_generate()
        hwgfx.label_set_text(self._label,text)
        print("PY: acquired label ", self._label)

    def __del__(self):
        hwgfx.label_drop(self._label)
        print("PY: dropped label", self._label)

    def set_text(text):
        hwgfx.label_set_text(self._label,text)

    def draw(self,x,y, color):
        hwgfx.label_render(self._label,x,y,color[0],color[1],color[2]);
