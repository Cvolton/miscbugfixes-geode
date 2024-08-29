# Misc Bugfixes
This mod several bugs in Geometry Dash. Bugfixes that affect gameplay are out of scope, as these could be considered cheating.

For a brief overview of fixed issues, see [about.md](about.md), this document serves as a detailed description of every issue fixed.

## Patched bugs
### Main
#### <cr>Incorrect</c> <cj>Total Orbs Collected</c> (Demon Key Fix)
The "Total Orbs Collected" (GS_22) stat can desync if you Save & Load between devices without constantly syncing your progress due to the way the save file merging works, resulting in a lower overall number. This counter is also used for awarding Demon Keys, which means that they won't get awarded properly either. However, when you launch the game, the correct "Total Orbs Collected" (GSM::getTotalCollectedCurrency) value is calculated and used to award the proper amount of Demon Keys.

The end result is that if you hit the bug, you get the correct amount of Keys on restart, but you won't get any new ones until you restart the game again. The fix sets the Total Orbs Collected (GS_22) stat to the amount calculated on game launch (GSM::getTotalCollectedCurrency).

Note that it seems that Rob is already somewhat doing this as part of the game's anticheat - the vanilla game resets the "Total Orbs Collected" (GS_22) stat to the real value (GSM::getTotalCollectedCurrency), but only if GS_22 > GSM::getTotalCollectedCurrency.

Also the fixed value persists even after uninstalling the mod, however the differences are saved in the mod's saved.json (if any are applied), so you can loop through the array there and substract all of the numbers from GS_22 to restore the original value.

#### <cj>Copy to Clipboard</c> not working (Android only)
This is simply an unimplemented feature on Android. The fix adds it to the game.

#### <cj>Saved Songs</c> page switching <cr>lag</c>
Whenever you switch pages in Saved Songs, the game checks if all songs in your save file are downloaded. If you have a lot of downloaded songs or a lot of saved levels, this can take quite a while, as it can result in thousands of disk accesses on every page load.

The fix caches the "is song downloaded" value the first time this access happens on every game launch and returns the cached value instead. This cached value is reset/updated whenever a song actually gets downloaded.

#### <cj>Profiles</c> showing <cr>old/broken</c> usernames
Profiles always display cached usernames (often loaded from your save file), even though they actually receive correct ones when the profile is loaded.

The fix updates the displayed username when the profile is loaded.

#### A <cr>crash</c> caused by <cj>refreshing and liking</c> <co>comments</c> at the same time
If a like dialog is spawned for a comment and the comment cell is destroyed in the meantime, the game has a chance to crash, as the GJComment object may be replaced.

The fix adds retain() and release() calls accordingly.

#### <cr>"12 days left"</c> timer bug
Timestamps used for timers in Geometry Dash only use the bottom 20 bits from the Unix Timestamp integer. This restricts them to a time period of 1048576 seconds or roughly 12.1 days. This is done through a binary AND operation (timestamp & 0xFFFFF).

If you look into it, the reason why Rob initially added this makes sense:

The timer was presumably stored in a 32-bit float, which would cause a precision of 256 seconds with a full timestamp. With the AND, the timer would have a precision of 1/16th of a second, which is more than enough.

However, the timer is now stored in a 64-bit double, which has a much higher precision than the 32-bit float, so the AND is no longer necessary.

This is proven by Android behavior, where this trim does not happen.

The issue however still affects Windows, macOS and iOS. The fix simply removes the AND operation.

#### <cj>"My Lists" Search</c> showing <cr>incorrect usernames</c>
When loading "My Lists", the account ID is treated as a user ID while displaying the header text. Since there is usually no user cached for these IDs, this usually results in the header simply saying the default "Online Lists". However, occassionally, you can find a match and this results in the header saying a completely incorrect username. The mod BetterInfo significantly accelerates this bug, as it has its own additional logic for resolving usernames from user IDs.

The fix properly implements logic to resolve account IDs to usernames for list search. It also fixes the missing apostrophe in "x's Levels" and "x's Lists"

#### <cj>Basement monster</c> <cr>softlock</c>
If you unlock the 3 keys in the basement and then save and close the game during the dialog with the monster, the result is that you've unlocked all 3 keys but the monster is still unfreed.

The fix checks for this specific condition and offers you to free the monster.

#### <cj>Show Hitboxes</c> not working with <co>mirror portals</c>
While most of the framework for rendering mirrored hitboxes is in place, RobTop also accidentally mirrors the line width, which as a result becomes negative. This means that the line is drawn with 0px size, so it is not drawn at all in the end. The fix makes the line width use its absolute value, so negative line width still gets drawn.

