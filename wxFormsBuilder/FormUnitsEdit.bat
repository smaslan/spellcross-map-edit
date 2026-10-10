set name=../source/forms/form_units_edit
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.cpp" "forms.cpp" "FormUnitsEdit"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.h" "forms.h" "FormUnitsEdit"



