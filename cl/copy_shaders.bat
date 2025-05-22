@echo off
cd ..
xcopy Externals\Engine\project\application\resources\shaders\* GameProject\resources\shaders\* /I /Y
pause