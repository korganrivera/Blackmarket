//  call with sindex=1 to start numbering streets at 1.  don't use zero, as that's used to mark non-street parts.
void streetmaker(unsigned x1, unsigned y1, unsigned x2, unsigned y2, char *map, unsigned blocksize, unsigned widthofmainmap, unsigned *stindexmap, unsigned sindex){
    unsigned vside, hside, p, i;
    char vsideflag=0;

    if(DEBUG) printf("\n\tentered streetmaker with (%u, %u, %u, %u, blocksize=%u, widthofmainmap=%u)", x1, y1, x2, y2, blocksize, widthofmainmap);
    if(DEBUG && map==NULL) puts("\t*map is null!!!!");
    if(DEBUG && stindexmap==NULL) puts("\t*stindexmap is null!!!!");



    //  calculate length of sides.
    vside = x2-x1+1;
    hside = y2-y1+1;

    if(DEBUG) printf("\n\tvside=%u, hside=%u", vside, hside);

    //  if both sides are small enough, we're done.
    if(vside<=blocksize && hside<=blocksize) return;

    //Otherwise, if both sides are larger enough to be split, choose randomly between
    //them, but do it proportionally to the ratio of horizontal to vertical cuts made
    //so far. This way, if vertical cuts are dominating, then horizontal ones will
    //become more likely, and vice versa. This should help to stop wacky ratios from
    //making cities have too much of one and not the other.
    else if(vside>blocksize && hside>blocksize) vsideflag=(hcount<(rand()%(hcount+vcount+2)));

    //  else if the vertical side is the only one that can be cut, do it.
    else if(vside>blocksize) vsideflag=1;

    if(DEBUG) printf("\n\tvsideflag: %u", vsideflag);

    if(vsideflag){
        vcount++;
        p = rand()%(vside-2)+1;
        if(DEBUG) printf("\n\tp=%u", p);

        if(DEBUG) printf("\n\tmarking cells...");
        for(i=y1; i<=y2; i++) {
                *(map+(p+x1)*widthofmainmap+i) = STREET;                //  mark that cell as a street.
                *(stindexmap+(p+x1)*widthofmainmap+i) = streetindex;    //  give that cell a street index number on the street index map.
        }
        if(DEBUG) printf("done,");

        streetindex++;                                                  //  increment index to give the next street a new name/index.
        if(DEBUG) printf("\n\t1st recurse...");
        streetmaker(x1, y1, p-1+x1, y2, map, blocksize, widthofmainmap, stindexmap);
        if(DEBUG) printf("\n\treturned from 1st recurse.");
        if(DEBUG) printf("\n\t2nd recurse...");
        streetmaker(p+1+x1, y1, x2, y2, map, blocksize, widthofmainmap, stindexmap);
        if(DEBUG) printf("\n\treturned from 2nd recurse.");
    }
    else{
        hcount++;
        p = rand()%(hside-2)+1;

        if(DEBUG) printf("\n\tp=%u", p);

        if(DEBUG) printf("\n\tmarking cells...");
        for(i=x1; i<=x2; i++){
                *(map+i*widthofmainmap+y1+p) = STREET;
                *(stindexmap+i*widthofmainmap+y1+p) = streetindex;
        }
        if(DEBUG) printf("\n\t");
        streetindex++;
        if(DEBUG) printf("\n\t1st recurse...");
        streetmaker(x1, y1, x2, p-1+y1, map, blocksize, widthofmainmap, stindexmap);
        if(DEBUG) printf("\n\treturned from 1st recurse.");
        if(DEBUG) printf("\n\t2nd recurse...");
        streetmaker(x1, p+1+y1, x2, y2, map, blocksize, widthofmainmap, stindexmap);
        if(DEBUG) printf("\n\treturned from 2nd recurse.");
    }
    if(DEBUG) printf("\n\tleaving streetmaker().");
}
