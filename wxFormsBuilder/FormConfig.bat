set name=../source/forms/form_config
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.cpp" "forms.cpp" "FormConfig"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.h" "forms.h" "FormConfig"



