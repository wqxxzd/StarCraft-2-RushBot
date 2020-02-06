------------------------------------------------------------------------------------------

Reference:

Some building functions from s2client-api/examples/common/bot_examples.cc
helpers.cpp from bot_examples.cc


------------------------------------------------------------------------------------------

Known Problem:

--- Linker issue: 

I talked to Chris about this issue way before, I haven't found a solution to solve this yet. If I use the Cmake file under the debugbot directory and run makefile, I will get linker error. So for now, only the following compilation steps work unless the linker issue is solved on tester's machine.

The bot name can only be named as "DebugBot" for now.

	1. Go to Sc2LadderServer/tests/debugbot directory and copy DebugBotMain.cpp, BotPrivate.cpp, BotPublic.cpp helpers.cpp into this directory. 

	2. Copy the makefile from the root directory under build/tests/debugbot/ into Sc2LadderServer/tests/debugbot directory.

	3. Run make you just copied.

	4. You will find the executable file "DebugBot" under Sc2LadderServer/build/bin.

	5. Rename it as "RushBot" if necessary.




An alternative way to run the bot without using Sc2LadderServer

	1. Go to Sc2LadderServer/s2client-api/examples

	2. Copy tutorial.cc, BotPrivate.cpp, BotPublic.cpp helpers.cpp into this directory.

	3. Run tutorial.cc from VS or Xcode




--- Makefile

You can try to use the makefile I submitted but I am not sure whether it will work or not. If not, use the upper method to compile.


------------------------------------------------------------------------------------------

Sorry about this inconvenience. If our bot couldn't compile, please feel free to let us know.


 