# Battery Watcher
**IN DEVELOPTMENT**

This program will read ACPI status file of AC power supply to see if a change in
state (whether the AC was plugged or unplugged) happened, allows for running executable
files accordingly.

This solution uses an infinite loop to read a file every second (or another custom set delay time)
to detect state changes and play a set sound, the 'sleep' function is used carefully since it's not
resource free,

Originally [ACPower Sound](https://github.com/DannyXjsu/acpower-sound).

## Usage
Check the help message by using parameter '-h'

```
usage: battery-watcher [-h] [-l] [-v] [-i executable] [-o executable]

Reads AC status and detects state changes..

option:
        -i      Relative path to file that gets executed when AC plugged.
        -o      Relative path to file that gets executed when AC unplugged.
        -h      Show this help message and exit.
        -l      Enables lite mode - run once and execute file depending of current AC state.
        -v      Enable verbose output - prints current state every loop.
```

### Autostart

I recommend creating a systemd user service so it starts every time you login, there is an example systemd service file.

```bash
# Copy/move this file to user config folder
$ cp battery-watcher.service $XDG_CONFIG_HOME/systemd/user/

# Reload daemon (updates systemd stuff)
$ systemctl --user daemon-reload

# Enable it 
$ systemctl --user enable --now battery-watcher
```


## Configuration
Change the necessary defines in the source file.


## Performance
You can use the lite mode with the mute option to measure the time it takes the program to run, I think it's about as fast and as light on resources as I can make.
