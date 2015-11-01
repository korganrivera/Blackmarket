# Blackmarket
Source for my text-based rpg with a dynamic economic model. Project in progress.

If you want to play what I have so far, use mapmaker-weathertest.c, and the .txt files.  
gcc mapmaker.c
and put the .txt files in the same folder as your executable.  

All you can do so far is walk around the map with the arrow keys.  A new map is generated each time you run it.  The map has streets with names, neighbourhoods with names, shops with doors.  The doors work but the shops are currently empty.  Still doing infrastructure.  The game gives you simulated weather updates as you walk around.  There is a crude turn-based clock so I could test the weather.  It tells you which street you're on and hood you're in as you walk around.

2015.10.30
I just finished a function that allows stores to buy the most profitable items from the supplier.  This took a while.  It sounds like the knapsack problem, but it's a little different.  Next step is to make shop databases, so shops can keep track of their own inventory.  Not sure how I'll do this yet.
