# ARK PSPSDK

1. Clone the repo

2. Place `pspdev` directory in `/usr/local'

3. Append to `$HOME/.bashrc`

```sh
  export PSPDEV="/usr/local/pspdev"
  export PATH="$PATH:$PSPDEV/bin"
```

4. Verify. Run `psp-gcc --verision`

Output should look like:

```sh
  psp-gcc (GCC) 4.3.2
  Copyright (C) 2008 Free Software Foundation, Inc.
  This is free software; see the source for copying conditions.  There is NO
  warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.```
```
    
