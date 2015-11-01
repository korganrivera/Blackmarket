# Blackmarket
Source for my text-based rpg with a dynamic economic model. Project in progress.

If you want to play what I have so far, use mapmaker-weathertest.c, and the .txt files.  
gcc mapmaker.c
and put the .txt files in the same folder as your executable.  

All you can do so far is walk around the map with the arrow keys.  A new map is generated each time you run it.  The map has streets with names, neighbourhoods with names, shops with doors.  The doors work but the shops are currently empty.  Still doing infrastructure.  The game gives you simulated weather updates as you walk around.  There is a crude turn-based clock so I could test the weather.  It tells you which street you're on and hood you're in as you walk around.

2015.10.30
I just finished a function that allows stores to buy the most profitable items from the supplier.  This took a while.  It sounds like the knapsack problem, but it's a little different.  Next step is to make shop databases, so shops can keep track of their own inventory.  Not sure how I'll do this yet.

2015.11.01
Just added the above algorithm to the game, and a bunch of databases.  The game now has a database of actual shops.  Each shop has its own inventory, and can track the sales history if every item.  The shops can also use my SupplierPurchasing algorithm to do their first stocking at the suppliers, where they buy the most profitable stuff they can within the limits of their warehouse space and cash.  That's pretty cool.  I've let the supplier give shops an initial rrp for items.  This rrp is based on a global desire for the product.  Oh, also every product in the game has its own desire values, and noise graph which will be used together to modify desire over time.  But this hasn't been utilised yet, it's still background stuff.  Anyway, I'd prefer each neighbourhood to have its own desire graphs for items, so you can take advantage of differences across the map, but I'll do that later.  Next step is to setup a working script for transactions when you walk into a store, and also have the stores restock on a frequent basis.