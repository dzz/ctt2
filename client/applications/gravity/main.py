from .background import background
from .vortex import vortex
from .player import player
from client.system.gamepad       import get_gamepad
from client.gfx.sprite           import sprite, sprite_renderer
from client.gfx.tileset          import tileset
from client.gfx.coordinates      import centered_view, Y_Axis_Down

class game:
    def __init__(self):
       self.background = background()
       self.vortex = vortex()
       self.player = player()

       configuration = {
                "image"         : "ship.png",
                "imageheight"   : 16,
                "imagewidth"    : 64,
                "margin"        : 0,
                "spacing"       : 0,
                "properties"    : {},
                "firstgid"      : 0,
                "tileheight"    : 16,
                "tilewidth"     : 16,
                "tileproperties" : {} 
                }

       self.sprite_tileset  = tileset( configuration = configuration, img_path = "sprite/" )

       self.ship_renderer = sprite_renderer( tileset = self.sprite_tileset, coordinates = centered_view(1920,1080, Y_Axis_Down))
       self.player_sprite = sprite(
                                        sprite_renderer = self.ship_renderer,
                                        named_animations = {
                                                                "default" : [0]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 1
                                     )
       self.engine_sprite = sprite(
                                        sprite_renderer = self.ship_renderer,
                                        named_animations = {
                                                                "default" : [1]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 1
                                     )
       self.fire_sprite = sprite(
                                        sprite_renderer = self.ship_renderer,
                                        named_animations = {
                                                                "default" : [2,3]
                                                                },
                                        current_animation = "default",
                                        ticks_per_frame = 5
                                     )

    def tick(self):
       pad         = get_gamepad(0)
       #print( pad.left_stick);
       self.background.update()
       self.player.tick(pad,self.vortex)
       self.fire_sprite.tick()
       self.background.x = self.player.x
       self.background.y = self.player.y

    def render(self):
        self.background.render()

        batch  = [];



        if(self.player.firing>0):
            batch.append([

                 self.fire_sprite,
                 [-8,-2],
                 6,
                 self.player.eng_r,
                 [0.0,0.0],
                 2 ])
            batch.append([

                 self.engine_sprite,
                 [-8,-8],
                 4,
                 self.player.eng_r,
                 [0.0,0.0],
                 2 ])

        batch.append([   

            self.player_sprite,
            [-8,-8],
            4,
            self.player.r,
            [0.0,0.0],
            2 
            ])




        self.ship_renderer.render(batch)



global g
def init():
    global g
    g = game()

def tick():
    g.tick()

def render():
    g.render()

def finalize():
    pass

def configure( configuration ):
    pass
