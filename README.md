# Misc Bugfixes
This mod several bugs in Geometry Dash. Bugfixes that affect gameplay are out of scope, as these could be considered cheating.

For a brief overview of fixed issues, see [about.md](about.md), this document serves as a detailed description of every issue fixed.

## Patched bugs
### <cj>Save and Play</c> out of memory <cr>crash</c>
Since Geometry Dash forces CCLocalLevels.dat to save before the editor is closed on Save and Play and Save and Exit, this can cause the game to run out of memory. For me personally, saving CCLocalLevels.dat allocated an additional 200 MB, which can be enough to push the game over the memory limit on Windows (2/4 GB depending on whether you have the patch installed) when object heavy levels are loaded.

The fix in this mod switches the game to a "Saving..." screen before saving the CCLocalLevels.dat file, which forces the editor to unload and therefore reduces the overall memory usage.

### <cr>Incorrect</c> <cj>Total Orbs Collected</c> (Demon Key Fix)
The "Total Orbs Collected" (GS_22) stat can desync if you Save & Load between devices without constantly syncing your progress due to the way the save file merging works, resulting in a lower overall number. This counter is also used for awarding Demon Keys, which means that they won't get awarded properly either. However, when you launch the game, the correct "Total Orbs Collected" (GSM::getTotalCollectedCurrency) value is calculated and used to award the proper amount of Demon Keys.

The end result is that if you hit the bug, you get the correct amount of Keys on restart, but you won't get any new ones until you restart the game again. The fix sets the Total Orbs Collected (GS_22) stat to the amount calculated on game launch (GSM::getTotalCollectedCurrency).

Note that it seems that Rob is already somewhat doing this as part of the game's anticheat - the vanilla game resets the "Total Orbs Collected" (GS_22) stat to the real value (GSM::getTotalCollectedCurrency), but only if GS_22 > GSM::getTotalCollectedCurrency.

### <cj>Copy to Clipboard</c> not working (Android only)
This is simply an unimplemented feature on Android. The fix adds it to the game.

### <cj>Saved Songs</c> page switching <cr>lag</c>
Whenever you switch pages in Saved Songs, the game checks if all songs in your save file are downloaded. If you have a lot of downloaded songs or a lot of saved levels, this can take quite a while, as it can result in thousands of disk accesses on every page load.

The fix caches the "is song downloaded" value the first time this access happens on every game launch and returns the cached value instead. This cached value is reset/updated whenever a song actually gets downloaded.

### <cj>Profiles</c> showing <cr>old/broken</c> usernames
Profiles always display cached usernames (often loaded from your save file), even though they actually receive correct ones when the profile is loaded.

The fix updates the displayed username when the profile is loaded.

### A <cr>crash</c> caused by <cj>refreshing and liking</c> <co>comments</c> at the same time
If a like dialog is spawned for a comment and the comment cell is destroyed in the meantime, the game has a chance to crash, as the GJComment object may be replaced.

The fix adds retain() and release() calls accordingly.

### <cr>"12 days left"</c> timer bug
Timestamps used for timers in Geometry Dash only use the bottom 20 bits from the Unix Timestamp integer. This restricts them to a time period of 1048576 seconds or roughly 12.1 days. This is done through a binary AND operation (timestamp & 0xFFFFF).

If you look into it, the reason why Rob initially added this makes sense:

The timer was presumably stored in a 32-bit float, which would cause a precision of 256 seconds with a full timestamp. With the AND, the timer would have a precision of 1/16th of a second, which is more than enough.

However, the timer is now stored in a 64-bit double, which has a much higher precision than the 32-bit float, so the AND is no longer necessary.

This is proven by Android behavior, where this trim does not happen.

The issue however still affects Windows, macOS and iOS. The fix simply removes the AND operation.

Note: This mod does not currently implement the fix on macOS, despite it being a supported platform for the other bugfixes.

### <cj>"My Lists" Search</c> showing <cr>incorrect usernames</c>
When loading "My Lists", the account ID is treated as a user ID while displaying the header text. Since there is usually no user cached for these IDs, this usually results in the header simply saying the default "Online Lists". However, occassionally, you can find a match and this results in the header saying a completely incorrect username. The mod BetterInfo significantly accelerates this bug, as it has its own additional logic for resolving usernames from user IDs.

The fix properly implements logic to resolve account IDs to usernames for list search. It also fixes the missing apostrophe in "x's Levels" and "x's Lists"

### <cj>Basement monster</c> <cr>softlock</c>
If you unlock the 3 keys in the basement and then save and close the game during the dialog with the monster, the result is that you've unlocked all 3 keys but the monster is still unfreed.

The fix checks for this specific condition and offers you to free the monster.