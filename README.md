# BGA_Game_portfolio

-----

<2D 턴제 알피지 포트폴리오>
쯔꾸르 알피지와 헬테이커에서 아이디어를 얻어 제작하였습니다.

- 윈도우 환경 : Visual Studio 2019 Community (C++, Win32, GDI+, OpenGL, OpenAL)
- 안드로이드 환경 : Android Studio (Java, NDK, OpenGL ES)

-----

#If you uese this Proejct, First Change Directory Setting 

"checkout merge conflict with same file when another change it differently."

#2021-03-08
- iString, iFPS, iWindow, WIN_TEST1 update
- iString(getLine, freeLine), iWindow(drawApp, Encoding), iSort

#2021-03-09
- assets=>add img(pokemon wallpaper)
- iWindow update, iArray add

#2021-03-10
- WIN_TEST1 update(keyDown & Up)
- separate typedef variable to iType, window method to iWindow 
- iArray update
- add iSize, iRect
<<<<<<< HEAD
#2021-03-11
- WIN_TEST1 update (WndProc)
- add iColor, update iStd(about Image), update iWindow (GDI+), update iPoint
#2021-03-14
- WIN_TEST1 update(fullScreen, WndProc)
- add fullScreen function, enforceResolution

#2021-03-11
- WIN_TEST1 update (WndProc)
- add iColor, update iStd(about Image), update iWindow (GDI+), update iPoint

#2021-03-23
- add tilemap func

#2021-03-24
- create TileMap class

#2021-03-25
- update TileMap Class -> MapTile Class

#2021-03-30
- update TileType, map, Character
- add Obj img(assets/Image)

------

#2021-07-11 
- delete previous files
- update files WIN32 Api + openGL 1.0 etc...

#2021-07-28
- openGL 1.x ver -> openGL 3.x ver (rendering with shader (alpha.vert, alpha.frag))

#2021-08-04
- add dot, Line, Rect, Circle, fillRect, vertex & fragment shader (GLSL in cpp)

#2021-08-12
- update fillRect using drawImage

#2021-09-13
- add class UI (effect to Menu, Stage, MapEditor, Battle etc...)

-- (update for Android) -----

#2021-09-14
- remove class UI
- add File MapEditorUI.h, .cpp / MenuUI.h, .cpp
- strcpy -> memcpy (for Android)

#2021-09-15
- update Loading (adjust for Android & Window)

#2021-09-16
- add StageUI, BattleUI (separate File, MapEditor, Menu, Stage, Battle)

#2021-09-24
- update MenuUI, StageUI, BattleUI, Character Files

#2021-09-26
- add Android project (in ADH_Android directory)

#2021-10-06
- made signing key for Release to Apk

#2021-10-07
- update MenuUI (adjust for Android)

#2021-10-26
- update Files Window, Android (BattleUI, Common, Intro, MenuUI, Stage, StageUI)
