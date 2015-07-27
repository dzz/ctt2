import client.system.audio as audio
from random import choice
class music_system:
    def __init__(self,music_file = None ):


        audio.get_track("DrumStatic", 128.0, True).play_clip(audio.get_clip("audio/drums/00.ogg",  16.0, 0.0), True)

        audio.get_track("DrumStatic").add_robin_clip( audio.get_clip("audio/drums/00.ogg", 16.0, 0.0) )
        audio.get_track("DrumStatic").add_robin_clip( audio.get_clip("audio/drums/01.ogg", 16.0, 0.0)  )
        audio.get_track("DrumStatic").set_robin_mode( "sequence" );

        #audio.get_track("DrumEffects",128.0, True).play_clip(audio.get_clip("audio/Tram01_DrumEffects.ogg", 64.0, 0.0), True).set_volume(0.0)
        #audio.get_track("Bass",       128.0, True).play_clip(audio.get_clip("audio/Tram01_Bass.ogg",        32.0, 0.0), True).set_volume(1.0)
        #audio.get_track("Pads",       64.0, True).play_clip(audio.get_clip("audio/Tram01_Pads.ogg",        16.0, 7.0), True).set_volume(0.0)
        #audio.get_track("Lead",       128.0, True).play_clip(audio.get_clip("audio/level_up.ogg",           16.0, 0.0), True).set_volume(1.0)
        audio.get_track("PickupHi",   128.0, True).play_clip(audio.get_clip("audio/Tram01_PickupHi.ogg",    16.0, 0.0), True).set_volume(0.0)
        audio.get_track("PickupLow",  128.0, True).play_clip(audio.get_clip("audio/Tram01_PickupLow.ogg",   16.0, 0.0), True).set_volume(0.0)

        audio.get_track("game_effects_0",128.0, True)
        audio.get_clip("audio/level_up.ogg", 1, -3.0) 

    def play(self):
        audio.beatEngine.play()
        return

    def track_volume(self,track,vol):
        audio.get_track(track).set_volume(vol)

    def track_pan(self,track,pan):
        audio.get_track(track).set_pan(pan)

    def trigger_event(self,event_name):
        if event_name == "level_up":
           audio.get_track("DrumStatic").retrigger()
           audio.get_track("game_effects_0").play_clip( audio.get_clip("audio/level_up.ogg"))
           #track_name = choice(["Bass","Lead", "Pads","DrumEffects"])
           #audio.get_track(track_name).retrigger()

           #if choice([False,True]):
           #    track_a = audio.get_track(choice(["PickupHi","Pads","PickupLow", "DrumEffects","Leads"]))
           #    track_b = audio.get_track(choice(["PickupHi","Pads","PickupLow", "DrumEffects","Leads"]))
           #    tmp = track_a.active_clip
           #    track_a.active_clip = track_b.active_clip
           #    track_b.active_clip = tmp



    def tick(self):
        pass
