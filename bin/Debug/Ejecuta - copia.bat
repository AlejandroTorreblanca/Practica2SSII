@echo off

set "files1=BH-F1.txt BH-F2.txt BH-F3.txt BH-F4.txt"
set "files2=BH-I1.txt BH-I2.txt BH-I3.txt BH-I4.txt"

for %%f in (%files1%) do (	
	Practica2SSII.exe "BC-F.txt" "Config-F.txt" %%f  )

for %%f in (%files2%) do (	
	Practica2SSII.exe "BC-I.txt" "Config-I.txt" %%f  )
   


exit