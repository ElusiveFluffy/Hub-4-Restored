#pragma once

namespace Hub4SFX {
    //Only SFX added to hub 4 restored
    enum GlobalSound
    {
        SmashCrateSmash = 0x2db,
        //Just here to not need to change the count everytime a sound is added
        SFX_COUNT
    };

    //Needs to be updated otherwise it won't load any new sfx
    void UpdateGlobalSoundCount();
}