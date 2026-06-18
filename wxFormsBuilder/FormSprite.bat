set name=../source/forms/form_sprite_view
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.cpp" "forms.cpp" "FormSprite"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "%name%.h" "forms.h" "FormSprite"



