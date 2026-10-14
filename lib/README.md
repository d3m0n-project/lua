# Custom d3m0n lua modules

This folder contains all builtins provided to interract correctly with the d3m0n system.
Note that these are loaded at compile time into the lua executable and need recompiling the binary after module modification.

## How to make mine?
Simply copy the provided [template](./template/) into `lib` folder and rename it as you want.

Create a single C file with the same name as the folder inside of it.

###  e.g. : 
```bash
cp -r lib/template/ lib/my_super_module/
mv lib/my_super_module/template.c lib/my_super_module/my_super_module.c
```

You can now edit your module functions as shown in the [template](./template/template.c)

You will also need to tell the auto module loader that this new super module is available for the next lua compilation. Register them inside [libs.h](./libs.h)