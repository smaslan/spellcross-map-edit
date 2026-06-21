set name=../source/forms/form_text
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.cpp" "forms.cpp" "FormText"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.h" "forms.h" "FormText"



