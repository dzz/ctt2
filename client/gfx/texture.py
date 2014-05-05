import hwgfx

#TODO:  this should be populated from the hardware's reported  maximum number 
#       of texture units. 

units = [ 0, 1, 2, 3 ]

class texture:
    def __init__(self, tex, w, h):
        self._tex   = tex
        self.w      = w
        self.h      = h
        print("PY: acquired texture ", self._tex)

    def __del__(self):
        hwgfx.texture_drop(self._tex)
        print("PY: dropped texture", self._tex)

    @classmethod
    def from_local_image(cls, local_image, filtered=False):
        tex = texture_generate( local_image.w, 
                                local_image.h, 
                                filtered)
        hwgfx.texture_upload(tex, local_image._img)
        return cls( tex, 
                    local_image.w, 
                    local_image.h )

    @classmethod
    def from_dims(cls, w,h,filtered=False):
        tex = hwgfx.texture_generate(w,h)
        return cls(tex, w, h)

    def upload_local_image(self,local_image):
        hwgfx.texture_upload(self._tex, local_image._img)

    def download_local_image(self, local_image):
        hwgfx.texture_download(self._tex, local_image._img)

    def bind(self, texture_unit):
        hwgfx.texture_bind(self._tex, texture_unit)

