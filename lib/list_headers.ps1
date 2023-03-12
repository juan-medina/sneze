gci -r -fi *.hpp `
| % {$_.FullName} `
| % {$_ -replace 'D:\\source\\sneze\\lib\\include\\sneze\\','#include "'} `
| % {$_ -replace '\\', '/'} | % {$_ -replace '.hpp', '.hpp"'} `
| Select-String -Pattern 'sneze.hpp' -NotMatch | clip
