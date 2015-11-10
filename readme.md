# Blackmarket
Source for my text-based rpg with a dynamic economic model. Project in progress.

If you want to play what I have so far, use blackmarket-restocker.c, and the .txt files.  
gcc blackmarket-restocker.c
and put the .txt files in the same folder as your executable.  

All you can do so far is walk around the map with the arrow keys.  A new map is generated each time you run it.  The map has streets with names, neighbourhoods with names, shops with doors.  Shops are generated, a main supplier to the shops is generated and the shops automatically stock themselves with the content of the supplier.  The shops buy the most profitable items that they can store and afford.  They calculate what price they'd able to sell any number of bought items for in their own neighbourhood.  This is based on their estimate of the current demand function for that item in their neighbourhood.  

A items are sold in a hood, the demand will drop and the shop will have to drop the price to sell the same amount.  If demand increases, the shop can also raise the price.  This is all done automatically. 

Demand for an item in each hood increases and decreases according to how much of that item is sold there.  If an item isn't sold there for a while, demand will increase again.

The game gives you simulated weather updates as you walk around.  There is a crude turn-based clock so I could test the weather.  It tells you which street you're on and hood you're in as you walk around.

Shops have the infrastructure there to store a sales history for every item they sell.  They will use this to run linear regressions to predict the next best price for the items they're selling.  I haven't used this code so far however.  Currently, when the shop restocks at the suppliers, the supplier tells the shop the best price to sell its purchases at.  This might actually be a better method because I avoid running several hundred regression calculations.

There's also noise graphs generated for every item in the game.  The intention is to add some noise to the demand of items but I haven't used this yet.

PROBLEMS LIST
-------------
problem: shops stock up on every turn made, and that's the slowest part, so you can't walk around the map without having to wait on each step.  It's annoying.
possible solution: make it happen less often than on every turn.  Have it scheduled, or come up with a way to only run the calculations when the input is idle for so long, or learn multithreading.

problem: can't yet buy anything in the shop.
solution: do that.

problem: there is currently no game in the game.  it's just a simple economics simulation.
possible solution: once the speed issue is fixed, put in basic enemies, weapons, armour, etc.



