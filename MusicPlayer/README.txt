/*********************************************/
version:v1.8

1. Introduction
    The music player reads data from SD card and then feed into the VS1053 chip via SPI bus. This is done in TIMER1's
    ISR with a interval of 2ms. This job has a high priority and uses the cpu exclusively. In the spare cpu time, player
    does some process control, like play/pause.
    
    Control:
    short press - toggle play and pause
    long press (more than 2 seconds)  - stop 
    long press in stop mode - record
    short press in record mode - stop recording
    short press left in play mode - shift to previous song
    long press left in play mode - fast rewind
    short press right in play mode - shift to next song
    long press right in play mode - fast forward
    short press up - volume plus 6
    press up and hold - volume up continuously
    short press up - volume minus 6
    press down and hold - volume down continuously
    
    Default setting:
    play mode = NORMAL play
    keys      = ENBALE	  
    scan available digit pins = DISABLE	 
    scan available analog pins= DISABLE 
    create an empty playlist
    
2. API:
    
    -- global variables --
    MusicPlayer player                          -- MusicPlayer class instance
    
    -- setup APIs --
    void        MusicPlayer::begin(void)        -- Music player initialization routine
    void        MusicPlayer::playOne(char *songName)
                                                -- Add a song to play list , equivalent to call addToPlaylist once
    boolean     addToPlaylist(char *songName)   -- Add a song to play list
    void        scanAndPlayAll(void)            -- Scan all the songs in TF card and add them to play list
    void        attachDigitOperation(int pinNum, void (*userFunc)(void), int mode)
                                                -- attach a callback to digital pin event
    void        attachAnalogOperation(int pinNum, void (*userFunc)(void))
                                                -- attach a callback to analog pin event
    
    -- config APIs --
    void        setVolume(unsigned char volume) -- setup volume (range: 0x00 - 0xfe )
    void        adjustVolume(boolean UpOrDown, unsigned char NumSteps = 6)
                                                -- adjust volume up or down by step (UpOrDown=true for up)
    void        setPlayMode(playMode_t playmode)-- set play mode (modes: refer to keywords.txt Constants)
    boolean     deleteSong(char *songName)      -- delete a song from play list
    void        keyEnable(void)                 
    void        keyDisable(void)
    void        analogControlEnable(void)
    void        digitalControlEnable(void)
  
    -- operate APIs --
    void        play(void)
    void        opPlay(void)  
    void        opPause(void) 
    void        opStop(void)  
    void        opVolumeUp(void)  
    void        opVolumeDown(void)  
    void        opNextSong(void)    
    void        opPreviousSong(void)
    void        opFastForward(void)
    void        opFastRewind(void) 
  
3. How to use:
    (1) make sure SD lib and SPI lib was installed;
    (2) include header files like example do;
    (3) call player.begin() in setup() function;
    (4) call other setup APIs or config APIs in setup() function;
    (5) call player.play() in loop() function;
    (6) call other config APIs in loop() function;
    (7) then forget the music player and insert your own routine into setup() or loop() function, your own routine
        will be executed in available cpu time.

4. Three different ways to play music:
    a. play a song with its name by calling the function player.playOne(songfile);
    b. add some songs into the default empty playlist(player.addToPlaylist());
    c. play all the songs in SD card root dir by calling the function player.scanAndPlayAll().

5. Notice PLS:
    a. Libs dependency: SD lib, SPI lib;
    b. TIMER1 is used;
    c. As the player need handling some control event(e.g.: key control, playlist processing ) circularly,
       you should call player.play() in the arduino loop function. Otherwise, the player will not play;
    d. ALL THE SONGS TO PLAY SHOULD BE IN THE ROOT DIRECTORY AND SONG NAME CAN NOT CONTAIN WIDE CHAR;
    e. setup APIs can only be called in setup() function.

2013/7/24 by Jacky Shao, Seeed Studio.
/**********************************************/