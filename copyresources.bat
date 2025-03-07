@echo off

xcopy Externals\Engine\project\application\resources\shaders\* GameProject\resources\shaders\ /W /I /F /Y /V

pause