import ui_area

mouse_focused_area = None
caret_handler = None

def init():
    ui_area.init_ui_areas()

def finalize():
    pass

def tick():
    render()

def render():
    for area in ui_area.order_areas():
        for renderer in area.renderers:
            renderer.render(area)
    
def calculate_mouse_position(area,x,y):
    xt = min(max( 0, x - area.r[0] ),area.r[2])
    yt = min(max( 0, y - area.r[1] ),area.r[3])
    return [xt,yt] 

def dispatch_mouseup(button,x,y):
    global mouse_focused_area
    if mouse_focused_area is not None:
        mouse_focused_area.rcv_mouse_button(button,x,y, down = False)
        mouse_focused_area.active = False
        mouse_focused_area = None

def dispatch_mousedown(button,x,y):
    global mouse_focused_area
    area = ui_area.find_ui_area(x,y)
    if area is not None:
        area.rcv_mouse_button(button,x,y, down = True)
        area.active = True
        mouse_focused_area = area

def dispatch_mousemotion(x,y):
    global mouse_focused_area
    if mouse_focused_area is not None:
        area = ui_area.find_ui_area(x,y)
        if area is not None:
            area.set_m(calculate_mouse_position(area,x,y))

def dispatch_key(key,down):
    global caret_handler
    if(caret_handler == None):
        dispatch_root_keybindings(key,down)
        if(mouse_focused_area is not None):
            area.rcv_key(key,down)
        return
    caret_handler.rcv_key(key,down)

def dispatch_root_keybindings(key,down):
    pass

def caret_release():
    global caret_handler
    caret_handler = None
