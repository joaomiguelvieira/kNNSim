@echo off
REM forfiles /p . /s /m *.* /c "cmd /c magick @file @file.ppm"
REM forfiles /p . /s /m *.tif /c "cmd /c magick  convert -resize 256x256! @file @file.ppm"
forfiles /p . /s /m *.tif /c "cmd /c magick  convert -define filter:blur=5 -resize 256x256! @file @file.blur.ppm"
