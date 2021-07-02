# Make a release

1. If you need to build a release start by building the project in **release**.   
1. Copy all `*.dll` and `WindowBuilder.exe` under **Release** folder and past it into **doc\Window Builder.app**


##  Adding the tool
> this section is extracted from [cyanic thread on example tool](https://forums.rpgmakerweb.com/index.php?threads/adding-your-own-tools-to-rpg-maker-mv.86920/)

Put your tool files and "Identification" folder inside a single folder. Depending on how you've obtained your copy of RPG Maker, you can put this tool folder in the following locations for it to be automatically detected by the editor:

Windows:
- *Standalone*: `%PROGRAMFILES(X86)%\KADOKAWA`
- *Steam*: `%PROGRAMFILES(X86)%\Steam\steamapps\common\RPG Maker MV\tool`

Note: for the Steam version, it really is hardcoded to the default library location in Program Files. It's probably an oversight that it isn't looking inside where RPG Maker is currently installed, despite it being perfectly tenable within the current framework.

Mac:
- Any version: inside your Applications folder
- *Steam*: `~/Library/Application Support/Steam/steamapps/common/RPG Maker MV`

To add the tool to your menus and toolbars, open a project, then go to "Tools -> RPG Maker MV Tools...". If you've added your tool to the locations the editor searches for, it should show up under Found Tools. If not, click "Select Path...", and browse to the tool's folder (or its parent folder). If it doesn't show up even after browsing, then you have used an unrecognized file name. Select the tool, click "Add", then "OK". The tool should now show up in your Tools menu and at the end of your toolbar. Click on the toolbar button or the Tools menu item to launch the tool with your currently open project. Note you cannot launch tools or manage them if you do not have a project open.
