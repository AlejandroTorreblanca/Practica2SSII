@echo off

set "files1=BH-F1.txt "


for %%f in (%files1%) do (	
	Practica2SSII.exe "BC-F.txt" "Config-F.txt" %%f  )
   


exit