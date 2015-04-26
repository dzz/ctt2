from math                        import atan2, pi, floor
from client.system.gamepad       import get_gamepad
from client.math.helpers         import rectangles_intersect, neighbor_coordinates

WALL_TILE_INDEX = 3

class player:
    def __init__(self, game):
        self.x = 16.0
        self.y = 16.0
        self.player_speed = 0.1
        self.animation_threshold = 0.1

        #if this value was 0.5 (1.0/2) the player would take up an entire 'unit' e.g.
        #tile. We set it to something slightly smaller so the player can fit through
        #cooridors without being perfectly aligned
        self.player_collision_size_over2 = 0.45
        self.direction = "up"
        self.animation_states = [ "up", "left", "down", "right" ] 
        self.game = game


    def update(self):
        def update_animation(sprite,velocity_x,velocity_y):
            # toggle 
            if( abs(velocity_x) + abs(velocity_y) > self.animation_threshold ):
                sprite.set_animation_enabled(True)
            else:
                sprite.set_animation_enabled(False)

            if abs(velocity_x+velocity_y) > 0 :
                angle = ( (atan2( velocity_x, velocity_y ) + pi ) / (2*pi) ) * 4
                angle = max(0,int(angle))
                angle = min(angle,3)

                if( self.animation_states[angle] != self.direction ):
                    self.direction = self.animation_states[angle]
                    sprite.select_animation( self.direction )

        def get_collision_rect(self,x,y):
            if self.game.get_tile( x,y) == WALL_TILE_INDEX:
                return [ x, y, x+1, y+1 ] 
            return None


        pad         = get_gamepad(0)
        velocity_x  = pad.leftStick[0]*self.player_speed
        velocity_y  = pad.leftStick[1]*self.player_speed

        update_animation( self.game.player_sprite, velocity_x, velocity_y)

        collision_rects = []

        for x,y in neighbor_coordinates:
            collision_rect = get_collision_rect( self, int(self.x + x), int(self.y + y) )
            if( collision_rect is not None):
                collision_rects.append(collision_rect)


        new_x = self.x + velocity_x 
        new_y = self.y + velocity_y

        sz = self.player_collision_size_over2
        horizontal_player_rect = [ new_x-sz, self.y-sz, new_x+sz, self.y+sz ]

        horizontal_collision = False
        for collision_rect in collision_rects:
            if rectangles_intersect( horizontal_player_rect, collision_rect ):
                horizontal_collision = True
                break
        if not horizontal_collision:
            self.x = new_x

        vertical_player_rect = [ self.x-sz, new_y-sz, self.x+sz, new_y+sz ]

        vertical_collision = False
        for collision_rect in collision_rects:
            if rectangles_intersect( vertical_player_rect, collision_rect ):
                vertical_collision = True
                break

        if not vertical_collision:
            self.y = new_y

        # End of collisions. The following is just turning the angle of motion
        # into an index between 0..3 to select the animation for our sprite

