# Changelog
## <cg>v1.6.0 & v1.6.1</c> (2025-04-30)
* <cg>Added</c> <cj>Completed Quests</c> getting stuck as <cr>unclaimable</c> fix

## <cg>v1.5.3</c> (2025-04-18)
* <cg>Enabled</c> <cj>Copy to Clipboard fix</c> on <cp>iOS</c>

## <cg>v1.5.1 & v1.5.2</c> (2025-04-11)
* <cg>Added</c> <cp>iOS</c> support

## <cg>v1.5.0</c> (2025-03-28)
* <cg>Fixed</c> a bug that caused the first MenuLayer to be incorrectly retained
* <cr>Removed</c> <cj>known broken mod versions check</c>
  * It has served its purpose

## <cg>v1.4.0</c> (2025-01-14)
* <cg>Added</c> <cj>automatic file cleanup</c>
  * This deletes files that were created by older versions of Geode but are not used for anything anymore
  * This also deletes old temporary files for uninstalled mods - this does NOT touch your save data
* <cg>Added</c> <cj>known broken mod versions check</c>
* <cg>Optimized</c> MenuLayer hooks

## <cg>v1.3.1</c> (2024-12-02)
* Rebuilt against newer Geode bindings in hopes to fix some more obscure issues

## <cg>v1.3.0</c> (2024-11-15)
* <cg>Added</c> <cj>GD 2.207</c> support
* <cr>Removed</c> <cj>Ignore Damage</c> <co>physics</c> fix
  * Bug fixed by RobTop now

## <cg>v1.2.1</c> (2024-08-30)
* <cg>Fixed</c> an <cp>Android-only</c> crash related to the lists fix

## <cg>v1.2.0</c> (2024-08-29)
* <cg>Added</c> <cj>Show Hitboxes</c> <co>Mirror Portal</c> fix
  * Both the visual bug and the physics change bug
* <cg>Added</c> <cj>Ignore Damage</c> <co>physics</c> fix
* <cg>Added</c> <cj>Wave Trail</c> teleporting when P2 enters a single portal fix
* <cg>Added</c> <cj>Settings</c> to disable some bugfixes
* <cg>Added</c> <cj>Lists</c> not loading levels fix
* <cr>Removed</c> <cj>Save and Play OOM fix</c>
  * Caused maintainability issues and potential mod conflicts
  * Also it is much less necessary in 2.206
* <cr>Removed</c> <cj>4GB Patch</c> (no longer necessary)

## <cg>v1.1.3</c> (2024-04-20)
* <cg>Improved</c> <cj>Geode Auto Updater</c> fix

## <cg>v1.1.2</c> (2024-03-28)
* <cg>Made</c> <cj>restarting</c> after applying <co>4GB Patch</c> <cy>safer</c>
* <cg>Fixed</c> a possible startup <cr>crash</c>
* <cg>Enabled</c> <cj>early-load</c> for the mod

## <cg>v1.1.1</c> (2024-03-15)
* <cg>Improved</c> <cj>CCMenuItem::activate</c> <cr>crash</c> fix

## <cg>v1.1.0</c> (2024-03-11)
* <cg>Added</c> <cj>CCMenuItem::activate</c> <cr>crash</c> fix
* <cg>Fixed</c> <cj>Save and Play</c> crashing for some <co>Mac</c> users with the mod installed

## <cg>v1.0.1</c> (2024-03-08)
* Initial release, labeled 1.0.1 because too many people downloaded actions builds that were labeled 1.0.0 and I want to be able to distinguish these