import json
from client.gfx.tileset         import tileset
from client.gfx.rect            import rect_tile, rect_tile_start, rect_tile_raw, rect_tile_end
import client.ctt2.host_config  as host_config

class tilemap:
    def __init__(self, configuration, img_path, filtered = False ):
        self.tilesets = []
        self.gid_tileset_map = {}  
        self.layers = []
        self.tileheight = configuration["tileheight"]

        for tileset_definition in configuration["tilesets"]:
            ts = tileset( tileset_definition, img_path) 
            for gid in range( ts.firstgid, ts.gidcount):
                self.gid_tileset_map[gid] = ts
            self.tilesets.append(ts)

        for layer_definition in configuration["layers"]:
            layer = {}
            layer["height"] = layer_definition["height"]
            layer["width"] = layer_definition["width"]
            layer["data"] = layer_definition["data"]
            self.layers.append(layer)
          
    def render(self,org_x,org_y,scale, debug=False ):
        active_ts = None
        for layer in self.layers:
            gid_idx = 0
            for y in range(0, layer["height"]):
                for x in range(0, layer["width"]):
                    gid_id = layer["data"][gid_idx]
                    if(gid_id>0):
                        ts = self.gid_tileset_map[gid_id]
                        if(ts is not active_ts):
                            if active_ts is not None:
                                rect_tile_end(active_ts)
                            active_ts = ts
                            rect_tile_start(ts)
                        if not debug:
                            rect_tile_raw( ts, gid_id, org_x+((x*self.tileheight))*scale, org_y+((y*self.tileheight))*scale, scale)
                    gid_idx+=1

    def gid_via_coord(self,x,y,layer):
        i = x+(y*self.layers[layer]["width"])
        gid_id = self.layers[layer]["data"][i]
        return gid_id

    def tile_prop_via_coord(self,x,y,layer,key):
        i = x+(y*self.layers[layer]["width"])
        gid_id = self.layers[layer]["data"][i]
        ts = self.gid_tileset_map[gid_id]
        return ts.tile_prop(gid_id,key)

    @classmethod 
    def from_json_file(cls, path, img_path, filtered=False ):
        root = host_config.get_config("app_dir")
        json_parsed = {}
        with open("{0}{1}".format(root,path)) as f:
            json_data = f.read()
            json_parsed = json.loads(json_data)
        return cls(json_parsed, img_path, filtered)
