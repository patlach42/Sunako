# mini-host plugin loader

What is Sunako?
---------------
Name is referenced to Sunako Nakahara.
Sunako is fork of Chibi - so it's still mini host, but this mini-host is can hide to tray. You can access some carla 
options for plugin from tray context menu.

Usage:
```
./Sunako -n <title_and_jack_client_name> -m -i <png_icon_path> <path_to_VST2_plugin>
```


What is Chibi?
---------------

Chibi is a mini-host audio plugin loader, meant to load one plugin at a time as if it was a standalone application.  

These are the goals for the project:
 - Provide a quick way to start audio plugins
 - Integrate with relevant Session Managers
 - Allow to be reused for other projects, so plugins can literally become standalones
 - Provide the most useful tools from host-side without any extra plugin work
 - Leverage and test Carla as much as possible (avoiding duplicate work and having head-start on features)

Chibi is basically Carla's little sister.  
It runs Carla's backend behind the scenes and shares quite a few visual traits.  
Building on top of what Carla has already achieved, it adds only the missing pieces for a "mini-host" setup.