Special thanks to Prevter for this fix.

#### <cj>Show Hitboxes</c> changing <cl>physics</c> during <co>mirror portal transitions</c>
`GameObject::determineSlopeDirection` is used, as you can guess from the name, to determine the slope orientation and flipped state. It hovewer does not return the value, but instead stores the states as GameObject members.
The issue now comes from the `GJBaseGameLayer::updateDebugDraw`, which calls this function if the object it wants to draw hitbox for is a slope. This changes the hitbox of the slope to it's horizontally flipped state very early and as a result makes the player die as if they hit the solid side of the slope. Note that this bug only occurs when hitboxes are visible.  

The fix simply disables that `GameObject::determineSlopeDirection` call, which fixes the issue.

Special thanks to Prevter for this fix and for the bug description (PR #10).

#### <cj>Ignore Damage</c> breaking <cl>slope physics</c>
To avoid illegitimate noclip hacks, the game has an anticheat measure, where after you spawn, you have to get killed by an invisible noclip spike. This is completely transparent to the user and doesn't trigger an actual death, instead it sets an "anticheat passed" flag internally. If this flag is not set, slope physics are different to mess with the user. Unfortunately RobTop didn't realize, that his noclip implementation would also trigger this anticheat, so the user is stuck with its consequences.

The fix forces a death by the anticheat spike when you spawn into a level with ignore damage enabled to make sure the anticheat passed flag is set correctly.

### Bugs not listed in about.md
These bugs are not listed in about.md, as their relevance to the end user is minimal, or they can be considered part of another bugfix. Despite that, this mod still patches them.

#### User IDs not saved for saved levels
This is not a bug in itself rather than a RobTop decision. However it has the side effect of possibly showing "-" as the username for the profile if you open it from a saved level cell.

The fix simply reverts this change and makes the user ID be saved for all saved levels.

#### Geode Updater Stuck
The current Geode auto updater remember when it last checked for updates and only asks the server if any updates have come out since that time. It, however, does not verify whether the update check was succesful. This can, in some rare circumstances, cause people to skip a release.

The fix forces Geode to check once again if the installed version is older than what was latest as of the last Geode update.

### Formerly patched bugs
#### CCMenuItem::activate crash
When you click a button, the game attempts to run code after its callback has been called. If the button callback removes the button (for example while closing a dialog), this can result in a crash, since it would already be removed and deallocated from memory when the post-callback code is called. This post-callback code is related to CCScriptManager, which can be used to attach scripts to buttons. Vanilla Geometry Dash never does this.

There are 2 possible fixes for this issue:
- Remove the CCScriptManager call, only making the button directly run the code it's meant to.
- Retain the button and release it after all calls have been made

Misc Bugfixes opts to choose the second option in case a mod desires to utilize the script feature of cocos for anything.

See also HJfod's Twitter for an alternative explanation: https://twitter.com/HJfod/status/1760076909663158588

This bugfix is still relevant, however it has been moved to [Geode itself](https://github.com/geode-sdk/geode/releases/tag/v2.0.0-beta.23) due to how easy the bug might be to trigger by mod devs.

#### "4 GB Patch"
32-bit Windows applications require the "Large Address Aware" flag to be enabled to be able to be able to use more than 2 GB RAM. This is disabled by default in most compilers, including MSVC, which RobTop is using. Enabling this flag is a single-byte patch of the exe and doing this raises the limit to 4 GB, which is the absolute technical maximum 32-bit applications can use in general.

The fix edits GeometryDash.exe to enable this flag. This edit persists even after the mod is uninstalled but a backup is available in the GD path, should the user wish to restore the original executable.

The fix has been made irrelevant by the 2.206 update, which made the game 64-bit on Windows. Since 64-bit applications have the Large Address Aware flag enabled by default, Geometry Dash can now use all memory available on a particular system.

### <cj>Save and Play</c> out of memory <cr>crash</c>
Since Geometry Dash forces CCLocalLevels.dat to save before the editor is closed on Save and Play and Save and Exit, this can cause the game to run out of memory. For me personally, saving CCLocalLevels.dat allocated an additional 200 MB, which can be enough to push the game over the memory limit on Windows (2/4 GB depending on whether you have the patch installed) when object heavy levels are loaded.

The fix in this mod switches the game to a "Saving..." screen before saving the CCLocalLevels.dat file, which forces the editor to unload and therefore reduces the overall memory usage.

This fix has been disabled due to it being difficult to maintain across versions and the crash being much harder to trigger with the introduction of the 64-bit Windows version. It is also a potential point of conflict with EditorCollab and BetterEdit mods.