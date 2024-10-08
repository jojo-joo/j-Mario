setlocal

cd /d %~dp0

makefsfile.exe -h -r -i ./asset -o fsdata.h

endlocal
