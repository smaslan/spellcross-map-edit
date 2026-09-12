set name=../source/forms/form_objects
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.cpp" "forms.cpp" "FormObjects"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.h" "forms.h" "FormObjects"



